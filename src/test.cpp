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

// Module Flags at the start
bool morseCodeModule = false;
bool drumpadModule = false;
bool mazeModule = false;

/*
===============================================================================
                            Morse Code Module
===============================================================================
*/

// -----------------------------------------------------------------------------
// Morse Code Light
// -----------------------------------------------------------------------------
#include <map>

// Variables -------------------------------------------------------------------

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

// Functions -------------------------------------------------------------------

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
#include <TM1637Display.h>

// Variables -------------------------------------------------------------------

// Pins
const int SevSegCLK = 22; // CLK pin for 7-segment display
const int SevSegDIO = 23; // DIO pin for 7-segment display
const int rotaryEncoderA = 35; // CLK pin for rotary encoder
const int rotaryEncoderB = 34; // DT pin for rotary encoder
const int rotaryEncoderButton = 36; // Button pin for rotary encoder
const int morseCodeConfirmationButton = 32; // Button pin for confirming Player's guess

// Display for "no"
const uint8_t SEG_NO[] = {
  SEG_C | SEG_E | SEG_G, // Displays n
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F // Displays o
};

// Create a display object of type TM1637Display
TM1637Display display(SevSegCLK, SevSegDIO);

// Changing Variables
int playerGuess = 1000;   // Initial number, constrained between 1000 and 9999
int rotaryIncrement = 1;  // Initial increment value for rotary encoder

// Checking states of inputs
int previousStateRotaryEncoderA = HIGH;
int previousStateRotaryButton = HIGH;
int previousConfirmationButtonState = HIGH;

// "Delay" Checks
unsigned long lastRotaryDebounceTime = 0;
unsigned long lastButtonDebounceTime = 0;
unsigned long lastConfirmationButtonDebounceTime = 0;
unsigned long lastPrintNoTime = 0;

// "Delays"
const unsigned long printNoDelay = 2000; // Delay for displaying "no" on the 7-segment display
const unsigned long debounceDelay = 100; // Debounce delay

bool pauseNumberDisplayMorseCode = false;

// Functions -------------------------------------------------------------------

void handleRotaryEncoder() {
  int rotaryA = digitalRead(rotaryEncoderA);
  int rotaryB = digitalRead(rotaryEncoderB);

  if (previousStateRotaryEncoderA == HIGH && rotaryA == LOW) {
    if (currentTime - lastRotaryDebounceTime >= debounceDelay) { // Debounce
      lastRotaryDebounceTime = currentTime;

      if (rotaryB == LOW) {
        playerGuess += rotaryIncrement;

        if (playerGuess > 9999) {
          playerGuess = 9999;
        }
        Serial.print("Player Guess: " + String(playerGuess)); // Debugging to see if number changes
      } 
      else {
        playerGuess -= rotaryIncrement;
        if (playerGuess < 1000) {
          playerGuess = 1000;
        }
        Serial.print("Player Guess: " + String(playerGuess)); // Debugging to see if number changes
      }
    }
  }
  previousStateRotaryEncoderA = rotaryA;
}

void handleRotaryButton() {
  int buttonState = digitalRead(rotaryEncoderButton);

  if (previousStateRotaryButton == HIGH && buttonState == LOW) {
    if (currentTime - lastButtonDebounceTime >= debounceDelay) { // Debounce
      lastButtonDebounceTime = currentTime;

    // Cycle through Increments: 1 -> 10 -> 100 -> 1000 -> 1
    switch (rotaryIncrement) {
      case 1:
        rotaryIncrement = 10;
        break;
      case 10:
        rotaryIncrement = 100;
        break;
      case 100:
        rotaryIncrement = 1000;
        break;
      case 1000:
        rotaryIncrement = 1; // Reset to 1
        break;
      default:
        rotaryIncrement = 1; // Default to 1 if something unexpected happens
        break;
      }
    }
    
    // Debugging
    Serial.println("Rotary Increment: " + String(rotaryIncrement)); // Debugging to see if increment changes
  }
  previousStateRotaryButton = buttonState;
}

void updateMorseCodeDisplay() {
  // Initialize the display
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);

  // If the display is paused for "no", do nothing
  if (pauseNumberDisplayMorseCode) {
    // Check if 2 seconds have passed to clear "no"
    if (currentTime - lastPrintNoTime >= printNoDelay) {
      lastPrintNoTime = currentTime;
      pauseNumberDisplayMorseCode = false; // Resume normal number display
      display.clear();                     // Clear the display
    }
  } else {
    // Display the player's guess
    display.showNumberDec(playerGuess, false);
  }
}

