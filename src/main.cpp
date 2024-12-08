// /*
//   * This project is to show a game inspired by the game Keep Talking and Nobody Explodes.
//   * The game is a two player game will have 3 modules which are
//   * 1. Morse Code Module
//   * 2. Drumpad Module
//   * 3. Maze Module
// */


// // Libraries
// #include <Arduino.h>
// #include <Wire.h>

// // Global Variables
// unsigned long currentTime = 0;

// // Module Flags at the start
// bool morseCodeModule = false;
// bool drumpadModule = false;
// bool mazeModule = false;

// bool morseCodeMistake = false;

// /*
// ===============================================================================
//                             Morse Code Module
// ===============================================================================
// */

// // -----------------------------------------------------------------------------
// // Morse Code Light
// // -----------------------------------------------------------------------------
// #include <map>

// // Variables -------------------------------------------------------------------

// // Pins
// const int morseCodeLED = 33; // LED for Morse code

// // Morse Code LED state and checks
// bool morseCodeLEDState = LOW;
// unsigned long previousMorseCodeLEDTime = 0;
// unsigned long delayDurationMorseCode = 0;

// // Time duration for Morse code
// unsigned long dotDuration = 1000; // How long the light is on for a dot
// unsigned long dashDuration = 3000;  // How long the light is on for a dash
// unsigned long betweenElementsDuration = 1000; // How long the light is off between dots and/or dashes
// unsigned long betweenDigitsDuration = 3000; // How long the light is off between digits
// unsigned long betweenSequenceDuration = 7000; // How long the light is off before the sequence restarts

// // Secret Morse code Passcode
// int numberPasscode;
// String encryptedPasscode;

// // Functions -------------------------------------------------------------------

// // Mapping morse code to digits
// String digitToMorse(char digit) {
//     switch (digit) {
//         case '0': return "-----";
//         case '1': return ".----";
//         case '2': return "..---";
//         case '3': return "...--";
//         case '4': return "....-";
//         case '5': return ".....";
//         case '6': return "-....";
//         case '7': return "--...";
//         case '8': return "---..";
//         case '9': return "----.";
//         default: return ""; // Invalid input
//     }
// }

// // Function to convert number to Morse code
// String numberToMorse(int number) {
//     String morseCode = "";
//     String numberStr = String(number); // Convert number to string
//     for (size_t i = 0; i < numberStr.length(); i++) {
//         morseCode += digitToMorse(numberStr[i]);
//         if (i < numberStr.length() - 1) {
//             morseCode += " "; // Add space between Morse codes
//         }
//     }
//     return morseCode;
// }

// void blinkLEDMorseCode(const String &morseCode) {
//   static size_t currentIndex = 0; // Tracks the current character in the Morse code
//   static bool isWaiting = false; // Flag for waiting periods between sequences

//   // If we're waiting between sequences
//   if (isWaiting) {
//     if (currentTime - previousMorseCodeLEDTime >= betweenSequenceDuration) {
//       isWaiting = false;
//       currentIndex = 0; // Reset to the start of the Morse code
//     }
//     return; // Do nothing until the waiting period is over
//   }

//   // If we've finished the Morse code, set the waiting flag
//   if (currentIndex >= morseCode.length()) {
//     isWaiting = true;
//     previousMorseCodeLEDTime = currentTime;
//     digitalWrite(morseCodeLED, LOW); // Ensure LED is off
//     return;
//   }

//   // Get the current character in the Morse code
//   char currentChar = morseCode[currentIndex];

//   // Handle LED behavior based on the character
//   if (currentTime - previousMorseCodeLEDTime >= delayDurationMorseCode) {
//     if (currentChar == '.') {
//       morseCodeLEDState = !morseCodeLEDState; // Toggle LED
//       delayDurationMorseCode = morseCodeLEDState ? dotDuration : betweenElementsDuration;
//     } else if (currentChar == '-') {
//       morseCodeLEDState = !morseCodeLEDState; // Toggle LED
//       delayDurationMorseCode = morseCodeLEDState ? dashDuration : betweenElementsDuration;
//     } else if (currentChar == ' ') {
//       morseCodeLEDState = LOW; // Ensure LED is off for space
//       delayDurationMorseCode = betweenDigitsDuration;
//     }

//     // Update the LED state
//     digitalWrite(morseCodeLED, morseCodeLEDState ? HIGH : LOW);

//     // Move to the next character if LED is off or during space
//     if (!morseCodeLEDState || currentChar == ' ') {
//       currentIndex++;
//     }

