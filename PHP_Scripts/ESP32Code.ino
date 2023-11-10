#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "Filtros.h"

#define Sensor_do_Reservatorio 34
#define SensorSolo 12
#define DHTPIN 4  // Define a porta onde o DHT11 está conectado (pino GPIO4)
#define SensorLigar 27

long unsigned timeLeituras = 0;
bool reservatorioVazio = false;

// Replace with your network credentials
const char* ssid = "POCO F4";
const char* password = "123345678";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://gardeningprojectteste.000webhostapp.com/teste/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorLocation = "Sala";

DHT dht(DHTPIN, DHT11);
FiltroRecursivo FiltroSolo(0.5f);
FiltroRecursivo FiltroReservatorio(0.5f);

void setup() {
  Serial.begin(115200);
  pinMode(Sensor_do_Reservatorio, INPUT);  //Input do sensor de chuva
  pinMode(SensorSolo, INPUT);              //Input do sensor de solo
  pinMode(SensorLigar, OUTPUT);
  digitalWrite(SensorLigar, HIGH);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado ao Wifi com o endereco de IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  for (int i = 0; i < 10; i++) {
    FiltroReservatorio(analogRead(Sensor_do_Reservatorio));
  }
  reservatorioVazio = FiltroReservatorio(analogRead(Sensor_do_Reservatorio)) > 2500;
}

void loop() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED && millis() - timeLeituras > 30000) {
    timeLeituras = millis();
    WiFiClientSecure* client = new WiFiClientSecure;
    client->setInsecure();  //don't use SSL certificate
    HTTPClient https;

    // Your Domain name with URL path or IP address with path
    https.begin(*client, serverName);

    // Specify content-type header
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Read temperature and humidity from DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println(F("Falha ao ler o sensor DHT!"));
    } else {
      // Compute heat index in Celsius (isFahrenheit = false)
      float hic = dht.computeHeatIndex(temperature, humidity, false);
      float reservatorio = FiltroReservatorio(analogRead(Sensor_do_Reservatorio));
      float solo = FiltroSolo(analogRead(SensorSolo));

      // Prepare your HTTP POST request data for SensorData
      String dataSensorData = "api_key=" + apiKeyValue + "&localizacao=" + sensorLocation + "&umidade=" + String(humidity) +
                             "&temperatura=" + String(temperature) + "&indice_calor=" + String(hic) + "&SensorSolo=" + String(solo);

      // Prepare your HTTP POST request data for SensorReservatorio
      String dataSensorReservatorio = "api_key=" + apiKeyValue + "&EstadoBomba=" + String(reservatorioVazio) + "&SituacaoReservatorio=" + "YourValueHere";

      // Send HTTP POST request for SensorData
      int httpResponseCodeSensorData = https.POST(dataSensorData);

      if (httpResponseCodeSensorData > 0) {
        Serial.print("HTTP Response code for SensorData: ");
        Serial.println(httpResponseCodeSensorData);
      } else {
        Serial.print("Error code for SensorData: ");
        Serial.println(httpResponseCodeSensorData);
        Serial.println(https.getString()); // Adicione esta linha para obter informações adicionais de depuração
      }

      // Send HTTP POST request for SensorReservatorio
      int httpResponseCodeSensorReservatorio = https.POST(dataSensorReservatorio);

      if (httpResponseCodeSensorReservatorio > 0) {
        Serial.print("HTTP Response code for SensorReservatorio: ");
        Serial.println(httpResponseCodeSensorReservatorio);
      } else {
        Serial.print("Error code for SensorReservatorio: ");
        Serial.println(httpResponseCodeSensorReservatorio);
        Serial.println(https.getString()); // Adicione esta linha para obter informações adicionais de depuração
      }

      // Free resources
      https.end();
    }
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Send an HTTP POST request every 30 seconds
}
