/** Load WLAN credentials from EEPROM */
void loadCredentials() {
  EEPROM.begin(512);
  int pos = 0;
  EEPROM.get(pos, ssid);
  pos = pos + sizeof(ssid);
  EEPROM.get(pos, password);
  pos = pos + sizeof(password);
  EEPROM.get(pos, apikey);
  pos = pos + sizeof(apikey);
  EEPROM.get(pos, dd_host);
  pos = pos + sizeof(dd_host);
  EEPROM.get(pos, room);
  pos = pos + sizeof(room);
  EEPROM.get(pos, location);
  pos = pos + sizeof(location);
  EEPROM.get(pos, dc);
  pos = pos + sizeof(dc);
  EEPROM.get(pos, ntpServer);
  pos = pos + sizeof(ntpServer);
  char ok[2+1];
  EEPROM.get(pos, ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
    apikey[0] = 0;
    room[0] = 0;
    location[0] = 0;
    dd_host[0] = 0;
    dc[0] = 0;
    ntpServer[0] = 0;
  }
  Serial.println("Loaded Config from EEPROM:");
  Serial.println(String("SSID: ")+ssid);
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  EEPROM.begin(512);
  int pos = 0;
  EEPROM.put(pos, ssid);
  pos = pos + sizeof(ssid);
  EEPROM.put(pos, password);
  pos = pos + sizeof(password);
  EEPROM.put(pos, apikey);
  pos = pos + sizeof(apikey);
  EEPROM.put(pos, dd_host);
  pos = pos + sizeof(dd_host);
  EEPROM.put(pos, room);
  pos = pos + sizeof(room);
  EEPROM.put(pos, location);
  pos = pos + sizeof(location);
  EEPROM.put(pos, dc);
  pos = pos + sizeof(dc);
  EEPROM.put(pos, ntpServer);
  pos = pos + sizeof(ntpServer);
  char ok[2+1] = "OK";
  EEPROM.put(pos, ok);
  EEPROM.commit();
  EEPROM.end();
}

void readConfig(){
  Serial.setTimeout(10000);
  char input[2];
  
  Serial.println("Enter Y to configure");
  Serial.readBytesUntil('\n', input, 2);

  if(input[0] == 'Y' || input[0] == 'y'){
    Serial.setTimeout(30000);
    Serial.println("Start Configure Mode (blank to leave unchanged)");
    char buffer[64];
    int read;

    Serial.println(ssid);
    Serial.print("Enter SSID: ");
    read = Serial.readBytesUntil('\n', buffer, 32);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(ssid, buffer, read+1);
    }
    Serial.println("");

    Serial.println(password);
    Serial.print("Enter SSID Password: ");
    read = Serial.readBytesUntil('\n', buffer, 32);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(password, buffer, read+1);
    }
    Serial.println("");

    Serial.println(ntpServer);
    Serial.print("Enter NTP Server Hostname (defaults to 'time.nist.gov'): ");
    read = Serial.readBytesUntil('\n', buffer, 32);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(ntpServer, buffer, read+1);
    } else {
      strncpy(ntpServer, "time.nist.gov", 13);
      ntpServer[13] = '\0';
    }
    Serial.println("");

    Serial.println(apikey);
    Serial.print("Enter Datadog API Key: ");
    read = Serial.readBytesUntil('\n', buffer, 34);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(apikey, buffer, read+1);
    }
    Serial.println("");

    Serial.println(dd_host);
    Serial.print("Enter host for datadog: ");
    read = Serial.readBytesUntil('\n', buffer, 52);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(dd_host, buffer, read+1);
    }
    Serial.println("");
    
    Serial.println(location);
    Serial.print("Enter location: ");
    read = Serial.readBytesUntil('\n', buffer, 32);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(location, buffer, read+1);
    }
    Serial.println("");

    Serial.println(room);
    Serial.print("Enter room: ");
    read = Serial.readBytesUntil('\n', buffer, 32);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(room, buffer, read+1);
    }
    Serial.println("");

    Serial.println(dc);
    Serial.print("Enter dc: ");
    read = Serial.readBytesUntil('\n', buffer, 4);
    if(read > 0){
      buffer[read] = '\0';
      Serial.println(buffer);
      strncpy(dc, buffer, read+1);
    }
    Serial.println("");

    Serial.println("Writing to EEPROM");
    saveCredentials();
  }
  
  Serial.setTimeout(1000);
}

