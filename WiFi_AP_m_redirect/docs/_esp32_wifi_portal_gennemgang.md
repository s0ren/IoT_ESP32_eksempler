# ESP32 WiFi Setup Portal (uden LittleFS)

Dette projekt demonstrerer hvordan en ESP32 automatisk kan forbinde til kendt WiFi, og hvis det fejler, oprette et Access Point med en webportal til konfiguration.

---

## Funktion

1. **Ved opstart** forsøger ESP32 at forbinde til et gemt WiFi-netværk (via `Preferences.h`).
2. Hvis forbindelsen **fejler**, starter ESP32 sit eget Access Point (`ESP32_Config`).
3. En webserver vises via AP, hvor brugeren kan:
   - Indtaste nyt SSID og password.
   - Se IP på nyt netværk efter forbindelse.
4. ESP32 forbinder til det nye netværk, **uden at lukke AP**.
5. Brugeren får vist IP-adressen og automatisk redirect.

---

## Biblioteker

- `WiFi.h`: WiFi-styring (AP/STA)
- `WebServer.h`: HTTP-server til brugerinteraktion
- `DNSServer.h`: Brugt til captive portal-effekt
- `Preferences.h`: Gemmer SSID/password lokalt

---

## Nøglefunktioner

### WiFi-forbindelse

```cpp
WiFi.begin(ssid.c_str(), password.c_str());
while (WiFi.status() != WL_CONNECTED) { ... }
```

### Start AP + Webserver

```cpp
WiFi.mode(WIFI_AP_STA);
WiFi.softAP(apSSID, apPassword);
server.on("/", []() { server.send(...); });
```

### DNS Captive Portal

```cpp
dnsServer.start(53, "*", WiFi.softAPIP());
```

### HTML Form

Indlejret HTML med `form` til at modtage SSID/password.

### Redirect med IP

```html
<script>
  setTimeout(() => { window.location.href = "http://[IP]"; }, 7000);
</script>
```

---

## Filstruktur (uden LittleFS)

- `main.cpp` – alt indlejret i koden
- HTML er defineret som C++-strings
- Webserver returnerer HTML direkte med `server.send(...)`

---

## Tips

- Brug `Serial.println(WiFi.localIP());` til debug
- Test på mobil/laptop og se om captive portal-popup vises
- Omdirigering kræver ofte brugeren skifter netværk manuelt

---

## Udvidelser (forslag)

- QR-kode med SSID og password
- LittleFS til ekstern HTML
- OTA firmware-opdatering
- Loginbeskyttelse af webinterface