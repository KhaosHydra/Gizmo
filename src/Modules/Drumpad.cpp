// #include <Arduino.h>
// #include <Wire.h>
// #include <math.h> // For sine wave calculation

// #define PIEZO_PIN 36        // Analog input pin for piezo microphone
// #define SPEAKER_PIN1 26     // First DAC pin
// #define SPEAKER_PIN2 25     // Second DAC pin (optional)
// #define SAMPLE_RATE 44100     // Sample rate for playback
// #define SMOOTHING_FACTOR 0.9 // Low-pass filter smoothing factor (0 to 1)
// #define GAIN 3              // Amplification factor for piezo signal
// #define THRESHOLD 100       // Minimum signal to trigger sound

// // Variables for signal processing
// float smoothedValue = 0;    // Stores the smoothed signal value
// int maxValue = 1;           // Maximum value for normalization

// void setupDrumpad() {
//   Serial.begin(9600);
//   Serial.println("Improved Piezo Microphone to Speaker Test...");
// }

// void loopDrumpad() {
//   // Step 1: Read the raw analog signal from the piezo
//   int piezoValue = analogRead(PIEZO_PIN);
//   if (piezoValue > maxValue) maxValue = piezoValue; // Update max value dynamically

//   // Step 2: Normalize the signal
//   int normalizedValue = (piezoValue * 4095) / maxValue;

//   // Step 3: Apply Gain
//   int amplifiedValue = normalizedValue * GAIN;
//   amplifiedValue = constrain(amplifiedValue, 0, 4095);

//   // Step 4: Apply Low-Pass Filtering
//   smoothedValue = (SMOOTHING_FACTOR * smoothedValue) + ((1 - SMOOTHING_FACTOR) * amplifiedValue);

//   // Step 5: Apply Threshold Check
//   if (smoothedValue > THRESHOLD) {
//     // Step 6: Map the smoothed value to DAC range (0-255)
//     int outputValue = map((int)smoothedValue, THRESHOLD, 4095, 0, 255);

//     // Step 7: Output the processed signal to both DAC channels
//     dacWrite(SPEAKER_PIN1, outputValue);
//     dacWrite(SPEAKER_PIN2, outputValue); // Optional: Dual DAC output for louder sound
//   } else {
//     // Silence the speaker if below threshold
//     dacWrite(SPEAKER_PIN1, 0);
//     dacWrite(SPEAKER_PIN2, 0);
//   }

//   // Step 8: Delay to maintain sample rate
//   delayMicroseconds(1000000 / SAMPLE_RATE); // Adjust for smooth playback
// }



// /*
// #################################################################################
// #   This is part 2 of the code for the "Piezo Microphone to Speaker" project.   #
// #################################################################################
// */

// #include <Arduino.h>
// #include <Wire.h>
// #include <math.h>       // For sine wave calculation
// #include <Trill.h>      // For Trill sensor

// // Pin Definitions
// #define PIEZO_PIN 36        // Analog input pin for piezo microphone
// #define SPEAKER_PIN1 26     // First DAC pin
// #define SPEAKER_PIN2 25     // Second DAC pin (optional)
// #define TRILL_SDA 21        // I2C SDA pin
// #define TRILL_SCL 22        // I2C SCL pin
// #define SAMPLE_RATE 44100   // Sample rate for playback (44.1 kHz)
// #define SMOOTHING_FACTOR 0.9 // Low-pass filter smoothing factor (0 to 1)
// #define GAIN 3              // Amplification factor for piezo signal
// #define DEFAULT_FREQUENCY 440 // Default frequency (Hz) when no touch is detected
// #define MIN_FREQUENCY 200   // Minimum frequency for touch input
// #define MAX_FREQUENCY 1000  // Maximum frequency for touch input

// // Initialize Trill sensor
// Trill trill;

// // Variables for signal processing
// // float smoothedValue = 0;    // Stores the smoothed signal value
// // int maxValue = 1;           // Maximum value for normalization
// float currentFrequency = DEFAULT_FREQUENCY; // Default frequency


// // Function to generate a sine wave with a specific frequency and volume
// void generateSineWave(int volume, float frequency) {
//   static float phase = 0.0; // Phase of the sine wave
//   float phaseIncrement = TWO_PI * frequency / SAMPLE_RATE;

//   // Calculate the sine wave sample
//   int sample = volume * (sin(phase) + 1); // Generate sine wave sample (0-255)
//   dacWrite(SPEAKER_PIN1, sample);        // Output to first DAC pin
//   dacWrite(SPEAKER_PIN2, sample);        // Optional: Dual DAC output for louder sound

//   // Increment phase and wrap around
//   phase += phaseIncrement;
//   if (phase >= TWO_PI) {
//     phase -= TWO_PI;
//   }
// }


// void setupDrumpad2() {
//   // Initialize serial communication
//   Serial.begin(9600);
//   Serial.println("Piezo + Trill Speaker Test...");

//   // Initialize I2C communication for Trill
//   Wire.begin(TRILL_SDA, TRILL_SCL);

//   // Initialize Trill sensor (Trill Craft in this example)
//   if (trill.setup(Trill::TRILL_CRAFT) != 0) {
//     Serial.println("Failed to initialize Trill sensor!");
//     while (1); // Halt the program if initialization fails
//   }
//   Serial.println("Trill sensor initialized successfully!");
// }

// void loopDrumpad2() {
//   // Step 1: Read and process the piezo microphone signal
//   int piezoValue = analogRead(PIEZO_PIN);
//   if (piezoValue > maxValue) maxValue = piezoValue; // Update max value

//   // Normalize the signal
//   int normalizedValue = (piezoValue * 4095) / maxValue;

//   // Apply Gain
//   int amplifiedValue = normalizedValue * GAIN;
//   amplifiedValue = constrain(amplifiedValue, 0, 4095);

//   // Apply Low-Pass Filtering
//   smoothedValue = (SMOOTHING_FACTOR * smoothedValue) + ((1 - SMOOTHING_FACTOR) * amplifiedValue);

//   // Map the smoothed value to DAC range (0-255)
//   int outputValue = map((int)smoothedValue, 0, 4095, 0, 255);

//   // Step 2: Read and process Trill sensor data
//   trill.read(); // Read data from Trill sensor

//   if (trill.num_touches > 0) {
//     // If a touch is detected, map touch location to frequency
//     int touchLocation = trill.touchLocation(0); // Get the location of the first touch
//     currentFrequency = map(touchLocation, 0, 32767, MIN_FREQUENCY, MAX_FREQUENCY); // Map touch to frequency range
//     Serial.print("Trill Frequency: ");
//     Serial.println(currentFrequency);
//   } else {
//     // If no touch is detected, revert to the default frequency
//     currentFrequency = DEFAULT_FREQUENCY;
//   }

//   // Step 3: Generate and output a sine wave to the speaker
//   generateSineWave(outputValue, currentFrequency);

//   // Maintain sample rate
//   delayMicroseconds(1000000 / SAMPLE_RATE);
// }