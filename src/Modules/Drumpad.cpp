// #include <Arduino.h>
// #include <Wire.h>

// #define PIEZO_PIN 36        // Analog pin connected to piezo microphone
// #define SPEAKER_PIN 26      // DAC pin connected to speaker
// #define SAMPLE_RATE 440   // Sample rate for playback (440 Hz)

// void setup() {
//   Serial.begin(9600);
//   Serial.println("Piezo Microphone to Speaker Test...");
// }

// void loop() {
//   // Read the analog signal from the piezo
//   int piezoValue = analogRead(PIEZO_PIN);
  
//   // Map the piezo value (0-4095) to a DAC-compatible value (0-255)
//   int outputValue = map(piezoValue, 0, 4095, 0, 255);
  
//   // Output the mapped value to the speaker
//   dacWrite(SPEAKER_PIN, outputValue);
  
//   // Delay to control sample rate
//   delayMicroseconds(1000000 / SAMPLE_RATE); // Adjust based on desired frequency
// }