//     // Update the previous time
//     previousMorseCodeLEDTime = currentTime;
//   }
// }

// void setupMorseCodeLight() {
//   // Set up Morse code LED
//   pinMode(morseCodeLED, OUTPUT);

//   // Generate a random passcode
//   numberPasscode = 1000 + esp_random() % 9000;

//   // Convert passcode to Morse code
//   encryptedPasscode = numberToMorse(numberPasscode);
// }

// void loopMorseCodeLight() {
//   // Debugging - Print the secret passcode
//   static bool passcodePrinted = false;
//   if (!passcodePrinted) {
//     Serial.println("Secret Passcode: " + String(numberPasscode));
//     passcodePrinted = true;
//   }

//   // Blink Morse code LED
//   blinkLEDMorseCode(encryptedPasscode);
// }

// // -----------------------------------------------------------------------------
// // Morse Code Input
// // -----------------------------------------------------------------------------
// #include <TM1637Display.h>

// // Variables -------------------------------------------------------------------

// // Pins
// const int SevSegCLK = 13; // CLK pin for 7-segment display
// const int SevSegDIO = 14; // DIO pin for 7-segment display
// const int rotaryEncoderA = 34; // CLK pin for rotary encoder
// const int rotaryEncoderB = 35; // DT pin for rotary encoder
// const int rotaryEncoderButton = 32; // Button pin for rotary encoder
// const int morseCodeConfirmationButton = 27; // Button pin for confirming Player's guess

// // Display for "no"
// const uint8_t SEG_NO[] = {
//   SEG_C | SEG_E | SEG_G, // Displays n
//   SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F // Displays o
// };

// const uint8_t SEG_YES[] = {
//   SEG_B | SEG_C | SEG_D | SEG_F | SEG_G, // Displays y
//   SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, // Displays e
//   SEG_A | SEG_C | SEG_D | SEG_F | SEG_G // Displays s
// };

// // Create a display object of type TM1637Display
// TM1637Display display(SevSegCLK, SevSegDIO);

// // Changing Variables
// int playerGuess = 1000;   // Initial number, constrained between 1000 and 9999
// int rotaryIncrement = 1;  // Initial increment value for rotary encoder

// // Checking states of inputs
// int previousStateRotaryEncoderA = HIGH;
// int previousStateRotaryButton = HIGH;
// int previousConfirmationButtonState = HIGH;

// // "Delay" Checks
// unsigned long lastRotaryDebounceTime = 0;
// unsigned long lastButtonDebounceTime = 0;
// unsigned long lastConfirmationButtonDebounceTime = 0;
// unsigned long lastPrintNoTime = 0;

// // "Delays"
// const unsigned long printNoDelay = 2000; // Delay for displaying "no" on the 7-segment display
// const unsigned long debounceDelay = 500; // Debounce delay

// bool pauseNumberDisplayMorseCode = false;
// bool morseCodee = false;

// // Functions -------------------------------------------------------------------

// void handleRotaryEncoder() {
//   int rotaryA = digitalRead(rotaryEncoderA);
//   int rotaryB = digitalRead(rotaryEncoderB);

//   if (previousStateRotaryEncoderA == HIGH && rotaryA == LOW) {
//     if (currentTime - lastRotaryDebounceTime >= debounceDelay) { // Debounce
//       lastRotaryDebounceTime = currentTime;

//       if (rotaryB == LOW) {
//         playerGuess += rotaryIncrement;

//         if (playerGuess > 9999) {
//           playerGuess = 9999;
//         }
//         Serial.print("Player Guess: " + String(playerGuess)); // Debugging to see if number changes
//       } 
//       else {
//         playerGuess -= rotaryIncrement;
//         if (playerGuess < 1000) {
//           playerGuess = 1000;
//         }
//         Serial.print("Player Guess: " + String(playerGuess)); // Debugging to see if number changes
//       }
//     }
//   }
//   previousStateRotaryEncoderA = rotaryA;
// }

// void handleRotaryButton() {
//   int buttonState = digitalRead(rotaryEncoderButton);

//   if (previousStateRotaryButton == HIGH && buttonState == LOW) {
//     if (currentTime - lastButtonDebounceTime >= debounceDelay) { // Debounce
//       lastButtonDebounceTime = currentTime;

