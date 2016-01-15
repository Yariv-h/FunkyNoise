/*
         FunkyNoise 1.0
 ----------------              
 A Collection Of 
 Animations 
 And Helper Functions 
 
 for two dimensional led effects
 on the 32x32 SmartMatrix.
 
 Tested on PJRCs Teensy 3.1 @ 96Mhz.
 
 With explicit thanks to Daniel Garcia,
 Mark Kriegsmann and Louis Beaudoin.
 
 Written by Stefan Petrick 2014.
 Extended by Will Tatam
 
 hello(at) stefan-petrick . de
 
 ...
 
 Download the required software first:
 FastLED 3.1
 Arduino IDE 1.0.6
 Teensyduino 1.2
 
 */

#include<FastLED.h>

//---LED SETUP STUFF
#define LED_PIN 2
#define CLOCK_PIN 6
#define COLOR_ORDER BGR

// the size of your matrix
#define kMatrixWidth  15
#define kMatrixHeight 15

// used in FillNoise for central zooming
byte CentreX =  (kMatrixWidth / 2) - 1;
byte CentreY = (kMatrixHeight / 2) - 1;

// set up the framebuffer
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[kMatrixWidth * kMatrixHeight];

// a place to store the color palette
CRGBPalette16 currentPalette;

// can be used for palette rotation
// "colorshift"
byte colorshift;

// The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS 3

uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];

// used for the random based animations
int16_t dx;
int16_t dy;
int16_t dz;
int16_t dsx;
int16_t dsy;  

// a 3dimensional array used to store the calculated 
// values of the different noise planes
uint8_t noise[NUM_LAYERS][kMatrixWidth][kMatrixHeight];

// used for the color histogramm
uint16_t values[256];

uint8_t noisesmoothing;

// everything for the button + menu handling
int button1;
int button2;
int button3;
byte mode;
byte pgm;
byte spd;
byte brightness;
byte red_level;
byte green_level;
byte blue_level;


void setup() {

  // enable debugging info output
  Serial.begin(115200);

  // add the SmartMatrix controller
  FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  // switch dithering off to avoid flicking at low fps
  FastLED.setDither(0);


  // fill all animation variables with valid values to
  // allow straight forward animation programming
  BasicVariablesSetup();

  // the pins for the 3 buttons of my custom interface
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  SetupMatrixText();
}


// basically beatsin16 with an additional phase

uint16_t beatsin(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, byte phase = 0)
{
  uint16_t beat = beat16( beats_per_minute);
  uint16_t beatsin = (sin16( beat+(phase*256)) + 32768);
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( beatsin, rangewidth);
  uint16_t result = lowest + scaledbeat;
  return result;
}

void loop() {

  for(int p=0; p<= 19; p++) {
    pgm=p;
    for(int i=0; i < 500; i++) {
      RunAnimationDependingOnPgm();
      ShowFrame();
    }
  }

} 






















