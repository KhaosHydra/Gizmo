/*############################################################################
  #                 This code is testinng the rotary encoder                 #
  ############################################################################*/

// Libraries
#include <Arduino.h> // Arduino library
#include <Wire.h> // I2C library

// Pins
#define rotaryEncoderA 35
#define rotaryEncoderB 34
#define RotaryEncoderButton 32

// Variables
int number = 0;
int previousStateRotaryEncoderA = HIGH;
int previousStateRotaryEncoderB = HIGH;
int previousStateRotaryEncoderButton = HIGH;

// Functions

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(rotaryEncoderA, INPUT);
  pinMode(rotaryEncoderB, INPUT);
  pinMode(RotaryEncoderButton, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rotaryA = digitalRead(rotaryEncoderA);
  int rotaryB = digitalRead(rotaryEncoderB);
  int buttonState = digitalRead(RotaryEncoderButton);

  if (previousStateRotaryEncoderA == HIGH && rotaryA == LOW) {
    delay(50); // debounce delay
    if (rotaryB == LOW) {
      // Turn right function
      number++;
      Serial.println(number);
    }
    else{
      // Turn left function
      number--;
    Serial.println(number);
    }
  }
  previousStateRotaryEncoderA = rotaryA;


  // Checks debounced button press
  if (previousStateRotaryEncoderButton == HIGH && buttonState == LOW) {
    delay(50); // debounce delay
    if (digitalRead(RotaryEncoderButton) == LOW) { // check again after delay
    // do a function
      Serial.println("Button pressed");
    }
  }
  previousStateRotaryEncoderButton = buttonState;

}