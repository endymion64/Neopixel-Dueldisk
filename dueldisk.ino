#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// GLOBAL VARIABLES --------------------------------------------------------

// Maximum allowed brightness value for the LEDs 
// Value can range from 0 until 255
#define _MAXB_ 150
// Minimum allowed brightness value
// Must be SMALLER than MAXB
// Value may range from 0 until 255
#define _MINB_ 90

// This bizarre construct isn't Arduino code in the conventional sense.
// It exploits features of GCC's preprocessor to generate a PROGMEM
// table (in flash memory) holding an 8-bit unsigned cosine 
// with image range [MINB-MAXB] over a period of 255
const int _SBASE_ = __COUNTER__ + 1; // Index of 1st __COUNTER__ ref below
const float _AMP_ = (_MAXB_ - _MINB_) / 2;
const float _CENTER_ = (_MAXB_ + _MINB_) / 2;
#define _S1_ (cos((__COUNTER__ - _SBASE_) / 127.5 * M_PI)) * _AMP_ + _CENTER_,
#define _S2_ _S1_ _S1_ _S1_ _S1_ _S1_ _S1_ _S1_ _S1_ // Expands to 8 items
#define _S3_ _S2_ _S2_ _S2_ _S2_ _S2_ _S2_ _S2_ _S2_ // Expands to 64 items
const uint8_t PROGMEM cosineTable[] = { _S3_ _S3_ _S3_ _S3_ }; // 256 items
 
// Similar to above, but for an 8-bit gamma-correction table.
#define _GAMMA_ 2.8
const int _GBASE_ = __COUNTER__ + 1; // Index of 1st __COUNTER__ ref below
#define _G1_ pow((__COUNTER__ - _GBASE_) / 255.0, _GAMMA_) * 255.0 + 0.5,
#define _G2_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ // Expands to 8 items
#define _G3_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ // Expands to 64 items
const uint8_t PROGMEM gammaTable[] = { _G3_ _G3_ _G3_ _G3_ }; // 256 items

// The pins on the MCU which connect to NeoPixels
#define NUMPINS   6
int pins[NUMPINS] = {3,5,8,9,11,12};

// The number of NeoPixel LEDs attached to the aforementioned pins of the MCU
int numPixels[NUMPINS] = {10,4,5,4,4,5};

// Declare array for the NeoPixel strips
Adafruit_NeoPixel strips[NUMPINS];

// Default color for the strips
#define R 255
#define G 0
#define B 121

// FUNCTIONS --------------------------------------------------------

// Convert separate R,G,B into packed 32-bit RGB color, altered by a brightness.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

void setup() {
  // This is for Pro Trinket 5V 16MHz, you can remove these lines if you are not using a Pro Trinket
#if defined (__AVR_ATmega328P__)
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
  // Sets the clock from 16 MHz to 8 MHz
  if(F_CPU == 8000000) clock_prescale_set(clock_div_2);
#endif
  // End of trinket special code

  //Create all the NeoPixel objects
  for(int i=0;i<NUMPINS;i++){
    strips[i] = Adafruit_NeoPixel(numPixels[i],pins[i],NEO_GRB + NEO_KHZ800);
    strips[i].begin();
    strips[i].show();
  }
}

void loop() {
  mode_cosine_gamma(); 
}

// Perceptually gamma-corrected cosine wave.  Sometimes you
// don't need things going to peak brightness all the time.
void mode_cosine_gamma() {
  uint32_t t = millis() / 8;
  for(uint8_t h=0; h<NUMPINS; h++){
    for(uint8_t i=0; i<strips[h].numPixels(); i++) {
      uint8_t j = i + (i > 4);
      uint8_t k = pgm_read_byte(&cosineTable[(t - j * strips[h].numPixels() ) & 0xFF]);
      k = pgm_read_byte(&gammaTable[k]);
      strips[h].setPixelColor(i, color(R,G,B,k));
    }
    strips[h].show();
  }
}

/*
// Filling the dots one after another with a color in parallel over all strips
void  colorWipe(Adafruit_NeoPixel strips[], int len, uint32_t c, uint8_t wait){
  uint8_t maxNumLeds=10;
  for(uint8_t j=0;j<maxNumLeds;j++){
    for(int i=0;i<len;i++){
      if(j<strips[i].numPixels()){
         uint8_t k = pgm_read_byte(&cosineTable[_MAXB_ & 0xFF]) / 2;
         k = pgm_read_byte(&gammaTable[k]);
         strips[i].setPixelColor(j,k * c);
      } 
    }
    for(int i=0;i<len;i++){
      if(j<strips[i].numPixels()){
        strips[i].show();
      } 
    }
    delay(wait);
  }
}
*/
