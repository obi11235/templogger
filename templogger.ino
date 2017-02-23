
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN  0
 
const char* ssid     = "";
const char* password = "";
const char* apiKey = "";
const char* dd_host = "";
const char* dd_tags = "\"dc:nyc\",\"location:front_of_rack\",\"room:server_room\"";


ESP8266WebServer server(80);
 
// Initialize DHT sensor 
DHT dht(DHTPIN, DHTTYPE);
 
float humidity, temp_c;
String webString="";

unsigned long previousMillis_sens = 0;
unsigned long previousMillis_dd = 0;
const long interval_sens = 4000;
const long interval_dd = 60000;

const char* host = "app.datadoghq.com";
const int httpsPort = 443;

void handle_root() {
  server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
  delay(100);
}
 
void setup(void)
{
  Serial.begin(115200);
  dht.begin();
 
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", handle_root);
  
  server.on("/temp", [](){
    gettemperature();
    webString="Temperature: "+String((int)temp_c)+" F";
    server.send(200, "text/plain", webString);
  });
 
  server.on("/humidity", [](){
    gettemperature();
    webString="Humidity: "+String((int)humidity)+"%";
    server.send(200, "text/plain", webString);
  });
  
  server.begin();
  Serial.println("HTTP server started");

  setupNTP();
}
 
void loop(void)
{
  server.handleClient();
  generateMetrics();
}

void generateMetrics(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis_dd < interval_dd) {
    return;
  }
  
  previousMillis_dd = currentMillis;
  unsigned long epoch = 0;
  gettemperature();
  do{
    epoch = getTime();
  }while(epoch == 0);
  
  Serial.println(epoch);
  String data = "{ \"series\" : [{\"metric\":\"environment.temperature\",\"points\":[["+String(epoch)+
  ", "+String((int)temp_c)+"]],\"type\":\"gauge\",\"host\":\""+String(dd_host)+"\",\"tags\":["+
  String(dd_tags)+",\"ip:"+String(WiFi.localIP())+"\"]}]}";
  
  sendmetric(data);
  
  data = "{ \"series\" : [{\"metric\":\"environment.humidity\",\"points\":[["+
  String(epoch)+
  ", "+String((int)humidity)+"]],\"type\":\"gauge\",\"host\":\""+
  String(dd_host)+"\",\"tags\":["+String(dd_tags)+",\"ip:"+String(WiFi.localIP())+"\"]}]}";
  
  sendmetric(data);
}

void sendmetric(String &data){

  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
//
//  if (client.verify(fingerprint, host)) {
//    Serial.println("certificate matches");
//  } else {
//    Serial.println("certificate doesn't match");
//  }

  String url = "/api/v1/series?api_key="+String(apiKey);
  Serial.print("requesting URL: ");
  Serial.println(url);
 
  String request = String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: curl/7.47.0\r\n" +
               "Accept: */*\r\n" +
               "Content-type: application/json\r\n" +
               "Content-Length: "+String((int)data.length())+"\r\n\r\n" +
               data + "\r\n\r\n";

  Serial.println(request);
  
  client.print(request);

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.print(line);
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void gettemperature() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis_sens >= interval_sens) {
    previousMillis_sens = currentMillis;
    
    float humidity_new, temp_c_new;
    humidity_new = dht.readHumidity();
    temp_c_new = dht.readTemperature();
    if (isnan(humidity_new) || isnan(temp_c_new)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    temp_c = temp_c_new;
    humidity = humidity_new;
    Serial.println("Temperature: "+String((int)temp_c)+" C");
    Serial.println("Humidity: "+String((int)humidity)+"%");
  }
}
