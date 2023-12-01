#include <Arduino.h>
#include <FastLED.h>

#include <numberMasks.h>

// Segment Config
#define DIGIT_SIZE 18
#define TILES 19

#define PORT_SIZE 4
#define PORTS 9

#define NUM_LEDS DIGIT_SIZE * TILES + PORT_SIZE * PORTS

// Output Config
#define DATA_PIN 14

// FASTLED Config
CRGB leds[NUM_LEDS];

const bool* DIGIT_ARRAY[] = {digitZero, digitOne, digitTwo, digitThree, digitFour, digitFive, digitSix, digitSeven, digitEight, digitNine, digitTen, digitEleven, digitTwelve, digitThirteen, digitFourteen, digitFifteen, digitSixteen, digitSeventeen, digitEighteen, digitNineteen};

int defaultColors[] = {CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::FireBrick, CRGB::Gray, CRGB::DarkGreen};
int defaultLayout[] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
int defaultLayoutColors[] = {CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::FireBrick, CRGB::FireBrick, CRGB::FireBrick, CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen};
int defaultPortLayout[] = {0, 1, 0, 1, 1, 1, 0, 1, 0};


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


  fisherYatesShuffle(defaultColors, 5);
  fisherYatesShuffle(defaultPortLayout, 9);
  fisherYatesShuffle(defaultLayout, 18);
  fisherYatesShuffle(defaultLayoutColors, 18);
  Serial.println("Shuffle Complete");

}

void loop() {
  for (int port = 0; port < PORTS; port++) {
    if (defaultPortLayout[port]) {
      for (int i = 0; i < PORT_SIZE; i++) {
        leds[i + (port*PORT_SIZE*2)] = CRGB::White;
      }
    } else {
      leds[0 + (port*PORT_SIZE*2)] = defaultColors[port];
      leds[1 + (port*PORT_SIZE*2)] = CRGB::Black;
      leds[2 + (port*PORT_SIZE*2)] = defaultColors[port];
      leds[3 + (port*PORT_SIZE*2)] = CRGB::White;
      Serial.println("Set LED PORT");
    }

    for (int i = 0; i < PORT_SIZE; i++){
      leds[i + (port*PORT_SIZE*2) + 4] = CRGB::Black;
    }
  }
  FastLED.show();


  for (int segment = 0; segment < TILES; segment++) {
    Serial.print("Tile ");
    Serial.println(segment);

    for (int i = 0; i < DIGIT_SIZE; i++){
      leds[i + (segment*DIGIT_SIZE) + (PORTS*PORT_SIZE*2)] = DIGIT_ARRAY[defaultLayout[segment]][i] ? defaultLayoutColors[segment] : CRGB::Black;
    }
    FastLED.show();
  }

  delay(100000);
}