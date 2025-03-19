DHTesp dht;
int dhtPin = 32;

int Hygrometer1 = 33;
int Hygrometer2 = 25;
int Hygrometer3 = 26;
int Hygrometer4 = 27;

int Relay1 = 4;
int Relay2 = 16;
int Relay3 = 17;
int Relay4 = 5;

const uint16_t AIR_SENSOR_VALUE = 3170;    // RECALIBRATE FOR V3 PLZ -Kyle from 6/8/24
const uint16_t WATER_SENSOR_VALUE = 2200;  // same for this one
const uint16_t MIN_SOIL_MOISTURE_THRESHOLD = 35;
const uint16_t MAX_SOIL_MOISTURE_THRESHOLD = 42;
const uint16_t MIN_AIR_HUMIDITY_THRESHOLD = 30;
const uint16_t MAX_AIR_HUMIDITY_THRESHOLD = 75;
const uint16_t MIN_AIR_TEMPERATURE_THRESHOLD = 21;
const uint16_t MAX_AIR_TEMPERATURE_THRESHOLD = 35;

const uint16_t ABSOLUTE_MAX_SOIL_MOISTURE_THRESHOLD = 50;
const uint16_t ABSOLUTE_MIN_SOIL_MOISTURE_THRESHOLD = 30;

// Timing adjustment section
const uint32_t WATER_PUMP_INTERVAL = 30000;
const uint32_t WATER_PUMP_PULSE_DURATION = 1500;
const uint32_t ESP32_RESET_INTERVAL = 604800000;
const unsigned long SOIL_SENSOR_INTERVAL = 30000;
const unsigned long DHT_SENSOR_INTERVAL = 30000;
const unsigned long OUTPUT_DATA_INTERVAL = 30000;

// Variable storage
unsigned long previousEsp32RestartTime = 0;
unsigned long previousSoilSensorTime = 0;
unsigned long previousDHTSensorTime = 0;
unsigned long previousOutputDataTime = 0;

float correctedHumidity = 0;
float correctedTemperature = 0;

int soilMoistureValue1 = 0;
int soilMoisturePercent1 = 0;
int soilMoistureValue2 = 0;
int soilMoisturePercent2 = 0;
int soilMoistureValue3 = 0;
int soilMoisturePercent3 = 0;
int soilMoistureValue4 = 0;
int soilMoisturePercent4 = 0;

unsigned long waterPumpPulseStartTime1 = 0;
unsigned long waterPumpPulseStartTime2 = 0;
unsigned long waterPumpPulseStartTime3 = 0;
unsigned long waterPumpPulseStartTime4 = 0;

float previousAirTemperature = 0;
float previousAirHumidity = 0;
float previousWaterPumpCheckTime = 0;
int previousSoilMoisturePercent = 0;


// Declare modules
void readSoilSensor();
void readDHTSensor();
void outputData();
void updateAirTemperature();
void updateAirHumidity();
void updateSoilMoisture();
void triggerWatering();