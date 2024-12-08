#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>

// Define the servo pin
const int servoPin = 27;

// Create a servo object
Servo myServo;

// Define positions
int position = 0;         // Variable to store the servo position
int step = 1;             // Step size for the back-and-forth motion

void setup() {
  // Attach the servo to the specified pin
  myServo.attach(servoPin);
}

void loop() {
  // Sweep from 0 to 180 degrees
  for (position = 0; position <= 180; position += step) {
    myServo.write(position); // Move the servo to the position
    delay(15);               // Wait for the servo to reach the position
  }

  // Sweep from 180 to 0 degrees
  for (position = 180; position >= 0; position -= step) {
    myServo.write(position);
    delay(15);
  }
}
