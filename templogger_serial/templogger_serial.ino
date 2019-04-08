#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

#define DHTTYPE DHT11
#define DHTPIN  5

char ssid[32] = "";
char password[64] = "";

char apikey[34] = "";
char dd_host[52] = "";
char location[32] = "";
char room[32] = "";
char dc[32] = "";
char ntpServer[32] = "";


// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

float humidity, temp_c;

unsigned long previousMillis_sens = 0;
unsigned long previousMillis_dd = 0;
const long interval_sens = 4000;
const long interval_dd = 60000;

const char* host = "api.datadoghq.com";
// *.datadoghq.com TLS Cert - SHA1 Fingerprint as of 20190408
const char* fingerprint = "10 E0 C4 4F 85 A8 58 BB CB C2 C6 D6 0E 60 59 83 C1 7D 0F EB";
const int httpsPort = 443;

// LED diagnostics for program operation and Wifi connection
boolean leds_on = true;

boolean connect;
long lastConnectTry = 0;
int status = WL_IDLE_STATUS;

ESP8266WebServer server(80);

void setup(void)
{
  // ESP-12 LED for Wifi indicator and NodeMCU LED for program/DD success
  // The pin assignments are not directly connected to GPIO and should work across any NodeMCU board
  if (leds_on) {
    pinMode(D4, OUTPUT); // Initialize D4 as an output for ESP-12 LED
    pinMode(D0, OUTPUT); // Initialize D0 as an output for NodeMCU LED
    digitalWrite(D0, LOW); // Turn on NodeMCU LED to indicate program is running
  }

  Serial.begin(74880);
  Serial.println("");
  loadCredentials(); // Load WLAN credentials from network

  readConfig();

  server.on("/", handleroot);
  server.on("/status", handleStatus);
  server.on("/temp", handleTemp);
  server.on("/humidity", handleHumid);
  server.onNotFound ( handleNotFound );

  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Connect DHT11 Temp sensor to GPIO5 (Pin D1)");

  connect = strlen(ssid) > 0;

  dht.begin();
  setupNTP();
}

void loop(void)
{
  if (connect) {
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  status = WiFi.status();
  if (status != WL_CONNECTED && millis() > (lastConnectTry + 60000) ) {
    connect = true;
  }

  //Do Work
  server.handleClient();
  if (status == WL_CONNECTED && strlen(apikey) > 0) {
    generateMetrics();
  }
}

void connectWifi() {
  if (leds_on) {
    digitalWrite(D4, HIGH); // Turn off ESP-12 LED to indicate no Wifi connection
  }
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i % 20 == 19) {
      Serial.println("");
    }

    i++;
    if (i > 240) {
      Serial.println("");
      Serial.println("Failed to connect.");
      WiFi.disconnect();
      return;
    }
  }

  status = WiFi.status();
  if (leds_on) {
    digitalWrite(D4, LOW); // Turn on ESP-12 LED to indicate Wifi is connected
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("DNS #1, #2 IP: ");
  WiFi.dnsIP().printTo(Serial);
  Serial.print(", ");
  WiFi.dnsIP(1).printTo(Serial);
  Serial.println();
}
