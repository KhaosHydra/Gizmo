// #include <Arduino.h>
// #include <Wire.h>
// #include <map>

// // Module connection pins (Digital Pins)
// const int MorseCode_LED = 33; // LED for Morse code

// // Global Variables
// int DotDuration = 1000;
// int DashDuration = 3000;
// int BetweenElementsDuration = 1000;
// int BetweenDigitsDuration = 3000;
// int BetweenSequenceDuration = 7000;

// // Secret Morse code Passcode
// int NumberPasscode;
// String EncryptedPasscode;

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

// // Function to blink the LED according to Morse code
// void blinkLEDMorseCode(const String &morseCode)
// {
//   for (char c : morseCode)
//   {
//     // Dot Duration
//     if (c == '.')
//     {
//       digitalWrite(MorseCode_LED, HIGH);
//       delay(DotDuration);
//       digitalWrite(MorseCode_LED, LOW);
//       delay(BetweenElementsDuration);
//     }

//     // Dash Duration
//     else if (c == '-')
//     {
//       digitalWrite(MorseCode_LED, HIGH);
//       delay(DashDuration);
//       digitalWrite(MorseCode_LED, LOW);
//       delay(BetweenElementsDuration);
//     }

//     // Gap Duration
//     else
//     {
//       delay(BetweenDigitsDuration);
//     }
//   }
// }

// // Function to setup the code
// void setupMorseCode()
// {
//   // Start the serial communication
//   Serial.begin(9600);

//   // Setting I/O pins
//   pinMode(MorseCode_LED, OUTPUT);

//   // Generate random number between 1000 and 9999
//   NumberPasscode = 1000 + esp_random() % 9000;

//   // Convert number to Morse code
//   EncryptedPasscode = numberToMorse(NumberPasscode);
// }

// // Function to loop the code
// void loopMorseCode ()
// {
//   // Debugging - Print the secret passcode
//   static bool passcodePrinted = false;
//   if (!passcodePrinted) {
//     Serial.println("Secret Passcode: " + String(NumberPasscode));
//     passcodePrinted = true;
//   }

//   // Blink Morse code
//   blinkLEDMorseCode(EncryptedPasscode);
//   delay(BetweenSequenceDuration);
// }