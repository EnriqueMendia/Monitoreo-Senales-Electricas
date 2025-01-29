#include <WiFi.h>
#include <Wire.h>
#include <RTClib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiClientSecure.h>

// Definición de pines para los módulos
#define RELAY_PIN 23       // Relé conectado al pin D23
#define SDA_PIN 21         // SDA del RTC conectado al pin D21
#define SCL_PIN 22         // SCL del RTC conectado al pin D22
#define PIN_DS18B20 15     // DS18B20 conectado al pin D15
#define PIN_ZMPT 35        // ZMPT101B conectado al pin D35
#define PIN_ACS712 34      // ACS712 conectado al pin D34

// Credenciales WiFi
const char* ssid = "Wifi kikin";            
const char* password = "87654321k";

// Token de Telegram y Chat ID
const char* token = "7777089180:AAHOIsqZ3K0xIu_lN3cvs7CUkNs75tER-UE";  
const char* chat_id = "1271362249";

// Definición de la conexión a ThingSpeak
const char* server = "api.thingspeak.com";    
String apiKey = "67HY3KSL6NG2DDBI";    
WiFiClient client;

// Objetos RTC y sensores
RTC_DS3231 rtc;
OneWire oneWire(PIN_DS18B20); 
DallasTemperature sensors(&oneWire);

// Variables globales
float voltage = 0.0;      
float current = 0.0;      
float temperature = 0.0;  
bool relayState = false;  

// Función de conexión WiFi
void setupWiFi() {
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConexión WiFi establecida");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nError: No se pudo conectar a WiFi.");
  }
}

// Leer corriente del ACS712
float readCurrent() {
  int sensorValue = analogRead(PIN_ACS712);
  float voltage = (sensorValue / 4095.0) * 3.3; // Resolución ADC de 12 bits
  return (voltage - 2.5) / 0.185; // Sensibilidad de 185 mV/A
}

// Leer voltaje del ZMPT101B
float readVoltage() {
  int sensorValue = analogRead(PIN_ZMPT);
  float voltage = (sensorValue / 4095.0) * 3.3; // Resolución ADC
  return voltage * 230.0; // Factor de calibración
}

// Leer temperatura del DS18B20
float readTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// Enviar mensaje por Telegram
void sendTelegramMessage(const String& text) {
  WiFiClientSecure secureClient;
  secureClient.setInsecure();  
  String url = "https://api.telegram.org/bot" + String(token) + "/sendMessage?chat_id=" + String(chat_id) + "&text=" + text;

  if (secureClient.connect("api.telegram.org", 443)) {
    secureClient.println("GET " + url + " HTTP/1.1");
    secureClient.println("Host: api.telegram.org");
    secureClient.println("Connection: close");
    secureClient.println();
    delay(500);
  } else {
    Serial.println("Error: No se pudo conectar a Telegram.");
  }
  secureClient.stop();
}

// Configurar ThingSpeak
void setupThingspeak() {
  if (client.connect(server, 80)) {
    Serial.println("Conectado a ThingSpeak");
  } else {
    Serial.println("Error al conectar a ThingSpeak");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  setupWiFi();
  sensors.begin();

  if (!rtc.begin()) {
    Serial.println("Error: No se pudo inicializar el RTC.");
  }

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relé apagado por defecto
}

void loop() {
  // Leer sensores
  current = readCurrent();
  voltage = readVoltage();
  temperature = readTemperature();

  // Enviar datos a ThingSpeak
  String data = "field1=" + String(voltage) + "&field2=" + String(current) + "&field3=" + String(temperature);
  if (client.connect(server, 80)) {
    client.print(String("POST /update HTTP/1.1\r\n") +
                 "Host: api.thingspeak.com\r\n" +
                 "Connection: close\r\n" +
                 "X-THINGSPEAKAPIKEY: " + apiKey + "\r\n" +
                 "Content-Type: application/x-www-form-urlencoded\r\n" +
                 "Content-Length: " + data.length() + "\r\n\r\n" +
                 data);
    client.stop();
  } else {
    Serial.println("Error: No se pudo enviar datos a ThingSpeak.");
  }

  // Alerta de corriente alta o voltaje bajo
  if (current > 5.0) {
    sendTelegramMessage("⚠ ¡Alerta! Corriente alta detectada: " + String(current, 2) + " A");
  }

  if (voltage < 190.0) {
    sendTelegramMessage("⚠ ¡Alerta! Voltaje bajo detectado: " + String(voltage, 2) + " V");
  }

  // Control del relé
  if (voltage < 190.0 || current > 5.0) {
    digitalWrite(RELAY_PIN, HIGH); // Encender relé
    relayState = true;
  } else {
    digitalWrite(RELAY_PIN, LOW); // Apagar relé
    relayState = false;
  }

  delay(10000); // Esperar 10 segundos antes de la siguiente lectura
}