//     // Cycle through Increments: 1 -> 10 -> 100 -> 1000 -> 1
//     switch (rotaryIncrement) {
//       case 1:
//         rotaryIncrement = 10;
//         break;
//       case 10:
//         rotaryIncrement = 100;
//         break;
//       case 100:
//         rotaryIncrement = 1000;
//         break;
//       case 1000:
//         rotaryIncrement = 1; // Reset to 1
//         break;
//       default:
//         rotaryIncrement = 1; // Default to 1 if something unexpected happens
//         break;
//       }
//     }
    
//     // Debugging
//     Serial.println("Rotary Increment: " + String(rotaryIncrement)); // Debugging to see if increment changes
//   }
//   previousStateRotaryButton = buttonState;
// }

// void updateMorseCodeDisplay() {
//   // Initialize the display
//   int k;
//   uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
//   uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
//   display.setBrightness(0x0f);

//   // If the display is paused for "no", do nothing
//   if (pauseNumberDisplayMorseCode) {
//     // Check if 2 seconds have passed to clear "no"
//     if (currentTime - lastPrintNoTime >= printNoDelay) {
//       lastPrintNoTime = currentTime;
//       pauseNumberDisplayMorseCode = false; // Resume normal number display
//       display.clear();                     // Clear the display
//     }
//   } else {
//     // Display the player's guess
//     display.showNumberDec(playerGuess, false);
//   }
// }

// void playerGuessConfirmation() {
//   int buttonState = digitalRead(morseCodeConfirmationButton);

//   if (previousConfirmationButtonState == HIGH && buttonState == LOW) {
//     if (currentTime - lastConfirmationButtonDebounceTime >= debounceDelay) {
//       lastConfirmationButtonDebounceTime = currentTime;

//       // Check if the player's guess matches the secret passcode
//       if (playerGuess == numberPasscode) {
//         // Morse Code Module Is Complete
//         morseCodeModule = true;
//         pauseNumberDisplayMorseCode = true; // Pause the number display

//         display.clear(); // Clear the display
//         display.setSegments(SEG_YES, 3, 0); // Display "yes"
//         lastPrintNoTime = currentTime; // Start timer for "yes"

//       } else {
//         // Player guess is incorrect
//         morseCodeModule = false;
//         pauseNumberDisplayMorseCode = true; // Pause the number display

//         // Display "no" on the 7-segment display
//         display.clear();
//         display.setSegments(SEG_NO, 2, 1); // Display "no"
//         lastPrintNoTime = currentTime;     // Start timer for "no"

//         morseCodeMistake = true; // Set the mistake flag
//       }
//     }
//   }
//   previousConfirmationButtonState = buttonState;
// }


// void setupMorseCodeDisplay() {
//   // Set the rotary encoder pins as inputs
//   pinMode(rotaryEncoderA, INPUT);
//   pinMode(rotaryEncoderB, INPUT);
//   pinMode(rotaryEncoderButton, INPUT);
//   pinMode(morseCodeConfirmationButton, INPUT);
// }

// void loopMorseCodeDisplay() {
//   handleRotaryEncoder();
//   handleRotaryButton();
//   updateMorseCodeDisplay();
//   playerGuessConfirmation();
// }




// /*
// ================================================================================
//                               Drumpad Module
// ================================================================================
// */

// /*
// ================================================================================
//                                 Maze Module
// ================================================================================
// */

// #include <ESP32Servo.h>

// #include <MPU6050.h>

// // Variables -------------------------------------------------------------------

// // Pins
// const int servoXPin = 16; // Servo motor X-axis
// const int servoYPin = 17; // Servo motor Y-axis
// const int mpuSDAPin = 21; // SDA pin for MPU6050
// const int mpuSCLPin = 22; // SCL pin for MPU6050
// const int irSensorPin[4] = {36, 39, 15, 2}; // IR sensor pins
// const int cornerLEDLatchPin = 0; // Latch pin for corner LEDs
// const int cornerLEDCLKPin = 0; // Clock pin for corner LEDs
// const int cornerLEDDataPin = 0; // Data pin for corner LEDs

// int sensorValue[4] = {0, 0, 0, 0}; // Array to store sensor values

// // Variables for Maze
// const int irSensorPin[4] = {36, 39, 15, 2}; // IR sensor pins
// const int cornerLEDLatchPin = 25;           // Latch pin for corner LEDs
// const int cornerLEDCLKPin = 26;             // Clock pin for corner LEDs
// const int cornerLEDDataPin = 15;            // Data pin for corner LEDs

