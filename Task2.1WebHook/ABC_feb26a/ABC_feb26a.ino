#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "DHT.h"

// Wi-Fi details
const char* ssid = "M34";         
const char* pass = "jatt@+123";     

// ThingSpeak details
unsigned long channelID =  3026522;   
const char* writeAPIKey = "TXQ5AQKECF4A27EC"; 

WiFiClient client;

// DHT Setup
#define DHTPIN 2          // Data pin connected to D2
#define DHTTYPE DHT11     // DHT11 Sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop() {
  float temperature = dht.readTemperature();  // Read temperature in Celsius

  // Check if the temperature reading is valid
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }

  // Print temperature to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Send temperature to ThingSpeak (Field 1)
  ThingSpeak.setField(1, temperature);
  ThingSpeak.writeFields(channelID, writeAPIKey);

  delay(30000); // Wait 30 seconds before sending again
}
