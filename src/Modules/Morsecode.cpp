// #include <Arduino.h>
// #include "SevSegShift.h"

// // Define the pins for the shift registers
// #define DATA_PIN  23  // DS (Data Serial)
// #define LATCH_PIN 22  // ST_CP (Storage Register Clock Pin)
// #define CLOCK_PIN 21  // SH_CP (Shift Register Clock Pin)

// // Create a SevSegShift object
// SevSegShift sevseg(DATA_PIN, CLOCK_PIN, LATCH_PIN);

// void setup() {
//   // Initialize Serial Monitor for debugging
//   Serial.begin(9600);

//   // Initialize the SevSegShift object
//   byte numDigits = 4;
//   byte digitPins[] = {8+1, 8+2, 8+3, 8+4}; // of ShiftRegister(s) | 8+x (2nd Register)
//   byte segmentPins[] = {0, 1, 2, 3, 4, 5, 6}; // of Shiftregister(s) | 8+x (2nd Register)
//   bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
//   byte hardwareConfig = COMMON_ANODE; // Your display type
//   bool updateWithDelays = false; // Default 'false' is recommended
//   bool leadingZeros = false; // Use 'true' if you want leading zeros
//   bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

//   sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
//   sevseg.setBrightness(100); // Set brightness (0-100)
// }

// void loop() {
//   sevseg.setNumber(1234);
//   sevseg.refreshDisplay(); // Must run repeatedly to refresh the display
//   delay(1000); // Update the display every second
// }