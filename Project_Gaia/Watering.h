void triggerWatering() {

Serial.println();
  
  if (soilMoisturePercent1 <= MIN_SOIL_MOISTURE_THRESHOLD) {
    waterPumpPulseStartTime1 = millis();
    digitalWrite(Relay1, HIGH); 
    Serial.println("Relay 1 ON (Watering plant 1)");
    delay(WATER_PUMP_PULSE_DURATION);  
    digitalWrite(Relay1, LOW);   
  } else {
    digitalWrite(Relay1, LOW);  
    Serial.println("Relay 1 OFF (No watering needed for plant 1)");
  }

  delay(500); 

  if (soilMoisturePercent2 <= MIN_SOIL_MOISTURE_THRESHOLD) {
    waterPumpPulseStartTime2 = millis();
    digitalWrite(Relay2, HIGH); 
    Serial.println("Relay 2 ON (Watering plant 2)");
    delay(WATER_PUMP_PULSE_DURATION); 
    digitalWrite(Relay2, LOW); 
  } else {
    digitalWrite(Relay2, LOW);  
    Serial.println("Relay 2 OFF (No watering needed for plant 2)");
  }

  delay(500);  

  if (soilMoisturePercent3 <= MIN_SOIL_MOISTURE_THRESHOLD) {
    waterPumpPulseStartTime3 = millis();
    digitalWrite(Relay3, HIGH);  
    Serial.println("Relay 3 ON (Watering plant 3)");
    delay(WATER_PUMP_PULSE_DURATION);  
    digitalWrite(Relay3, LOW);  
  } else {
    digitalWrite(Relay3, LOW); 
    Serial.println("Relay 3 OFF (No watering needed for plant 3)");
  }

  delay(500);  

  if (soilMoisturePercent4 <= MIN_SOIL_MOISTURE_THRESHOLD) {
    waterPumpPulseStartTime4 = millis();
    digitalWrite(Relay4, HIGH);  
    Serial.println("Relay 4 ON (Watering plant 4)");
    delay(WATER_PUMP_PULSE_DURATION);  
    digitalWrite(Relay4, LOW);   
  } else {
    digitalWrite(Relay4, LOW);
    Serial.println("Relay 4 OFF (No watering needed for plant 4)");
  }

Serial.println();
Serial.println("--------------------------------------------------------------------------------");

}