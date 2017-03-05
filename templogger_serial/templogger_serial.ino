#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN  0
 
char ssid[32] = "";
char password[32] = "";

char apikey[34] = "";
char dd_host[52] = "";
char location[32] = "";
char room[32] = "";
char dc[4] = "";
char ntpServer[32] = "";
 
// Initialize DHT sensor 
DHT dht(DHTPIN, DHTTYPE);

float humidity, temp_c;

unsigned long previousMillis_sens = 0;
unsigned long previousMillis_dd = 0;
const long interval_sens = 4000;
const long interval_dd = 60000;

const char* host = "app.datadoghq.com";
const int httpsPort = 443;

boolean connect;
long lastConnectTry = 0;
int status = WL_IDLE_STATUS;

ESP8266WebServer server(80);

void setup(void)
{
  Serial.begin(115200);
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
  Serial.println("Connect DHT22 Temp sensor to GPIO0 (Pin D3)");

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
  if (status == WL_CONNECTED && strlen(apikey) > 0){
    generateMetrics();
  }
}

void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(i%20 == 19){Serial.println("");}
    
    i++;
    if(i > 240){
      Serial.println("");
      Serial.println("Failed to connect.");
      WiFi.disconnect();
      return;
    }
  }

  status = WiFi.status();
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
