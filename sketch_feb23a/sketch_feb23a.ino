#include <FastLED.h>

// MAIN SETTINGS
#define LED_PIN             5
#define NUM_LEDS            58
#define BRIGHTNESS          64
#define PWM_PIN             7
#define LED_TYPE            WS2811
#define COLOR_ORDER         GRB
#define UPDATES_PER_SECOND  100

// L WING LED
#define WING_R_LED_1_START  0
#define WING_R_LED_2_OFFSET 5
#define WING_R_LED_LENGTH   12

// R WING LED
#define WING_L_LED_1_START  29
#define WING_L_LED_2_OFFSET 5
#define WING_L_LED_LENGTH   12

// PWM LEVELS (COLORS + TURN SIGNALS) ====
// E0 ====================================
// COLOR 1 (D0) PWM VALUES
#define CLR1_R_LO   1915
#define CLR1_M_LO   1930
#define CLR1_M_HI   1950
#define CLR1_L_HI   1980
// COLOR 2 (D1) PWM VALUES
#define CLR2_R_LO   1520
#define CLR2_M_LO   1558
#define CLR2_M_HI   1585
#define CLR2_L_HI   1610
// COLOR 3 (D2) PWM VALUES
#define CLR3_R_LO   1140
#define CLR3_M_LO   1200
#define CLR3_M_HI   1218
#define CLR3_L_HI   1260
// E1 ====================================
// COLOR 4 (D0) PWM VALUES
#define CLR4_R_LO   1805
#define CLR4_M_LO   1828
#define CLR4_M_HI   1842
#define CLR4_L_HI   1850
// COLOR 5 (D1) PWM VALUES
#define CLR5_R_LO   1430
#define CLR5_M_LO   1460
#define CLR5_M_HI   1475
#define CLR5_L_HI   1495
// COLOR 6 (D2) PWM VALUES
#define CLR6_R_LO   1070
#define CLR6_M_LO   1090
#define CLR6_M_HI   1105
#define CLR6_L_HI   1140
// E2 ====================================
// COLOR 7 (D0) PWM VALUES
#define CLR7_R_LO   1695
#define CLR7_M_LO   1720
#define CLR7_M_HI   1730
#define CLR7_L_HI   1750
// COLOR 8 (D1) PWM VALUES
#define CLR8_R_LO   1330
#define CLR8_M_LO   1350
#define CLR8_M_HI   1370
#define CLR8_L_HI   1390
// COLOR 9 (D2) PWM VALUES
#define CLR9_R_LO   965
#define CLR9_M_LO   985
#define CLR9_M_HI   999
#define CLR9_L_HI   1025
// PWM LEVELS EOF ========================

CRGB leds[NUM_LEDS];

int c = 1;
int pwm_value;

int CLR1_R_HI = CLR1_M_LO + 1;
int CLR1_L_LO = CLR1_M_HI - 1;
int CLR2_R_HI = CLR2_M_LO + 1;
int CLR2_L_LO = CLR2_M_HI - 1;
int CLR3_R_HI = CLR3_M_LO + 1;
int CLR3_L_LO = CLR3_M_HI - 1;

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int brightness = 0; 


void setup() {

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    
  
    delay( 1000 ); // power-up safety delay
    // FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    
    pinMode(PWM_PIN, INPUT);
    Serial.begin(115200);
    
    //currentPalette = RainbowColors_p;
    //currentBlending = LINEARBLEND;
}


