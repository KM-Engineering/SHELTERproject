#include <DHTesp.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <Variables.h>
#include <SensorModule.h>
#include <Watering.h>
#include <OutputData.h>

const char* ssid = "Kyle's Wifi";
const char* password = "PLDTWIFI7VsT9";

WebServer server(80);

float airTempHistory[20] = {0};
float airRHHistory[20] = {0};
float soilRH1History[20] = {0};
float soilRH2History[20] = {0};
float soilRH3History[20] = {0};
float soilRH4History[20] = {0};

void updateHistory(float* history, int size, float newValue) {
  for (int i = size - 1; i > 0; i--) {
    history[i] = history[i - 1];
  }
  history[0] = newValue;
}

void handleRoot() {
  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>The SHELTER Project</title>
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
      <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;500&display=swap" rel="stylesheet">
      <style>
          body {
              font-family: 'Roboto', sans-serif;
              text-align: center;
              background-color: #f9f9f9;
              color: #333;
              padding: 20px;
              margin: 0;
              transition: background-color 0.3s, color 0.3s;
          }
          .container {
              display: grid;
              grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
              gap: 20px;
              max-width: 1200px;
              margin: 60px auto 20px;
              padding: 20px;
          }
          h1 {
              grid-column: 1 / -1;
              color: #2196F3;
              font-size: 24px;
              margin: 0;
          }
          .card {
              background: white;
              padding: 20px;
              border-radius: 10px;
              box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1);
              cursor: pointer;
              transition: transform 0.3s ease, box-shadow 0.3s ease;
          }
          .card:hover {
              transform: translateY(-5px);
              box-shadow: 0px 10px 20px rgba(0, 0, 0, 0.2);
          }
          .card i {
              font-size: 24px;
              margin-bottom: 10px;
              color: #2196F3;
          }
          .card h3 {
              margin: 0;
              font-size: 18px;
              color: #333;
          }
          .card p {
              margin: 10px 0;
              font-size: 24px;
              font-weight: bold;
          }
          .progress-bar {
              width: 100%;
              height: 10px;
              background-color: #e0e0e0;
              border-radius: 5px;
              margin-top: 10px;
          }
          .progress {
              height: 100%;
              border-radius: 5px;
              background-color: #4caf50;
              transition: width 0.5s ease;
          }
          .card.warning .progress {
              background-color: #ffeb3b;
          }
          .card.critical .progress {
              background-color: #f44336;
          }
          .modal {
              display: none;
              position: fixed;
              z-index: 1;
              left: 0;
              top: 0;
              width: 100%;
              height: 100%;
              background-color: rgba(0, 0, 0, 0.5);
              animation: fadeIn 0.3s ease;
          }
          .modal-content {
              background-color: white;
              margin: 5% auto;
              padding: 20px;
              border-radius: 10px;
              width: 90%;
              max-width: 800px;
              height: 85vh;
              box-shadow: 0px 4px 20px rgba(0, 0, 0, 0.2);
              position: relative;
          }
          .close {
              float: right;
              font-size: 28px;
              font-weight: bold;
              cursor: pointer;
              color: #333;
          }
          .close:hover {
              color: #000;
          }
          .chart-container {
              position: relative;
              width: 100%;
              height: 70vh;
          }
          @keyframes fadeIn {
              from { opacity: 0; }
              to { opacity: 1; }
          }
          .button-container {
              position: absolute;
              top: 20px;
              right: 20px;
              display: flex;
              gap: 10px;
          }
          .button-container button {
              padding: 10px 15px;
              border: none;
              border-radius: 5px;
              background-color: #2196F3;
              color: white;
              cursor: pointer;
              transition: background-color 0.3s ease;
          }
          .button-container button:hover {
              background-color: #1976D2;
          }
          .dark-mode {
              background-color: #121212;
              color: #ffffff;
          }
          .dark-mode .card {
              background-color: #1e1e1e;
              color: #ffffff;
          }
          .dark-mode .card h3 {
              color: #ffffff;
          }
          .dark-mode .modal-content {
              background-color: #333;
              color: #ffffff;
          }
          .dark-mode .close {
              color: #ffffff;
          }
          .dark-mode .close:hover {
              color: #ddd;
          }
          @media (max-width: 600px) {
              .container {
                  grid-template-columns: 1fr;
                  margin-top: 80px;
              }
              .button-container {
                  top: 10px;
                  right: 10px;
              }
              .modal-content {
                  height: 90vh;
              }
              .chart-container {
                  height: 60vh;
              }
          }
      </style>
  </head>
  <body>
      <div class="button-container">
          <button onclick="toggleDarkMode()">Toggle Dark Mode</button>
          <button onclick="updateData()">Refresh Data</button>
      </div>
      <div class="container">
          <h1>The SHELTER Project</h1>
          <div class="card" onclick="showHistory('airTemp', 'Air Temperature History')">
              <i class="fas fa-thermometer-half"></i>
              <h3>Air Temperature</h3>
              <p><span id="airTemp">0</span>°C</p>
              <div class="progress-bar">
                  <div id="airTempBar" class="progress" style="width: 0%;"></div>
              </div>
          </div>
          <div class="card" onclick="showHistory('airRH', 'Air Humidity History')">
              <i class="fas fa-tint"></i>
              <h3>Air Humidity</h3>
              <p><span id="airRH">0</span>%</p>
              <div class="progress-bar">
                  <div id="airRHBar" class="progress" style="width: 0%;"></div>
              </div>
          </div>
          <div class="card" onclick="showHistory('soilRH1', 'Soil RH1 History')">
              <i class="fas fa-seedling"></i>
              <h3>Soil RH1</h3>
              <p><span id="soilRH1">0</span>%</p>
              <div class="progress-bar">
                  <div id="soilRH1Bar" class="progress" style="width: 0%;"></div>
              </div>
          </div>
          <div class="card" onclick="showHistory('soilRH2', 'Soil RH2 History')">
              <i class="fas fa-seedling"></i>
              <h3>Soil RH2</h3>
              <p><span id="soilRH2">0</span>%</p>
              <div class="progress-bar">
                  <div id="soilRH2Bar" class="progress" style="width: 0%;"></div>
              </div>
          </div>
          <div class="card" onclick="showHistory('soilRH3', 'Soil RH3 History')">
              <i class="fas fa-seedling"></i>
              <h3>Soil RH3</h3>
              <p><span id="soilRH3">0</span>%</p>
              <div class="progress-bar">
                  <div id="soilRH3Bar" class="progress" style="width: 0%;"></div>
              </div>
          </div>
          <div class="card" onclick="showHistory('soilRH4', 'Soil RH4 History')">
              <i class="fas fa-seedling"></i>
              <h3>Soil RH4</h3>
              <p><span id="soilRH4">0</span>%</p>
              <div class="progress-bar">
                  <div id="soilRH4Bar" class="progress" style="width: 0%;"></div>
              </div>
          </div>
      </div>

      <div id="historyModal" class="modal" onclick="closeModalOnOutsideClick(event)">
          <div class="modal-content" onclick="event.stopPropagation()">
              <span class="close" onclick="closeModal()">&times;</span>
              <h2 id="modalTitle"></h2>
              <div class="chart-container">
                  <canvas id="historyChart"></canvas>
              </div>
          </div>
      </div>

      <footer>
          <p>Last updated: <span id="lastUpdated">Loading...</span></p>
          <p>Project by me</p>
      </footer>

      <script>
          let sensorData = {};
          let liveUpdateInterval = null;
          let historyChart = null;

          function updateData() {
              fetch('/data')
              .then(response => response.json())
              .then(data => {
                  sensorData = data;
                  updateCard('airTemp', data.airTemp, 50);
                  updateCard('airRH', data.airRH, 100);
                  updateCard('soilRH1', data.soilRH1, 100);
                  updateCard('soilRH2', data.soilRH2, 100);
                  updateCard('soilRH3', data.soilRH3, 100);
                  updateCard('soilRH4', data.soilRH4, 100);
                  document.getElementById('lastUpdated').innerText = new Date().toLocaleTimeString();

                  if (document.getElementById('historyModal').style.display === 'block') {
                      const type = document.getElementById('modalTitle').innerText.replace(' History', '');
                      const title = document.getElementById('modalTitle').innerText;
                      updateChart(type, title);
                  }
              })
              .catch(error => console.error('Error fetching data:', error));
          }

          function updateCard(type, value, max) {
              const card = document.querySelector(`.card[onclick*="${type}"]`);
              const progressBar = card.querySelector('.progress');
              const valueElement = card.querySelector('p span');

              valueElement.innerText = value;
              progressBar.style.width = `${(value / max) * 100}%`;

              // Visual feedback
              if (type === 'airTemp') {
                  if (value < 15 || value > 30) card.classList.add('critical');
                  else if (value < 18 || value > 25) card.classList.add('warning');
                  else card.classList.remove('warning', 'critical');
              } else {
                  if (value < 20 || value > 80) card.classList.add('critical');
                  else if (value < 30 || value > 70) card.classList.add('warning');
                  else card.classList.remove('warning', 'critical');
              }
          }

          function showHistory(type, title) {
              document.getElementById('modalTitle').innerText = title; // Fixed: No extra "History" appended
              document.getElementById('historyModal').style.display = 'block';
              updateChart(type, title);
              liveUpdateInterval = setInterval(() => updateChart(type, title), 15000);
          }

          function updateChart(type, title) {
              const historyKey = type + 'History';
              const history = sensorData[historyKey] || [];

              if (!historyChart) {
                  const ctx = document.getElementById('historyChart').getContext('2d');
                  historyChart = new Chart(ctx, {
                      type: 'line',
                      data: {
                          labels: Array.from({ length: history.length }, (_, i) => `Point ${i + 1}`),
                          datasets: [{
                              label: title, // Use the correct title
                              data: history,
                              borderColor: '#2196F3',
                              fill: false,
                              pointRadius: 5,
                              pointHoverRadius: 8,
                              pointBackgroundColor: '#2196F3',
                              pointBorderColor: '#ffffff',
                              pointBorderWidth: 2,
                          }]
                      },
                      options: {
                          responsive: true,
                          maintainAspectRatio: false,
                          animation: {
                              duration: 500,
                              easing: 'easeOutQuart',
                          },
                          interaction: {
                              mode: 'nearest',
                              intersect: false,
                          },
                          plugins: {
                              tooltip: {
                                  enabled: true,
                              }
                          },
                          scales: {
                              y: {
                                  beginAtZero: true,
                              }
                          }
                      }
                  });
              } else {
                  historyChart.data.labels = Array.from({ length: history.length }, (_, i) => `Point ${i + 1}`);
                  historyChart.data.datasets[0].data = history;
                  historyChart.data.datasets[0].label = title; // Update the title
                  historyChart.update();
              }
          }

          function closeModal() {
              document.getElementById('historyModal').style.display = 'none';
              if (liveUpdateInterval) {
                  clearInterval(liveUpdateInterval);
                  liveUpdateInterval = null;
              }
          }

          function closeModalOnOutsideClick(event) {
              if (event.target === document.getElementById('historyModal')) {
                  closeModal();
              }
          }

          function toggleDarkMode() {
              document.body.classList.toggle('dark-mode');
          }

          setInterval(updateData, 15000);
      </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html; charset=UTF-8", page);
}

