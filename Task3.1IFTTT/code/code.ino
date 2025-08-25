#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

char ssid[] = "M34";
char pass[] = "jatt@+123";

const char server[] = "maker.ifttt.com";
String IFTTT_Event = "amansunlight";
String IFTTT_Key   = "g2Bjp_j2UumLjCrG3hzQriqbNsE8D3-DgIWBb3ECsGp";
int port = 80;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

int thresholdLux = 2000;
bool sunlight = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(2000);
  }
  Serial.println("Connected");
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.println(lux);
  if (lux > thresholdLux && !sunlight) {
    triggerIFTTT(IFTTT_Event);
    sunlight = true;
  }
  else if (lux <= thresholdLux && sunlight) {
    triggerIFTTT(IFTTT_Event);
    sunlight = false;
  }
  delay(10000);
}

void triggerIFTTT(String event) {
  String url = "/trigger/" + event + "/with/key/" + IFTTT_Key;
  client.get(url);
  Serial.println("Triggered: " + event);
}
