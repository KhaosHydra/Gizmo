#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>

#define speakerPin 25
#define piezoDisk 34
#define LEDColouredRing 23
#define scl_Trill 22
#define sda_Trill 21

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(piezoDisk, INPUT);

  Serial.begin(115200);
}

void loop() {
 int piezoReading = analogRead(piezoDisk); // Read analog input from piezo disk

  // If a significant tap is detected on the piezo
  if (piezoReading > 20) { // Threshold to detect taps (adjust as needed)
    Serial.println("Tap detected!");

    // Generate a simple tone on the speaker
    tone(speakerPin, 1000, 200); // Play a 1kHz tone for 200ms
    delay(200);                   // Wait for the tone to finish
  }

  delay(50); // Small delay to avoid rapid triggering
}
