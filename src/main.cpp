/*
  * This project is to show a game inspired by the game Keep Talking and Nobody Explodes.
  * The game is a two player game will have 3 modules which are
  * 1. Morse Code Module
  * 2. Drumpad Module
  * 3. Maze Module
*/


// Libraries
#include <Arduino.h>
#include <Wire.h>

// Global Variables
unsigned long currentTime = 0;

/*
===============================================================================
                            Morse Code Module
===============================================================================
*/

// -----------------------------------------------------------------------------
// Morse Code Light
// -----------------------------------------------------------------------------
#include <map>

// Pins
const int morseCodeLED = 33; // LED for Morse code

// Morse Code LED state and checks
bool morseCodeLEDState = LOW;
unsigned long previousMorseCodeLEDTime = 0;
unsigned long delayDurationMorseCode = 0;

// Time duration for Morse code
unsigned long dotDuration = 1000; // How long the light is on for a dot
unsigned long dashDuration = 3000;  // How long the light is on for a dash
unsigned long betweenElementsDuration = 1000; // How long the light is off between dots and/or dashes
unsigned long betweenDigitsDuration = 3000; // How long the light is off between digits
unsigned long betweenSequenceDuration = 7000; // How long the light is off before the sequence restarts

// Secret Morse code Passcode
int numberPasscode;
String encryptedPasscode;

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

void blinkLEDMorseCode(const String &morseCode) {
  static size_t currentIndex = 0; // Tracks the current character in the Morse code
  static bool isWaiting = false; // Flag for waiting periods between sequences

  // If we're waiting between sequences
  if (isWaiting) {
    if (currentTime - previousMorseCodeLEDTime >= betweenSequenceDuration) {
      isWaiting = false;
      currentIndex = 0; // Reset to the start of the Morse code
    }
    return; // Do nothing until the waiting period is over
  }

  // If we've finished the Morse code, set the waiting flag
  if (currentIndex >= morseCode.length()) {
    isWaiting = true;
    previousMorseCodeLEDTime = currentTime;
    digitalWrite(morseCodeLED, LOW); // Ensure LED is off
    return;
  }

  // Get the current character in the Morse code
  char currentChar = morseCode[currentIndex];

  // Handle LED behavior based on the character
  if (currentTime - previousMorseCodeLEDTime >= delayDurationMorseCode) {
    if (currentChar == '.') {
      morseCodeLEDState = !morseCodeLEDState; // Toggle LED
      delayDurationMorseCode = morseCodeLEDState ? dotDuration : betweenElementsDuration;
    } else if (currentChar == '-') {
      morseCodeLEDState = !morseCodeLEDState; // Toggle LED
      delayDurationMorseCode = morseCodeLEDState ? dashDuration : betweenElementsDuration;
    } else if (currentChar == ' ') {
      morseCodeLEDState = LOW; // Ensure LED is off for space
      delayDurationMorseCode = betweenDigitsDuration;
    }

    // Update the LED state
    digitalWrite(morseCodeLED, morseCodeLEDState ? HIGH : LOW);

    // Move to the next character if LED is off or during space
    if (!morseCodeLEDState || currentChar == ' ') {
      currentIndex++;
    }

    // Update the previous time
    previousMorseCodeLEDTime = currentTime;
  }
}

void setupMorseCodeLight() {
  // Set up Morse code LED
  pinMode(morseCodeLED, OUTPUT);

  // Generate a random passcode
  numberPasscode = 1000 + esp_random() % 9000;

  // Convert passcode to Morse code
  encryptedPasscode = numberToMorse(numberPasscode);
}

void loopMorseCodeLight() {
  // Debugging - Print the secret passcode
  static bool passcodePrinted = false;
  if (!passcodePrinted) {
    Serial.println("Secret Passcode: " + String(numberPasscode));
    passcodePrinted = true;
  }

  // Blink Morse code LED
  blinkLEDMorseCode(encryptedPasscode);
}
// -----------------------------------------------------------------------------
// Morse Code Input
// -----------------------------------------------------------------------------

/*
================================================================================
                              Drumpad Module
================================================================================
*/

/*
================================================================================
                                Maze Module
================================================================================
*/

/*
================================================================================
                        Miniaml setup for the project
================================================================================
*/
void setup() {
  Serial.begin(9600);

  setupMorseCodeLight();
}

void loop() {
  currentTime = millis();

  loopMorseCodeLight();

}