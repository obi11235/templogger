void generateMetrics(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis_dd < interval_dd) {
    return;
  }
  
  previousMillis_dd = currentMillis;
  unsigned long epoch = 0;
  gettemperature();
  int i = 0;
  do{
    epoch = getTime();
    if(i > 240){
      Serial.println("");
      Serial.println("Failed to connect.");
      return;
    }
    i++;
  }while(epoch == 0);
  
  Serial.println(epoch);
  
  sendmetric(String("humidity"), humidity, epoch);
  sendmetric(String("temperature"), temp_c, epoch);
}

void sendmetric(String metric, float value, unsigned int epoch){

  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(":");
  Serial.println(httpsPort);

//  if (client.verify(fingerprint, host)) {
//    Serial.println("certificate matches");
//  } else {
//    Serial.println("certificate doesn't match");
//  }

  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/api/v1/series?api_key="+String(apikey);
//  Serial.print("requesting URL: ");
//  Serial.println(url);

  String data = "{ \"series\" : [{\"metric\":\"environment."+metric+"\",\"points\":[["+String(epoch)+
  ", "+String((int)value)+"]],\"type\":\"gauge\",\"host\":\""+String(dd_host)+"\",\"tags\":["+
  "\"dc:"+String(dc)+"\",\"location:"+String(location)+"\",\"room:"+String(room)+"\",\"ip:"+
  toStringIp( WiFi.localIP())+"\"]}]}";
 
  String request = String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: curl/7.47.0\r\n" +
               "Accept: */*\r\n" +
               "Content-type: application/json\r\n" +
               "Content-Length: "+String((int)data.length())+"\r\n\r\n" +
               data + "\r\n\r\n";

  //Serial.println(request);
  
  client.print(request);

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
//    Serial.print(line);
    if (line == "\r") {
//      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  //Serial.println("reply was:");
  //Serial.println("==========");
  if (line.startsWith("{\"status\": \"ok\"}") && leds_on) {
    digitalWrite(D0, HIGH); // Blink NodeMCU LED to indicate DD success
    delay(300);
    digitalWrite(D0, LOW);
  }
  Serial.println(line);
  //Serial.println("==========");
  Serial.println("closing connection");
}
