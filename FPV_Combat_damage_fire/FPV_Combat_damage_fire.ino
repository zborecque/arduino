#include <FastLED.h>

// MAIN SETTINGS
#define NUM_LEDS                      60
#define BRIGHTNESS                    64
#define LED_PIN                       5
#define PWM_SWTCH_PIN                 7
#define PWM_FPVC_PIN                  9
#define LED_TYPE                      WS2811
#define COLOR_ORDER                   GRB
#define UPDATES_PER_SECOND            100
#define FRAMES_PER_SECOND             60

// PWM VALUESx
#define PWM_SWTCH_ALL_OFF_HI          1200
#define PWM_SWTCH_FIRE_DMG_LO         1400
#define PWM_SWTCH_FIRE_DMG_HI         1600
#define PWM_CUSTOM_ANIM_LO            1750
#define PWM_FPVC_NOHIT_LO             1450
#define PWM_FPVC_NOHIT_HI             1650

// LED SECTIONS
#define LEDSECT_WING_R_TOP_START      34
#define LEDSECT_WING_R_TOP_END        43
#define LEDSECT_WING_L_TOP_START      46
#define LEDSECT_WING_L_TOP_END        55
#define LEDSECT_WING_R_BOTT_START     24
#define LEDSECT_WING_R_BOTT_END       33
#define LEDSECT_WING_L_BOTT_START     12
#define LEDSECT_WING_L_BOTT_END       21
#define LEDSECT_LIGHT_R_TOP_START     0
#define LEDSECT_LIGHT_R_TOP_END       3
#define LEDSECT_LIGHT_L_TOP_START     56
#define LEDSECT_LIGHT_L_TOP_END       59
#define LEDSECT_LIGHT_R_BOTT_START    4
#define LEDSECT_LIGHT_R_BOTT_END      7
#define LEDSECT_LIGHT_L_BOTT_START    8
#define LEDSECT_LIGHT_L_BOTT_END      11
#define LEDSECT_LIGHT_MID_TOP_START   44
#define LEDSECT_LIGHT_MID_TOP_END     45
#define LEDSECT_LIGHT_MID_BOTT_START  22
#define LEDSECT_LIGHT_MID_BOTT_END    23

bool gReverseDirection = false;

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

// LED SECTIONS
// TODO 

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
int hits_taken = 0;
int pwm_switch_value;
int pwm_fpvc_value;

int CLR1_R_HI = CLR1_M_LO + 1;
int CLR1_L_LO = CLR1_M_HI - 1;
int CLR2_R_HI = CLR2_M_LO + 1;
int CLR2_L_LO = CLR2_M_HI - 1;
int CLR3_R_HI = CLR3_M_LO + 1;
int CLR3_L_LO = CLR3_M_HI - 1;








// == FIRE EXAMPLE =============================================




// Fire2012 with programmable Color Palette
//
// This code is the same fire simulation as the original "Fire2012",
// but each heat cell's temperature is translated to color through a FastLED
// programmable color palette, instead of through the "HeatColor(...)" function.
//
// Four different static color palettes are provided here, plus one dynamic one.
// 
// The three static ones are: 
//   1. the FastLED built-in HeatColors_p -- this is the default, and it looks
//      pretty much exactly like the original Fire2012.
//
//  To use any of the other palettes below, just "uncomment" the corresponding code.
//
//   2. a gradient from black to red to yellow to white, which is
//      visually similar to the HeatColors_p, and helps to illustrate
//      what the 'heat colors' palette is actually doing,
//   3. a similar gradient, but in blue colors rather than red ones,
//      i.e. from black to blue to aqua to white, which results in
//      an "icy blue" fire effect,
//   4. a simplified three-step gradient, from black to red to white, just to show
//      that these gradients need not have four components; two or
//      three are possible, too, even if they don't look quite as nice for fire.
//
// The dynamic palette shows how you can change the basic 'hue' of the
// color palette every time through the loop, producing "rainbow fire".

CRGBPalette16 gPal;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {

  delay(3000); // sanity delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
  gPal = HeatColors_p;
  
  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  
  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  
  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  pinMode(PWM_SWTCH_PIN, INPUT);
  pinMode(PWM_FPVC_PIN, INPUT);


  Serial.begin(115200);
  
  //currentPalette = RainbowColors_p;
  //currentBlending = LINEARBLEND;

}

void loop()
{

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random());

  // Set up PWM readers
  pwm_switch_value = pulseIn(PWM_SWTCH_PIN, HIGH);
  pwm_fpvc_value = pulseIn(PWM_FPVC_PIN, HIGH);

  Serial.println(pwm_switch_value + " | " + pwm_fpvc_value);

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

  Fire2012WithPalette(); // run simulation frame, using palette colors
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  /*
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
  */

}

void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}


// ==============================================================================













 

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
