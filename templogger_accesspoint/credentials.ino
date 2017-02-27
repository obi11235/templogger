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
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(strlen(password)>0?"********":"<no password>");
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
  char ok[2+1] = "OK";
  EEPROM.put(pos, ok);
  EEPROM.commit();
  EEPROM.end();
}
