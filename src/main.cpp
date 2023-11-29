#include <Arduino.h>
#include <FastLED.h>

#include <numberMasks.h>

// Segment Config
#define DIGIT_SIZE 18
#define SEGMENTS 4
#define NUM_LEDS DIGIT_SIZE * SEGMENTS

// Output Config
#define DATA_PIN 14

// FASTLED Config
CRGB leds[NUM_LEDS];

const bool* DIGIT_ARRAY[] = {digitZero, digitOne, digitTwo, digitThree, digitFour, digitFive, digitSix, digitSeven, digitEight, digitNine, digitTen, digitEleven, digitTwelve, digitThirteen, digitFourteen, digitFifteen, digitSixteen, digitSeventeen, digitEighteen, digitNineteen};

int defaultLayout[] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
int defaultLayoutColors[] = {CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen};



// Wheat, Wool, Brick, Ore, Wood, Desert
// Yellow/golden, Light Green, red, light blue/grey, brown/darkgreen, tan/beige
// CRGB COLOR_ARRAY[] = {CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::Red, CRGB::Gray, CRGB::DarkGreen};
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

void loop(){

  fisherYatesShuffle(defaultLayout, 18);
  fisherYatesShuffle(defaultLayoutColors, 18);

  for (int segment = 0; segment < SEGMENTS; segment++) {

    for (int i = 0; i < DIGIT_SIZE; i++){
      leds[i + (segment*DIGIT_SIZE)] = DIGIT_ARRAY[defaultLayout[segment]][i] ? defaultLayoutColors[segment] : CRGB::Black;
    }
    FastLED.show();
  }

  delay(5000);
  
}