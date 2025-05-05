# Beskrivelse af main.cpp (ESP32 WiFi Portal)

Dette dokument gennemgår `main.cpp` trin for trin og forklarer, hvordan koden fungerer.

## 1. Inkludering og deklarationer

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
```

Disse biblioteker håndterer WiFi-forbindelser, HTTP-server, DNS-omdirigering og lagring af indstillinger i flash (NVS).

```cpp
void connectToWiFi();
void resetWiFiSettings();
void handleReset();
```

Fremadrettede deklarationer af funktioner.

```cpp
Preferences preferences;
WebServer webserver(80);
const byte DNS_PORT = 53;
DNSServer dnsServer;
bool dnsServerActive = false;
```

Opretter instanser og styrer DNS-serverens aktive tilstand.

## 2. Netværksoplysninger og HTML-indhold

```cpp
const char* apSSID = "ESP32_AP_CONFIG_smag";
const char* apPassword = "24.12.2025";
```

Navn og adgangskode til ESP32’s Access Point.

HTML-indhold:
- `htmlForm`: Formular til WiFi-konfiguration
- `htmlLanding`: Visning når ESP32 er forbundet
- `htmlResetConfirm`: Bekræftelsesside efter reset

## 3. HTTP-handlerfunktioner

### handleRoot

Viser opsætningsformularen.

### handleLanding

Viser en statisk HTML-side når enheden er forbundet som klient.

### handleSave

Gemmer SSID og password i flash, forsøger forbindelse, og viser ny IP med auto-redirect.

### handleReset

Sletter credentials og viser HTML-bekræftelse.

## 4. resetWiFiSettings()

Sletter `ssid` og `password` fra NVS.

## 5. startAccessPoint()

- Starter WiFi i dual-mode (AP + STA).
- Starter captive portal DNS.
- Konfigurerer ruter `/`, `/save` og `/reset`.

## 6. startLandingPage()

- Stopper DNS-server.
- Slukker AP.
- Starter webserver med landing page.

## 7. connectToWiFi()

- Henter credentials fra NVS.
- Forbinder til netværk (op til 10 sek.).
- Logger status.

## 8. setup()

- Starter Serial og åbner Preferences.
- Forsøger at forbinde til kendt netværk.
- Hvis mislykket, starter AP-mode.
- Hvis lykkes, starter landing page.

## 9. loop()

- Kører DNS-processering kun hvis aktiv.
- Håndterer HTTP-klienter.