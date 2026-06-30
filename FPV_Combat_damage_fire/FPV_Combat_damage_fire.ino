#include <FastLED.h>

// MAIN SETTINGS =====================================
#define NUM_LEDS                      60
#define BRIGHTNESS                    240
#define LED_PIN                       5
#define PWM_PIN                       7
#define LED_TYPE                      WS2811
#define COLOR_ORDER                   GRB
#define UPDATES_PER_SECOND            90

// PWM VALUES ========================================
#define PWM_RAINBOW                   1000
#define PWM_POLICE                    1150
#define PWM_DAMAGE_MODE               1400
#define PWM_HIT_DETECTION             1700

// LED LAMPS SECTIONS ================================
// TOP LAMPS
#define LAMP_TOP_R_START              0
#define LAMP_TOP_R_END                3
#define LAMP_TOP_L_START              56
#define LAMP_TOP_L_END                59
// BOTTOM LAMPS
#define LAMP_BOTTOM_R_START           4
#define LAMP_BOTTOM_R_END             7
#define LAMP_BOTTOM_L_START           8
#define LAMP_BOTTOM_L_END             11
// FRONT LAMP
#define LAMP_FRONT_TOP_START          44
#define LAMP_FRONT_TOP_END            45
#define LAMP_FRONT_BOTTOM_START       22
#define LAMP_FRONT_BOTTON_END         23

// LED FIRE SECTIONS =================================
#define FIRE_TOP_R_START              44
#define FIRE_TOP_R_END                34
#define FIRE_TOP_L_START              45
#define FIRE_TOP_L_END                55
#define FIRE_BOTTOM_R_START           23
#define FIRE_BOTTOM_R_END             33
#define FIRE_BOTTOM_L_START           22
#define FIRE_BOTTOM_L_END             12
#define FIRE_TOPMID_R_START           0
#define LAMP_TOPMID_R_END             3
#define LAMP_TOPMID_L_START           56
#define LAMP_TOPMID_L_END             59
#define LAMP_BOTMID_R_START           7
#define LAMP_BOTMID_R_END             4
#define LAMP_BOTMID_L_START           8
#define LAMP_BOTMID_L_END             11

// FIRE ANIMATION PARAMETERS ==========================
#define COOLING                       55
#define SPARKING                      120
#define FIRE_TIMEOUT                  3000

// POLICE BEACON ANIMATION PARAMETERS
#define POLICE_BLINKS                 32

// ====================================================

CRGB leds[NUM_LEDS];

int clear_leds = 0;
int hits_taken = 0;
int hits_last_time = 0;
int fire_start_side = 0;
int fire_time_start = 0;
int fire_time_stop = 0;
int fire_last_level = 0;
int fire_mode_entered = 0;
int fire_mode_exit_time = 0;
int police_time_start = 0;
int police_beacon_blink_count = 0;
int police_beacon_count_delay = 0;
int police_beacon_ticks = 0;
int police_beacon_ticks_odd = 0;
int police_last_mode = 0;
int pwm_value;

CRGBPalette16 gPal;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  gPal = HeatColors_p;
  // pinMode(PWM_FPVC_PIN, INPUT);
  // Serial.begin(115200);
}

