# ESP32 WiFi Setup Portal – gennemgang af kode uden LittleFS

Denne gennemgang beskriver, hvordan programmet fungerer linje for linje. Formålet er at gøre det muligt for ESP32 at forbinde til et netværk automatisk og – hvis det fejler – oprette et access point med en webserver til WiFi-konfiguration.

---

## 1. Inkludering af biblioteker 

(linje 4–8)
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <LittleFS.h>
```

Her inkluderes nødvendige biblioteker:
- `WiFi.h` styrer STA- og AP-tilstande.
- `WebServer.h` driver den indlejrede HTTP-server.
- `DNSServer.h` muliggør captive portal-funktionalitet.
- `Preferences.h` bruges til at gemme netværksdata i flash.
- `LittleFS.h` (bruges ikke i denne version, men er inkluderet).

---

## 2. Globale objekter og konstanter 

(linje 10–14)
```cpp
Preferences preferences;
WebServer server(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;
const char* apSSID = "ESP32_Config";
const char* apPassword = "12345678";
```

Disse linjer opretter nødvendige objekter til server, DNS og lagring samt konfigurerer navnet på ESP32’s access point.

---

## 3. setup()-funktion (linje 82–108)

```cpp
Serial.begin(115200);
preferences.begin("wifi", false);
LittleFS.begin(true);
```

Initialiserer seriemonitor, flash-lagring og filsystem.

```cpp
WiFi.mode(WIFI_STA);
String ssid = preferences.getString("ssid", "");
String password = preferences.getString("password", "");
```

ESP32 starter i klient-tilstand (STA) og forsøger at hente gemte credentials.

```cpp
if (ssid != "") {
    WiFi.begin(...);
    ...
}
```

Hvis credentials findes, forsøges forbindelse i op til 10 sekunder.

```cpp
if (WiFi.status() != WL_CONNECTED) {
    startAccessPoint();
} else {
    Serial.println(...);
}
```

Hvis forbindelsen fejler, kaldes `startAccessPoint()`.

---

## 4. startAccessPoint()-funktion (linje 16–54)

```cpp
WiFi.mode(WIFI_AP_STA);
WiFi.softAP(apSSID, apPassword);
```

ESP32 starter som både access point og station (dobbeltmode).

```cpp
dnsServer.start(DNS_PORT, "*", apIP);
```

Alle domæneforespørgsler peges mod ESP32's IP – dette simulerer en captive portal.

```cpp
server.on("/", []() {
  File file = LittleFS.open("/index.html", "r");
  ...
});
```

Returnerer HTML-formularen.

```cpp
server.on("/save", HTTP_POST, []() {
  ...
  preferences.putString(...);
  connectToWiFi();
  ...
});
```

Gemmer brugerens input og forsøger at forbinde til det ønskede WiFi.

---

## 5. connectToWiFi()-funktion (linje 56–80)

```cpp
String ssid = preferences.getString(...);
WiFi.begin(...);
```

Starter WiFi-forbindelse ud fra lagrede data.

```cpp
while (WiFi.status() != WL_CONNECTED && retry < 20)
```

Venter maksimalt 10 sekunder (20 × 500 ms). Hvis forbindelsen lykkes, vises IP’en – ellers beholdes AP aktiv.

---

## 6. loop()-funktion (linje 110–112)

```cpp
dnsServer.processNextRequest();
server.handleClient();
```

Holder DNS og HTTP-servere kørende og responsive.

---

## Konklusion

Denne version fungerer uden SPIFFS eller LittleFS til filhåndtering. HTML-indholdet kan dog stadig leveres som strings, eller – som her – via indlejret filsystem hvis ønsket senere.