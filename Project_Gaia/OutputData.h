void outputData() {

  unsigned long currentMillis = millis();


  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;


  unsigned long displaySeconds = seconds % 60;
  unsigned long displayMinutes = minutes % 60; 
  unsigned long displayHours = hours % 24;

  Serial.println();
  
  Serial.print("Timestamp: ");
  if (displayHours < 10) Serial.print("0"); 
  Serial.print(displayHours);
  Serial.print(":");
  if (displayMinutes < 10) Serial.print("0"); 
  Serial.print(displayMinutes);
  Serial.print(":");
  if (displaySeconds < 10) Serial.print("0");
  Serial.print(displaySeconds);
  Serial.println();

  // DHT
  Serial.print("Corrected Humidity: ");
  Serial.print(correctedHumidity);
  Serial.print("%  Corrected Temperature: ");
  Serial.print(correctedTemperature);
  Serial.println("°C");

  // S.H
  Serial.print("Hygrometer 1: ");
  Serial.print(soilMoisturePercent1);
  Serial.print("%\t");

  Serial.print("Hygrometer 2: ");
  Serial.print(soilMoisturePercent2);
  Serial.print("%\t");

  Serial.print("Hygrometer 3: ");
  Serial.print(soilMoisturePercent3);
  Serial.print("%\t");

  Serial.print("Hygrometer 4: ");
  Serial.print(soilMoisturePercent4);
  Serial.println("%");
}