void loop()
{
  pwm_value = pulseIn(PWM_PIN, HIGH);
  Serial.println(pwm_value);
  if ((pwm_value >= CLR1_R_LO and pwm_value <= CLR1_R_HI)
    or (pwm_value >= CLR2_R_LO and pwm_value <= CLR2_R_HI)
    or (pwm_value >= CLR3_R_LO and pwm_value <= CLR3_R_HI)) {
    TurnSignal_R();
  }
  else if ((pwm_value >= CLR1_L_LO and pwm_value <= CLR1_L_HI)
    or (pwm_value >= CLR2_L_LO and pwm_value <= CLR2_L_HI)
    or (pwm_value >= CLR3_L_LO and pwm_value <= CLR3_L_HI)) {
    TurnSignal_L();
  }
  else if (pwm_value >= CLR1_M_LO and pwm_value <= CLR1_M_HI) {
    currentPalette = RainbowStripeColors_p;
    currentBlending = NOBLEND;
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors( startIndex );
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else if (pwm_value >= CLR2_M_LO and pwm_value <= CLR2_M_HI) {
    currentPalette = CloudColors_p;           currentBlending = LINEARBLEND;
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors( startIndex );
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else if (pwm_value >= CLR3_M_LO and pwm_value <= CLR3_M_HI) {
    SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND;
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors( startIndex );
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
  else {
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND; 
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors( startIndex );
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }

  
  
    /*
      for(int i = 0; i < NUM_LEDS; i++ )
       {
       if (pwm_value > 1700) {
          leds[i].setRGB(125,0,255);  // Set Color HERE!!!
       }
       else if (pwm_value > 1500) {
          leds[i].setRGB(255,125,250);  // Set Color HERE!!!
       }
       else {
          leds[i].setRGB(255,255,0);  // Set Color HERE!!!
       }
       leds[i].fadeLightBy(brightness);
       FastLED.show();
       delay(80);  
      }

      delay(500);
      Serial.println(c);

       
      pwm_value = pulseIn(PWM_PIN, HIGH);
      Serial.println(pwm_value);

            for(int i = 0; i < NUM_LEDS; i++ )
       {
       leds[i].setRGB(0,0,0);  // Set Color HERE!!!
       leds[i].fadeLightBy(brightness);
       FastLED.show(); 
       delay(80);  
      }
    
    
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    /*
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    */
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;

        if( secondHand < 30 ) { SetupZbrTestPalette();    currentBlending = LINEARBLEND;  }
        else {  currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND;  }

        /*
        
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }

        */
    }
}

void TurnSignal_R() 
{  
  for(int i = 0; i < WING_R_LED_LENGTH; i++) {
      leds[i].setRGB(255,255,0);
      leds[i].fadeLightBy(brightness);
      leds[(WING_R_LED_LENGTH*2) + WING_R_LED_2_OFFSET - (i + 1)].setRGB(255,255,0);
      leds[(WING_R_LED_LENGTH*2) + WING_R_LED_2_OFFSET - (i + 1)].fadeLightBy(brightness);
      FastLED.show();
      delay(10); 
    }
    delay(80);
    for(int i = WING_R_LED_1_START; i < WING_R_LED_LENGTH; i++) {
      leds[i].setRGB(0,0,0);
      leds[i].fadeLightBy(brightness);
      leds[(WING_R_LED_LENGTH*2) + WING_R_LED_2_OFFSET - (i + 1)].setRGB(0,0,0);
      leds[(WING_R_LED_LENGTH*2) + WING_R_LED_2_OFFSET - (i + 1)].fadeLightBy(brightness);
      FastLED.show();
      delay(10); 
    }
    delay(80);
}

void TurnSignal_L() 
{
  for(int i = 0; i < WING_L_LED_LENGTH; i++) {
      leds[i + WING_L_LED_1_START].setRGB(255,255,0);
      leds[i + WING_L_LED_1_START].fadeLightBy(brightness);
      leds[(WING_L_LED_LENGTH*2) + WING_L_LED_2_OFFSET - (i + 1) + WING_L_LED_1_START].setRGB(255,255,0);
      leds[(WING_L_LED_LENGTH*2) + WING_L_LED_2_OFFSET - (i + 1) + WING_L_LED_1_START].fadeLightBy(brightness);
      FastLED.show();
      delay(10); 
    }
    delay(80);
    for(int i = 0; i < WING_L_LED_LENGTH; i++) {
      leds[i + WING_L_LED_1_START].setRGB(0,0,0);
      leds[i + WING_L_LED_1_START].fadeLightBy(brightness);
      leds[(WING_L_LED_LENGTH*2) + WING_L_LED_2_OFFSET - (i + 1) + WING_L_LED_1_START].setRGB(0,0,0);
      leds[(WING_L_LED_LENGTH*2) + WING_L_LED_2_OFFSET - (i + 1) + WING_L_LED_1_START].fadeLightBy(brightness);
      FastLED.show();
      delay(10); 
    }
    delay(80);
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

void SetupZbrTestPalette()
{
    CRGB red  = CHSV( HUE_PINK, 255, 255);
    CRGB black  = CRGB::Black;

    currentPalette = CRGBPalette16(
                               red,  black,  red,  black,
                               red,  black,  red,  black,
                               red,  black,  red,  black,
                               red,  black,  red,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
