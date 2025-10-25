#include <EmonLib.h>
EnergyMonitor emon1;   // Current sensor

// Calibration constants
const float voltageCalibration = 0.230;  // Tune this for your ZMPT101B (V per ADC unit)
const float currentCalibration = 30.0;   // ACS712 30A module

const float ratePerUnit = 10.0;  // ₹10 per kWh

float voltageRMS = 0.0;
float currentRMS = 0.0;
float powerW = 0.0;
float energyKWh = 0.0;
float bill = 0.0;

void setup() {
  Serial.begin(9600);
  delay(2000);

  emon1.current(A0, currentCalibration); // Current sensor
  Serial.println("Smart Energy Monitor Started...");
  Serial.println("Voltage(V), Current(A), Power(W), Energy(kWh), Bill(Rs)");
}

void loop() {
  // Measure current RMS
  currentRMS = emon1.calcIrms(1480);

  // Measure voltage RMS manually
  int rawVoltage = analogRead(A1);
  voltageRMS = rawVoltage * voltageCalibration;  // Scale ADC reading

  // Calculate power
  powerW = voltageRMS * currentRMS;

  // Calculate energy (kWh)
  energyKWh += powerW / 1000.0 / 3600.0;
 // Calculate bill
  bill = energyKWh * ratePerUnit;
  // Print data

Serial.print(voltageRMS, 2); Serial.print(",");
Serial.print(currentRMS, 3); Serial.print(",");
Serial.print(powerW, 2); Serial.print(",");
Serial.print(energyKWh, 6); Serial.print(",");
Serial.println(bill, 4);
 delay(1000);
}
