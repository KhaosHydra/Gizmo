#include <Arduino.h>
#include <Wire.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define SevSegCLK 22
#define SevSegDIO 23
#define rotaryEncoderA 35
#define rotaryEncoderB 34
#define RotaryEncoderButton 32

const uint8_t SEG_NO[] = {
  SEG_C | SEG_E | SEG_G,                          // Displays n
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F   // Displays o
};

TM1637Display display(SevSegCLK, SevSegDIO);

// Variables
int number = 0;
int previousStateRotaryEncoderA = HIGH;
int previousStateRotaryEncoderB = HIGH;
int previousStateRotaryEncoderButton = HIGH;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(rotaryEncoderA, INPUT);
  pinMode(rotaryEncoderB, INPUT);
  pinMode(RotaryEncoderButton, INPUT);
}

void loop()
{
  // Initialize the display
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);

  // seeing position of rotary encoder
  int rotaryA = digitalRead(rotaryEncoderA);
  int rotaryB = digitalRead(rotaryEncoderB);
  int buttonState = digitalRead(RotaryEncoderButton);

  // change display number
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
  previousStateRotaryEncoderA = rotaryA;  // save the current state as the last state, for next time through the loop

    // Checks debounced button press
  if (previousStateRotaryEncoderButton == HIGH && buttonState == LOW) {
    delay(50); // debounce delay
    if (digitalRead(RotaryEncoderButton) == LOW) { // check again after delay
    // do a function
      display.clear();
      // Serial.println("Button pressed");
      display.setSegments(SEG_NO, 2, 1);  // Display "no" in the centre
      delay(5000);
    }
  }
  previousStateRotaryEncoderButton = buttonState;   // save the current state as the last state, for next time through the loop

  display.showNumberDec(number, false); // Display

}