// Servo servoX, servoY;
// const int servoXPin = 16;
// const int servoYPin = 17;

// int gyroXOffset = 0, gyroYOffset = 0;
// bool selfBalancingActive = false;
// unsigned long selfBalancingStartTime = 0;

// MPU6050 gyroSensor;

// // IR Sensor and Corner LED Logic
// const byte cornerLEDValues[] = {
//     B00000001, // Corner 1
//     B00000010, // Corner 2
//     B00000100, // Corner 3
//     B00001000  // Corner 4
// };

// bool randomCornerSelected = false;
// int selectedCorner = -1; // Stores the selected corner
// bool allLightsOn = false;

// void calibrateGyro() {
//     Serial.println("Calibrating Gyroscope...");
//     Wire.begin();
//     gyroSensor.initialize();
//     if (gyroSensor.testConnection()) {
//         Serial.println("MPU6050 connection successful");
//     } else {
//         Serial.println("MPU6050 connection failed");
//     }
//     // Manually calibrate offsets
//     gyroXOffset = gyroSensor.getXGyroOffset();
//     gyroYOffset = gyroSensor.getYGyroOffset();
//     Serial.println("Gyroscope Calibrated.");
// }

// void moveServosOppositeGyro() {
//     if (!selfBalancingActive) {
//         selfBalancingActive = true;
//         selfBalancingStartTime = millis();
//     }

//     if (millis() - selfBalancingStartTime < 10000) { // 10 seconds
//         int gyroX = gyroSensor.getRotationX() - gyroXOffset;
//         int gyroY = gyroSensor.getRotationY() - gyroYOffset;

//         int servoXPos = constrain(90 - gyroX / 100, 0, 180); // Map to servo range
//         int servoYPos = constrain(90 - gyroY / 100, 0, 180);

//         servoX.write(servoXPos);
//         servoY.write(servoYPos);
//     } else {
//         servoX.write(90); // Reset to center
//         servoY.write(90);
//         selfBalancingActive = false;
//         morseCodeMistake = false; // Reset mistake flag
//     }
// }

// void cornerLED(int corner) {
//     digitalWrite(cornerLEDLatchPin, LOW);
//     shiftOut(cornerLEDDataPin, cornerLEDCLKPin, MSBFIRST, cornerLEDValues[corner]);
//     digitalWrite(cornerLEDLatchPin, HIGH);
// }

// void handleMazeLogic() {
//     // Select a random corner at the start
//     if (!randomCornerSelected) {
//         selectedCorner = random(0, 4); // Choose a random corner (0-3)
//         cornerLED(selectedCorner);
//         randomCornerSelected = true;
//         Serial.println("Random Corner Selected: " + String(selectedCorner + 1));
//     }

//     // Check IR sensors
//     allLightsOn = false;
//     for (int i = 0; i < 4; i++) {
//         int sensorValue = analogRead(irSensorPin[i]);
//         if (sensorValue < 3900) {
//             allLightsOn = true;
//             break;
//         }
//     }

//     if (allLightsOn) {
//         digitalWrite(cornerLEDLatchPin, LOW);
//         shiftOut(cornerLEDDataPin, cornerLEDCLKPin, MSBFIRST, B11111111); // All LEDs on
//         digitalWrite(cornerLEDLatchPin, HIGH);
//         mazeModule = true; // Set the module as complete
//         Serial.println("Maze Module Complete!");
//     }
// }

// void setupMaze() {
//     pinMode(cornerLEDLatchPin, OUTPUT);
//     pinMode(cornerLEDCLKPin, OUTPUT);
//     pinMode(cornerLEDDataPin, OUTPUT);

//     servoX.attach(servoXPin);
//     servoY.attach(servoYPin);

//     for (int i = 0; i < 4; i++) {
//         pinMode(irSensorPin[i], INPUT);
//     }

//     calibrateGyro();
//     servoX.write(90); // Initialize servos to center
//     servoY.write(90);
// }

// void loopMaze() {
//     if (morseCodeMistake) {
//         moveServosOppositeGyro();
//     } else if (!mazeModule) {
//         handleMazeLogic();
//     }
// }

// /*
// ================================================================================
//                                 Start Module
// ================================================================================
// */

// // Variables -------------------------------------------------------------------

// // Pins
// const int dataPin = 26; // Pin connected to DS of 74HC595 (serial data input)
// const int latchPin = 25; // Pin connected to ST_CP of 74HC595 (latch pin)
// const int clockPin = 12; // Pin connected to SH_CP of 74HC595 (clock pin)
// const int startButton = 0; // Pin connected to the start button

