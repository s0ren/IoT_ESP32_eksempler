#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

void connectToWiFi();
void resetWiFiSettings();

Preferences preferences;
AsyncWebServer server(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;
bool dnsServerActive = false;

const char* apSSID = "ESP32_AP_CONFIG_smag";
const char* apPassword = "24.12.2025";

const char* htmlForm = R"rawliteral(
<!DOCTYPE html>
<html>
  <body>
    <h2>WiFi Konfiguration</h2>
    <form action="/save" method="POST">
      SSID: <input name="ssid"><br>
      Password: <input name="password" type="password"><br>
      <input type="submit" value="Tilslut">
    </form>
    <form action="/reset" method="GET">
      <input type="submit" value="Nulstil WiFi">
    </form>
  </body>
</html>
)rawliteral";

const char* htmlLanding = R"rawliteral(
<!DOCTYPE html>
<html>
  <body>
    <h2>ESP32 er forbundet</h2>
    <p>Enheden er tilsluttet netværket og klar til brug.</p>
  </body>
</html>
)rawliteral";

const char* htmlResetConfirm = R"rawliteral(
<!DOCTYPE html>
<html>
  <body>
    <h2>WiFi-indstillinger nulstillet</h2>
    <p>Gemte credentials er slettet.</p>
    <p>Genstart enheden for at gå i konfigurationsmode.</p>
    <p><a href=\"/\">Tilbage</a></p>
  </body>
</html>
)rawliteral";

void resetWiFiSettings() {
  preferences.begin("wifi", false);
  preferences.remove("ssid");
  preferences.remove("password");
  preferences.end();
  Serial.println("Gemte WiFi-indstillinger er slettet.");
}

void connectToWiFi() {
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  if (ssid == "") return;

  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Forbinder til ");
  Serial.println(ssid);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Forbundet! IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("Kunne ikke forbinde - holder AP aktiv.");
  }
}

void startAccessPoint() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSSID, apPassword);

  IPAddress apIP = WiFi.softAPIP();
  if (dnsServer.start(DNS_PORT, "*", apIP))
    dnsServerActive = true;

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlForm);
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid, password;

    if (request->hasParam("ssid", true)) {
      ssid = request->getParam("ssid", true)->value();
    }
    if (request->hasParam("password", true)) {
      password = request->getParam("password", true)->value();
    }

    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    delay(1000);
    connectToWiFi();

    String newIP = WiFi.localIP().toString();
    String redirectPage =
      "<html>\n"
      "<body>\n"
      "<h3>Forbinder til nyt netværk...</h3>\n"
      "<p>Hvis forbindelsen lykkes, kan du besøge ESP32 på følgende IP:</p>\n"
      "<p><strong>" + newIP + "</strong></p>\n"
      "<p><a href='http://" + newIP + "'>Åbn ESP32</a></p>\n"
      "<script>setTimeout(function() { window.location.href = 'http://" + newIP + "'; }, 7000);</script>\n"
      "</body>\n"
      "</html>\n";

    request->send(200, "text/html", redirectPage);
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    resetWiFiSettings();
    request->send(200, "text/html", htmlResetConfirm);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.println("[INFO] Ukendt sti: " + request->url());
    request->send(404, "text/plain", "404: Ikke fundet");
  });

  server.begin();
  Serial.println("AP + Async Webserver startet på: " + apIP.toString());
}

void startLandingPage() {
  dnsServer.stop();
  dnsServerActive = false;
  WiFi.softAPdisconnect(true);

  server.reset();  // Kun nødvendig hvis serveren genstartes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlLanding);
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    resetWiFiSettings();
    request->send(200, "text/html", htmlResetConfirm);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "404: Ikke fundet");
  });

  server.begin();
  Serial.println("Landing page tilgængelig på: " + WiFi.localIP().toString());
}

void setup() {
  Serial.begin(9600);
  preferences.begin("wifi", false);

  WiFi.mode(WIFI_STA);
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  if (ssid != "") {
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("Prøver at forbinde til kendt WiFi...");

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
    }
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi fejlede. Starter AP...");
    startAccessPoint();
  } else {
    Serial.println("Forbundet som klient. IP: " + WiFi.localIP().toString());
    startLandingPage();
  }
}

void loop() {
  if (dnsServerActive) {
    dnsServer.processNextRequest();
  }
}