void loop()
{
  random16_add_entropy(random());
  pwm_value = pulseIn(PWM_PIN, HIGH);
  int current_time = millis();
  if (pwm_value >= PWM_HIT_DETECTION) {
    if ((hits_last_time == 0 or (hits_last_time + FIRE_TIMEOUT) < current_time) and hits_taken < 10) {
      hits_taken++;
      hits_last_time = current_time;
    }
    if (hits_last_time + 150 > current_time and hits_taken < 10) {
      SetFireLevel(7, 1);
    }
    else {
      SetFireLevel(hits_taken, 0);
    }
    police_last_mode = 0;
    fire_mode_entered = 1;
  }
  else if (pwm_value >= PWM_DAMAGE_MODE) {
    SetFireLevel(hits_taken, 0);
    police_last_mode = 0;
    fire_mode_entered = 1;
  }
  else if (pwm_value >= PWM_POLICE) {
    if (fire_mode_entered == 1) {
      if (fire_mode_exit_time == 0) {
        fire_mode_exit_time = current_time;
      }
      if (fire_mode_exit_time + 5000 < current_time) {
        fire_mode_entered = 0;
      }
      SetFireLevel(hits_taken, 0);
    }
    else {
      if (police_last_mode == 0) {
        FastLED.clear();
        police_last_mode = 1;
      }
      else {
        currentBlending = LINEARBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1;
        PoliceBeacons(1);
        FillLEDsFromPaletteColors(startIndex, LAMP_TOP_L_START, LAMP_TOP_L_END);
        FillLEDsFromPaletteColors(startIndex, LAMP_BOTTOM_R_START, LAMP_BOTTOM_R_END);
        FillLEDsFromPaletteColors(startIndex, LAMP_FRONT_TOP_START, LAMP_FRONT_TOP_END);
        PoliceBeacons(0);
        FillLEDsFromPaletteColors(startIndex, LAMP_TOP_R_START, LAMP_TOP_R_END);
        FillLEDsFromPaletteColors(startIndex, LAMP_BOTTOM_L_START, LAMP_BOTTOM_L_END);
        FillLEDsFromPaletteColors(startIndex, LAMP_FRONT_BOTTOM_START, LAMP_FRONT_BOTTON_END);
      }
    }
  }
  else if (pwm_value >= PWM_RAINBOW) {
    if (fire_mode_entered == 1) {
      if (fire_mode_exit_time == 0) {
        fire_mode_exit_time = current_time;
      }
      if (fire_mode_exit_time + 5000 < current_time) {
        fire_mode_entered = 0;
      }
      SetFireLevel(hits_taken, 0);
    }
    else {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND; 
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1;
      FillLEDsFromPaletteColors(startIndex, 0, NUM_LEDS-1);
      police_last_mode = 0;
    }
  }
  else {
    if (fire_mode_entered == 1) {
      if (fire_mode_exit_time == 0) {
        fire_mode_exit_time = current_time;
      }
      if (fire_mode_exit_time + 5000 < current_time) {
        fire_mode_entered = 0;
      }
      SetFireLevel(hits_taken, 0);
    }
    else {
      hits_taken = 0;
      hits_last_time = 0;
      police_time_start = 0;
      police_beacon_blink_count = 0;
      police_last_mode = 0;
      FastLED.clear();
    }
  }
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void SetFireLevel(int fire_level, int explosion)
{
  int current_fire_side = FireSingleSide();
  int cooling_modifier = 0;
  int sparking_modifier = 0;
  if (explosion == 1) {
    cooling_modifier = -30;
    sparking_modifier = 75;
  }
  if (fire_level >= 8) {
    cooling_modifier = fire_level * -2.5;
    sparking_modifier = fire_level * 8;
  }
  if (fire_level == 6) {
    cooling_modifier = 15;
    sparking_modifier = -15;
  }
  if (fire_level == 5) {
    cooling_modifier = 25;
    sparking_modifier = -25;
  }
  if (fire_level == 4) {
    cooling_modifier = 35;
    sparking_modifier = -35;
  }
  if (fire_level <= 2) {
    cooling_modifier = 75;
    sparking_modifier = 75;
  }
  if (fire_last_level != fire_level) {
    FastLED.clear();
    fire_last_level = fire_level;
  }
  else if (fire_level >= 5) {
    FireSectionMax11(fire_level + 1, FIRE_TOP_L_START, FIRE_TOP_L_END - (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
    FireSectionMax11(fire_level + 1, FIRE_TOP_R_START, FIRE_TOP_R_END + (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
    FireSectionMax11(fire_level + 1, FIRE_BOTTOM_L_START, FIRE_BOTTOM_L_END + (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
    FireSectionMax11(fire_level + 1, FIRE_BOTTOM_R_START, FIRE_BOTTOM_R_END - (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
  }
  else if (fire_level >= 3) {
    if (current_fire_side == 1) {
      FireSectionMax11(fire_level + 1, FIRE_TOP_L_START, FIRE_TOP_L_END - (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
      FireSectionMax11(fire_level + 1, FIRE_BOTTOM_L_START, FIRE_BOTTOM_L_END + (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
    }
    if (current_fire_side == 2) {
      FireSectionMax11(fire_level + 1, FIRE_TOP_R_START, FIRE_TOP_R_END + (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
      FireSectionMax11(fire_level + 1, FIRE_BOTTOM_R_START, FIRE_BOTTOM_R_END - (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
    }
  }
  else if (fire_level == 2) {
    int random_fire = random(10);
    int current_time = millis();
    int random_threshold = 3;
    int time_fire = 1000;
    int time_stop = 500;
    if (fire_level < 2) {
      random_threshold = 7;
      time_fire = 300;
      time_stop = 1500;
    }
    if ((random_fire > random_threshold or (fire_time_start > 0 and fire_time_start + time_fire > current_time)) and (fire_time_stop == 0 or fire_time_stop + time_stop < current_time)) {
      if (fire_time_start == 0) {
        fire_time_start = current_time;
      }
      if (current_fire_side == 1) {
      FireSectionMax11(fire_level + 1, FIRE_TOP_L_START, FIRE_TOP_L_END - (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
      FireSectionMax11(fire_level + 1, FIRE_BOTTOM_L_START, FIRE_BOTTOM_L_END + (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
      }
      if (current_fire_side == 2) {
      FireSectionMax11(fire_level + 1, FIRE_TOP_R_START, FIRE_TOP_R_END + (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
      FireSectionMax11(fire_level + 1, FIRE_BOTTOM_R_START, FIRE_BOTTOM_R_END - (10 - fire_level), COOLING + cooling_modifier, SPARKING + sparking_modifier);
      }
    }
    else {
      fire_time_start = 0;
      fire_time_stop = current_time;
      FastLED.clear();
    }
  }
  else {
    fire_start_side = 0;
    FastLED.clear();
  }
}

int FireSingleSide()
{
  if (fire_start_side < 1 or fire_start_side > 2) {
    int fire_start_draw = random(10);
    if (fire_start_draw >= 5) {
      fire_start_side = 1;
    }
    else {
      fire_start_side = 2;
    }
  }
  return fire_start_side;
}

void FireSectionMax11(int length, int start_led, int end_led, int cooling, int sparking)
{
  static byte heat[11];
  for(int i = 0; i < length; i++) {
    heat[i] = qsub8(heat[i],  random8(0, ((cooling * (length - 1)) / length) + 2));
  }
  for(int k = length - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  if(random8() < sparking) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }
  for(int j = 0; j < length; j++) {
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    if (end_led < start_led) {
      pixelnumber = start_led - j;
    } else {
      pixelnumber = start_led + j;
    }
    leds[pixelnumber] = color;
  }
}

void PoliceBeacons(int is_odd)
{
  CRGB blue = CHSV(HUE_BLUE, 255, 255);
  CRGB black = CRGB::Black;
  if (police_beacon_ticks >= POLICE_BLINKS) {
    currentPalette = CRGBPalette16(black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black);
    if (police_beacon_count_delay >= 4) {
      police_beacon_count_delay = 0;
      police_beacon_ticks = 0;
      if (police_beacon_ticks_odd == 0) {
        police_beacon_ticks_odd = 1;
      }
      else {
        police_beacon_ticks_odd = 0;
      }
    }
    else {
      police_beacon_count_delay++;
    }
  }
  else {
    if (police_beacon_blink_count < 1) {
      if ((is_odd == 1 and police_beacon_ticks_odd == 1) or (is_odd == 0 and police_beacon_ticks_odd == 0)) {
        currentPalette = CRGBPalette16(blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue, blue);
      }
      else {
        currentPalette = CRGBPalette16(black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black);
      }
    }
    else if (police_beacon_blink_count < 2) {
      currentPalette = CRGBPalette16(black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black);
    }
    if (police_beacon_blink_count >= 2) {
      police_beacon_blink_count = 0;
      police_beacon_ticks++;
    }
    else {
      police_beacon_blink_count++;
    }
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex, int startIndex, int endIndex)
{
  uint8_t brightness = 255;
  for (int i = startIndex; i <= endIndex; ++i) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}
