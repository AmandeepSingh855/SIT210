#include "arduino_secrets.h"
#include "thingProperties.h"

#define DOT_DURATION 200            // Duration of dot (in milliseconds)
#define DASH_DURATION 600           // Duration of dash
#define PAUSE_BETWEEN_PARTS 200     // Pause between dots and dashes
#define PAUSE_BETWEEN_LETTERS 600   // Pause between letters
#define PAUSE_BETWEEN_WORDS 1200    // Pause between words

// Short blink (dot)
void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DOT_DURATION);
  digitalWrite(LED_BUILTIN, LOW);
  delay(PAUSE_BETWEEN_PARTS);
}

// Long blink (dash)
void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DASH_DURATION);
  digitalWrite(LED_BUILTIN, LOW);
  delay(PAUSE_BETWEEN_PARTS);
}

// Morse code for letter A (.-)
void blinkA() {
  dot();
  dash();
  delay(PAUSE_BETWEEN_LETTERS);
}

// Morse code for letter M (--)
void blinkM() {
  dash();
  dash();
  delay(PAUSE_BETWEEN_LETTERS);
}

// Morse code for letter N (-.)
void blinkN() {
  dash();
  dot();
  delay(PAUSE_BETWEEN_LETTERS);
}

void setup() {
  Serial.begin(9600);
  delay(1500);  // Wait for serial monitor to start if open

  pinMode(LED_BUILTIN, OUTPUT);   // Initialize built-in LED as output

  initProperties();               // Initialize IoT Cloud properties (from thingProperties.h)

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Blink your name "AMAN" in Morse code ONLY if ledState == true
  if (ledState) {
    blinkA();
    blinkM();
    blinkA();
    blinkN();
    delay(PAUSE_BETWEEN_WORDS);  // Pause before repeating the name
  } else {
    // Turn off LED when ledState is false
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// This function is automatically called when 'ledState' changes from the cloud
void onLedStateChange() {
  if (ledState) {
    Serial.println("Blinking AMAN started.");
  } else {
    Serial.println("Blinking AMAN stopped.");
    digitalWrite(LED_BUILTIN, LOW);  // Ensure LED is off when stopped
  }
}
