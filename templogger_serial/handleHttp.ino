void handleTemp(){
    gettemperature();
    server.send(200, "text/plain", "Temperature: "+String((int)temp_c)+" C");
  }

void handleHumid(){
  gettemperature();
  server.send(200, "text/plain", "Humidity: "+String((int)humidity)+"%");
}

void handleStatus(){
  server.send(200, "text/html", "<html><head></head><body>Location: "+String(location)+
  "</br>Room: "+String(room)+
  "</br>DC: "+String(dc)+
  "</br>NTP Server: "+String(ntpServer)+
  "</br></body>");
}

void handleroot() {
  server.send(200, "text/html", "<html><head>Temp BUG</head><body></br>Connect DHT22 Temp sensor to GPIO0 (Pin D3)</br><a href='/temp'>Temperature</a></br><a href='/humidity'>Humidity</a></br><a href='/status'>Status</a></br></body>");
  delay(100);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 404, "text/plain", message );
}