// bool startGame = false; // Flag to start the game

// int previousStateStartButton = HIGH; // Previous state of the start button

// unsigned long previousTime = 0; // prevoius time
// unsigned long timerStartGameInterval = 1000; // current time
// int timeStartGame = 300; // Time to start the game in seconds

// // Segments: DP, G, F, E, D, C, B, A (Q7 -> Q0)
// byte digits[] = {
//   B01000000, // 0 (common cathode)
//   B01111001, // 1
//   B00100100, // 2
//   B00110000, // 3
//   B00011001, // 4
//   B00010010, // 5
//   B00000010, // 6
//   B01111000, // 7
//   B00000000, // 8
//   B00010000  // 9
// };

// byte digitsDecimal[] = {
//   B00111111, // 0 (common anode)
//   B00000110, // 1
//   B01011011, // 2
//   B01001111, // 3
//   B01100110, // 4
//   B01101101, // 5
//   B01111101, // 6
//   B00000111, // 7
//   B01111111, // 8
//   B01101111  // 9
// };

// byte invertDigits[] = {
//   B00111111, // 0 (common anode)
//   B00000110, // 1
//   B01011011, // 2
//   B01001111, // 3
//   B01100110, // 4
//   B01101101, // 5
//   B01111101, // 6
//   B00000111, // 7
//   B01111111, // 8
//   B01101111  // 9
// };

// // Functions -------------------------------------------------------------------

// void displayMixedSevSegNumber(int number) {
//   int ones = number % 10;         // Rightmost digit (common cathode)
//   int tens = (number / 10) % 10;  // Leftmost digit (common anode)
//   int hundreds = (number / 100) % 10; // Hundreds digit
//   int thousands = (number / 1000) % 10; // Thousands digit

//   digitalWrite(latchPin, LOW);  // Disable latch

//   // Send data to the second digit (common cathode)
//   shiftOut(dataPin, clockPin, MSBFIRST, digits[ones]);

//   // Send data to the first digit (common cathode)
//   shiftOut(dataPin, clockPin, MSBFIRST, digits[tens]);

//   // Send data to the third digit (common anode, inverted)
//   shiftOut(dataPin, clockPin, MSBFIRST, digitsDecimal[hundreds]);

//   shiftOut(dataPin, clockPin, MSBFIRST, invertDigits[thousands]);

//   digitalWrite(latchPin, HIGH); // Enable latch
// }

// void countdownTimer(int time) {
//   if (currentTime - previousTime >= timerStartGameInterval) {
//     previousTime = currentTime; // Update the timer

//     // Decrease the countdown time
//     if (time > 0) {
//       time--;
//     }

//     // Display the countdown in MM:SS format
//     int minutes = time / 60;  // Calculate the minutes
//     int seconds = time % 60;  // Calculate the seconds

//     // Print the countdown to the Serial Monitor
//     if (seconds < 10) {
//       Serial.print(minutes);
//       Serial.print(":0");
//       Serial.println(seconds); // Add a leading zero for single-digit seconds
//     } else {
//       Serial.print(minutes);
//       Serial.print(":");
//       Serial.println(seconds);
//     }
//   }
// }

// void beginStartGame() {
//   if (previousStateStartButton == HIGH && startButton == LOW) {
//     if (currentTime - lastButtonDebounceTime >= debounceDelay) {
//       lastButtonDebounceTime = currentTime;

//       // Start the game
//       startGame = true;
//     }
//   }
// }

// void setupStart() {
//   // Set shift register pins as outputs
//   pinMode(dataPin, OUTPUT);
//   pinMode(latchPin, OUTPUT);
//   pinMode(clockPin, OUTPUT);
// }

// void loopStart() {
//   beginStartGame();
//   if (startGame) {
//     for (int i = timeStartGame; i > 0; i--) {
//       countdownTimer(i);
//       displayMixedSevSegNumber(i);

//       loopMaze();
//     }
//   }
// }

// /*
// ================================================================================
//                         Miniaml setup for the project
// ================================================================================
// */

// void setup() {
//   Serial.begin(9600);

//   setupMorseCodeDisplay();
//   setupMorseCodeLight();
//   setupMaze();
//   // setupDrumpad();

// }

// void loop() {
//   // Update the current time
//   currentTime = millis();

//   loopMorseCodeLight();
//   loopMorseCodeDisplay();

// }