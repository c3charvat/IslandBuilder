#include <Arduino.h>
#include <FastLED.h>

#include <numberMasks.h>

// Segment Config
#define DIGIT_SIZE 18
#define NUM_DIGITS 1
#define SEGMENTS 1
#define NUM_LEDS DIGIT_SIZE * NUM_DIGITS * SEGMENTS

// Output Config
#define DATA_PIN 14

// FASTLED Config
CRGB leds[NUM_LEDS];

const bool* DIGIT_ARRAY[] = {digitZero, digitOne, digitTwo, digitThree, digitFour, digitFive, digitSix, digitSeven, digitEight, digitNine, digitTen, digitEleven, digitTwelve, digitThirteen, digitFourteen, digitFifteen, digitSixteen, digitSeventeen, digitEighteen, digitNineteen};

int testArray[] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};

// Wheat, Wool, Brick, Ore, Wood, Desert
// Yellow/golden, Light Green, red, light blue/grey, brown/darkgreen, tan/beige
CRGB COLOR_ARRAY[] = {CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::Red, CRGB::Gray, CRGB::DarkGreen};
// CRGB TEST_COLOR_ARRAY[] = {CRGB::Brown, CRGB::DarkGreen};


/**
 * Shuffles an input array. I think. Pulled from chatGPT. Should work I think.
 * Inputs: Array to be shuffled, Length of array.
 * ToDo: Remove array length requirement.
*/
void fisherYatesShuffle(int arr[], int length) {
  for (int i = length - 1; i > 0; i--) {
    int j = random(0, i + 1); // Generate a random index between 0 and i
    // Swap arr[i] and arr[j]
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void setup(){
    Serial.begin(115200);
    Serial.println("Begin");
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(10);  // Adjust this value to change overall brightness
}

int digitCount = 0;
void loop(){
  for (int i = 0; i < DIGIT_SIZE; i++){
    leds[i] = DIGIT_ARRAY[digitCount % 20][i] ? CRGB::DarkGoldenrod : CRGB::Black;
  }
  FastLED.show();
  digitCount++;
  delay(1000);
}