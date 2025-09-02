

const uint8_t BUTTON_PIN = 2;        
const uint8_t PIR_PIN    = 3;        
const uint8_t LED1_PIN   = LED_BUILTIN; 
const uint8_t LED2_PIN   = 10;       

volatile bool buttonEvent = false;
volatile bool pirEvent = false;

volatile bool led1State = false;
volatile bool led2State = false;

volatile uint32_t lastButtonMs = 0;
volatile uint32_t lastPirMs = 0;


void onButtonISR() {
  uint32_t now = millis();
  if (now - lastButtonMs > 200) {   
    led1State = !led1State;         
    digitalWrite(LED1_PIN, led1State);
    buttonEvent = true;             
    lastButtonMs = now;
  }
}


void onPirISR() {
  uint32_t now = millis();
  if (now - lastPirMs > 500) {      
    led2State = !led2State;         
    digitalWrite(LED2_PIN, led2State);
    pirEvent = true;                
    lastPirMs = now;
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);  
  pinMode(PIR_PIN, INPUT);            
  Serial.begin(115200);
  while (!Serial) {}

  
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), onPirISR, RISING);

  Serial.println(Task 4.2C ready: Button - LED1 (D13), PIR - LED2 (D10));
}

void loop() {
  
  if (buttonEvent) {
    noInterrupts();
    bool state = led1State;
    buttonEvent = false;
    interrupts();
    Serial.print("Button interrupt -> LED1: ");
    Serial.println(state ? "ON" : "OFF");
  }

  
  if (pirEvent) {
    noInterrupts();
    bool state = led2State;
    pirEvent = false;
    interrupts();
    Serial.print("PIR interrupt - LED2: ");
    Serial.println(state ? "ON" : "OFF");
  }

  delay(5); 
}
