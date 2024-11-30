#include <Arduino.h>
#include <Wire.h>
#include <map>

// Module connection pins (Digital Pins)
#define MorseCode_LED 33 // LED for Morse code

// Global Variables
int DotDuration = 1000;
int DashDuration = 3000;
int betweenElements = 1000;
int betweenNumberDuration = 3000;

// Secret Morse code Passcode
int numberPasscode;

// Mapping morse code to digits
String digitToMorse(char digit) {
    switch (digit) {
        case '0': return "-----";
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        case '9': return "----.";
        default: return ""; // Invalid input
    }
}

// Function to convert number to Morse code
String numberToMorse(int number) {
    String morseCode = "";
    String numberStr = String(number); // Convert number to string
    for (size_t i = 0; i < numberStr.length(); i++) {
        morseCode += digitToMorse(numberStr[i]);
        if (i < numberStr.length() - 1) {
            morseCode += " "; // Add space between Morse codes
        }
    }
    return morseCode;
}

// Function to blink the LED according to Morse code
void blinkLEDMorseCode(const String &morseCode)
{
  for (char c : morseCode)
  {
    // Dot Duration
    if (c == '.')
    {
      digitalWrite(MorseCode_LED, HIGH);
      delay(DotDuration);
      digitalWrite(MorseCode_LED, LOW);
      delay(betweenElements);
    }
    // Dash Duration
    else if (c == '-')
    {
      digitalWrite(MorseCode_LED, HIGH);
      delay(DashDuration);
      digitalWrite(MorseCode_LED, LOW);
      delay(betweenElements);
    }
    // Gap Duration
    else
    {
      delay(betweenNumberDuration);
    }
  }
}

// Function to setup the code
void setup()
{
  // Start the serial communication
  Serial.begin(9600);

  // Set the LED pin as output
  pinMode(MorseCode_LED, OUTPUT);

  // Generate random number
  numberPasscode = esp_random() % 10000;
}

// Function to loop the code
void loop()
{
  // Convert number to Morse code
  String morseCode = numberToMorse(numberPasscode);

  // Blink the LED according to morse code
  while (true){
    blinkLEDMorseCode(morseCode);
    delay(7000);
  }

}