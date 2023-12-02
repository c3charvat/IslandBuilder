#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>

#include <numberMasks.h>

// Hex Tile Config
#define DIGIT_SIZE 18
#define TILES 18

// Port Config
#define PORT_SIZE 4
#define PORTS 9

// Total Number of LEDS
#define NUM_LEDS DIGIT_SIZE * (TILES + 1) + PORT_SIZE * PORTS * 2

// Output Config
#define DATA_PIN 14

// FASTLED Config
CRGB leds[NUM_LEDS];

const bool* DIGIT_ARRAY[] = {digitZero, digitOne, digitTwo, digitThree, digitFour, digitFive, digitSix, digitSeven, digitEight, digitNine, digitTen, digitEleven, digitTwelve, digitThirteen, digitFourteen, digitFifteen, digitSixteen, digitSeventeen, digitEighteen, digitNineteen};

// Wheat, Wool, Brick, Ore, Wood, Desert
// Yellow/golden, Light Green, red, light blue/grey, brown/darkgreen, tan/beige
// CRGB COLOR_ARRAY[] = {CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::Red, CRGB::Gray, CRGB::DarkGreen};
// CRGB TEST_COLOR_ARRAY[] = {CRGB::Brown, CRGB::DarkGreen};

int defaultColors[] = {CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::FireBrick, CRGB::Gray, CRGB::DarkGreen};
int defaultLayout[] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
int defaultLayoutColors[] = {CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::DarkGoldenrod, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::GreenYellow, CRGB::FireBrick, CRGB::FireBrick, CRGB::FireBrick, CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen};
int defaultPortLayout[] = {0, 1, 0, 1, 1, 1, 0, 1, 0};


uint8_t ambientLightData[10];
uint8_t ambientLightIndex = 0;
int32_t ambientLightTotal = 0;
uint8_t ambientLightAverage = 0;

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

void initLightSensor() {
  Wire.beginTransmission(0x29);
  Wire.write(0x80);
  Wire.write(0x3);
  Wire.endTransmission();
}

uint8_t readAmbientLightData() {
  Wire.beginTransmission(0x29);
  Wire.write(0b10010100);
  Wire.endTransmission();
  Wire.requestFrom(0x29, 2);

  // Read the data from the BH1730
  byte highByte = Wire.read();
  byte lowByte = Wire.read();

  // Combine the high and low bytes to get the ambient light level
  int lightLevel = (highByte << 8) | lowByte;

  // return the ambient light level
  return(map(lightLevel, 10, 65535, 10, 50) );
}

void setup(){
  Serial.begin(115200);
  Serial.println("Begin");
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();

  // Begin I2C on pins 39, 40.
  Wire.begin(9, 10);
  initLightSensor();

  FastLED.setBrightness(readAmbientLightData());  // Start really dim.

  Serial.println(FastLED.getBrightness());

  fisherYatesShuffle(defaultColors, 5);
  fisherYatesShuffle(defaultPortLayout, 9);
  fisherYatesShuffle(defaultLayout, 18);
  fisherYatesShuffle(defaultLayoutColors, 18);

  int portColorIndex = 0;
  
  // Ports. Every other port is used on the board. The odd ports are set to black, and the evens follow the shuffled arrays.
  for (int port = 0; port < PORTS; port++) {
    if (defaultPortLayout[port]) {
      // Handle the 3:1 ports
      for (int i = 0; i < PORT_SIZE; i++) {
        leds[i + (port*PORT_SIZE*2)] = CRGB::White;
      }
    } else {
      // Handle the 2:1 ports.
      leds[0 + (port*PORT_SIZE*2)] = defaultColors[portColorIndex];
      leds[1 + (port*PORT_SIZE*2)] = CRGB::Black;
      leds[2 + (port*PORT_SIZE*2)] = defaultColors[portColorIndex];
      leds[3 + (port*PORT_SIZE*2)] = CRGB::White;
      portColorIndex++;
    }

    for (int i = 0; i < PORT_SIZE; i++){
      leds[i + (port*PORT_SIZE*2) + 4] = CRGB::Black;
    }
  }
  Serial.println("Completed Ports");


  int offset = 0;
  int desertTile = random(0, 18);

  for (int segment = 0; segment < TILES; segment++) {
    if (segment == desertTile) {
      for (int i = 0; i < DIGIT_SIZE; i++){
        leds[i + (segment*DIGIT_SIZE) + (PORTS*PORT_SIZE*2)] = digitDash[i] ? CRGB::Tan : CRGB::Black;
      }
      offset = DIGIT_SIZE;
    }

    for (int i = 0; i < DIGIT_SIZE; i++) {
      leds[i + (segment*DIGIT_SIZE) + (PORTS*PORT_SIZE*2) + offset] = DIGIT_ARRAY[defaultLayout[segment]][i] ? defaultLayoutColors[segment] : CRGB::Black;
    }
  }

  FastLED.show();
  Serial.println("Completed Board Setup");
}

void loop() {

  // Non-blocking function occurs every second to refresh the LEDs. Makes sure nothing gets bugged during play.
  if (millis() % 1000 == 0) {
    FastLED.show();
  }

  // Autobrightness causes a feedback loop. I went too hard into aesthetics. Oh well. Manual brightess control it is. Probably for the best tbh. Could be used to set at the start of the game though.... Let's run with that for now.
  // // Autobrightness. This should be a moving average of the ambient light. Updates every 10ms.
  // uint8_t ambientLight = readAmbientLightData();
  // ambientLightTotal = ambientLightTotal - ambientLightData[ambientLightIndex] + ambientLight;
  // ambientLightData[ambientLightIndex] = ambientLight;
  // ambientLightIndex = (ambientLightIndex + 1) % 10;
  // ambientLightAverage = ambientLightTotal / 10;
  // if (millis() % 10 == 0){
  //   FastLED.setBrightness((readAmbientLightData() + readAmbientLightData() + readAmbientLightData()) / 3);
  //   FastLED.show();
  // }
}