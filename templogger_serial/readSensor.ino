boolean gettemperature() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis_sens >= interval_sens) {
    previousMillis_sens = currentMillis;
    
    float humidity_new, temp_c_new;
    humidity_new = dht.readHumidity();
    temp_c_new = dht.readTemperature();
    if (isnan(humidity_new) || isnan(temp_c_new)) {
      Serial.println("Failed to read from DHT sensor!");
      return false;
    }
    temp_c = temp_c_new;
    humidity = humidity_new;
    Serial.println("Temperature: "+String((int)temp_c)+" C");
    Serial.println("Humidity: "+String((int)humidity)+"%");
  }
  return true;
}