void playerGuessConfirmation() {
  int buttonState = digitalRead(morseCodeConfirmationButton);

  if (previousConfirmationButtonState == HIGH && buttonState == LOW) {
    if (currentTime - lastConfirmationButtonDebounceTime >= debounceDelay) {
      lastConfirmationButtonDebounceTime = currentTime;

      // Check if the player's guess matches the secret passcode
      if (playerGuess == numberPasscode) {
        // Morse Code Module Is Complete
        morseCodeModule = true;
      } else {
        // Player guess is incorrect
        morseCodeModule = false;
        pauseNumberDisplayMorseCode = true; // Pause the number display

        // Display "no" on the 7-segment display
        display.clear();
        display.setSegments(SEG_NO, 2, 1); // Display "no"
        lastPrintNoTime = currentTime;     // Start timer for "no"
      }
    }
  }
  previousConfirmationButtonState = buttonState;
}


void setupMorseCodeDisplay() {
  // Set the rotary encoder pins as inputs
  pinMode(rotaryEncoderA, INPUT);
  pinMode(rotaryEncoderB, INPUT);
  pinMode(rotaryEncoderButton, INPUT);
  pinMode(morseCodeConfirmationButton, INPUT);
}

void loopMorseCodeDisplay() {
  handleRotaryEncoder();
  handleRotaryButton();
  updateMorseCodeDisplay();
  playerGuessConfirmation();
}




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
// Variables ------------------------------------------------------------------- 

// Pins
const int startButtonPin = 18; // Start button pin
const int timerDIO = 5; // DIO pin for the timer display
const int timerCLK = 17; // CLK pin for the timer display

// Flags
int previousStateStartButton = HIGH;

unsigned long previousTimerTime = 0;

unsigned long lastStartButtonDebounceTime = 0;

int startGame = false;

int countdownTime = 5 * 60; // Countdown time in seconds (5 minutes)

// Create a display object of type TM1637Display
TM1637Display displayTimer(timerDIO, timerCLK);

// Functions -------------------------------------------------------------------


void setupTimer() {
  // Initialize the display
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  displayTimer.setBrightness(0x0f); // Set maximum brightness
  displayTimer.clear(); // Clear the display
}

void loopTimer() {

  // Check if 1 second has passed
  if (currentTime - previousTimerTime >= 1000) {
    previousTimerTime = currentTime; // Update the previous time

    if (countdownTime > 0) {
      countdownTime--; // Decrement countdown time

      // Calculate minutes and seconds
      int minutes = countdownTime / 60;
      int seconds = countdownTime % 60;

      // Debugging - Print the time remaining
      // Format the time as MM:SS for debugging
      Serial.print("Time remaining: ");
      Serial.print(minutes);
      Serial.print(":");
      if (seconds < 10) Serial.print("0"); // Add leading zero for seconds
      Serial.println(seconds);

      // Display the time on the 7-segment display
      int displayTime = (minutes * 100) + seconds; // Format as MMSS
      displayTimer.showNumberDecEx(displayTime, 0b01000000, true); // Display with colon
    } else {
      // Countdown complete
      displayTimer.clear();
      displayTimer.setSegments(SEG_NO, 2, 0); // Display "no" or complete message
      Serial.println("Countdown Complete!");

    }
  }
}

void setupStartButton() {
  pinMode(startButtonPin, INPUT);
}

void loopStartButton() {
  int startButton = digitalRead(startButtonPin);

  // Start game
  if(previousStateStartButton == HIGH && startButton == LOW) {
    startGame = true;
    Serial.println("Game Started!");
  }
  previousStateStartButton = startButton;
}

void setup() {
  Serial.begin(9600);

  // Game setup
  setupMorseCodeLight();
  setupMorseCodeDisplay();

  setupTimer();
  setupStartButton();
}

void loop() {
  currentTime = millis();

  loopStartButton();

  // Game loop
  if (startGame == true) {
    loopMorseCodeLight();
    loopMorseCodeDisplay();
    loopTimer();
  }

  // End game
  if (mazeModule && drumpadModule && morseCodeModule) {
    // Game is complete

    startGame = false;
  }
}