void handleData() {
  float airTemp = random(200, 350) / 10.0;
  float airRH = random(30, 70);
  float soilRH1 = random(20, 60);
  float soilRH2 = random(20, 60);
  float soilRH3 = random(20, 60);
  float soilRH4 = random(20, 60);

  updateHistory(airTempHistory, 20, airTemp);
  updateHistory(airRHHistory, 20, airRH);
  updateHistory(soilRH1History, 20, soilRH1);
  updateHistory(soilRH2History, 20, soilRH2);
  updateHistory(soilRH3History, 20, soilRH3);
  updateHistory(soilRH4History, 20, soilRH4);

  DynamicJsonDocument jsonDoc(2048);

  jsonDoc["airTemp"] = airTemp;
  jsonDoc["airRH"] = airRH;
  jsonDoc["soilRH1"] = soilRH1;
  jsonDoc["soilRH2"] = soilRH2;
  jsonDoc["soilRH3"] = soilRH3;
  jsonDoc["soilRH4"] = soilRH4;

  JsonArray airTempHist = jsonDoc.createNestedArray("airTempHistory");
  JsonArray airRHHist = jsonDoc.createNestedArray("airRHHistory");
  JsonArray soilRH1Hist = jsonDoc.createNestedArray("soilRH1History");
  JsonArray soilRH2Hist = jsonDoc.createNestedArray("soilRH2History");
  JsonArray soilRH3Hist = jsonDoc.createNestedArray("soilRH3History");
  JsonArray soilRH4Hist = jsonDoc.createNestedArray("soilRH4History");

  for (int i = 0; i < 20; i++) {
    airTempHist.add(airTempHistory[i]);
    airRHHist.add(airRHHistory[i]);
    soilRH1Hist.add(soilRH1History[i]);
    soilRH2Hist.add(soilRH2History[i]);
    soilRH3Hist.add(soilRH3History[i]);
    soilRH4Hist.add(soilRH4History[i]);
  }

  String json;
  serializeJson(jsonDoc, json);

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);

  dht.setup(dhtPin, DHTesp::DHT22);
  readSoilSensor();  // Initial ping to stabilize hygrometers

  delay(10000);

  Serial.println("STARTUP FINISHED");
  Serial.println();
  Serial.println("--------------------------------------------------------------------------------");

  // Connect to WiFi
  int wifiRetryCount = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && wifiRetryCount < 10) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    wifiRetryCount++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi. Restarting...");
    ESP.restart();
  }

  Serial.println("Connected! IP address: " + WiFi.localIP().toString());
  Serial.println("--------------------------------------------------------------------------------");

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
//  unsigned long currentMillis = millis();

  // DHT22 timer
//  if (currentMillis - previousDHTSensorTime >= DHT_SENSOR_INTERVAL) {
//    readDHTSensor();
//    previousDHTSensorTime = currentMillis;
//  }

  // Soil moisture sensor timer
//  if (currentMillis - previousSoilSensorTime >= SOIL_SENSOR_INTERVAL) {
//    readSoilSensor();
//    previousSoilSensorTime = currentMillis;
//  }

  // Output data timer
 // if (currentMillis - previousOutputDataTime >= OUTPUT_DATA_INTERVAL) {
 //   outputData();
 //   previousOutputDataTime = currentMillis;
 // }

  // Watering timer
 // if (currentMillis - previousWaterPumpCheckTime >= WATER_PUMP_INTERVAL) {
 //   previousWaterPumpCheckTime = currentMillis;
 //   triggerWatering();
//  }
}