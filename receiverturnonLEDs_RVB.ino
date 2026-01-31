#include <Adafruit_NeoPixel.h>

#define LED_PIN     16
//#define NUM_LEDS    200
#define NUM_LEDS    40


Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial1.begin(9600);  // RX = GPIO1 ← Connected from TX on sender
  Serial.println("💡 startup ..");
  strip.begin();
  strip.setBrightness(180);
  strip.fill(strip.Color(0, 0, 255));
  strip.show();

  Serial.println("💡 LED board ready and listening...");
}

void loop() {
  // 1) Always run the idle animation step (non-blocking)
  alltheTime();

  // 2) Always check for voice commands (also non-blocking)
  if (Serial1.available()) {
    int commandID = Serial1.parseInt();

    // Optional: flush any leftover newline/spaces if your sender includes them
    while (Serial1.available()) Serial1.read();

    strip.clear();
    strip.show();

    Serial.print("📥 Received Command ID: ");
    Serial.println(commandID);

    switch (commandID) {
      case 100:   commandHeard(); break;
      case 10000: haloArmorHitEffect(); break;
      case 10001: newZombieEffect(); break;
      case 10002: overshieldEffect(); break;
      case 10003: headshotEffect(); break;
      case 10004: betrayalEffect(); break;
      case 10005: killamanjaroEffect(); break;
      case 10006: distractionEffect(); break;
      case 10007: kingOfTheHillEffect(); break;
      case 10008: boofGangEffect(); break;
      default:
        Serial.println("⚠️ Unknown command ID.");
        break;
    }
  }
}



void alltheTime() {

  const uint32_t RED    = strip.Color(220,  0,   0);
  const uint32_t WHITE   = strip.Color(255, 255, 255);
  const uint32_t GREEN   = strip.Color(0, 255, 0);
/*
// ---------- Phase 0: Blink Red and Green Every Second ----------
  strip.fill(strip.Color(255, 0, 0));  // Solid red
  strip.show();
  delay(1000);  // hold for one second
  strip.fill(strip.Color(0, 255, 0));  // Solid green
  strip.show();
  delay(1000);  // hold for one second

*/

// ---------- Phase 1: First 6 LEDS turn white while the rest stay red ----------


  strip.fill(strip.Color(220, 0, 0));
  strip.show();
    
    // sweep left->right
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, WHITE);
      if (i > 0) strip.setPixelColor(i - 3, RED);
      strip.show();
      delay(75);
    }
    // sweep right->left
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
      strip.setPixelColor(i, GREEN);
      if (i < NUM_LEDS - 1) strip.setPixelColor(i + 3, RED);
      strip.show();
      delay(100);
    }

} 


// Example animations
/*
void alltheTime() {
  static uint16_t startHue = 0;
  static uint8_t sparkleCountdown = 0;

  // Draw a soft rainbow background
  for (int i = 0; i < NUM_LEDS; i++) {
    uint16_t hue = startHue + (i * 10);
    uint32_t color = strip.ColorHSV(hue * 182, 255, 100); // dim background
    strip.setPixelColor(i, color);
  }

  // Occasionally flash a random LED with a bright color
  if (sparkleCountdown == 0) {
    int sparkleIndex = random(NUM_LEDS);
    uint8_t sparkleHue = random(0, 255);  // Random hue
    uint32_t sparkleColor = strip.ColorHSV(sparkleHue * 182, 255, 255);  // Full brightness

    strip.setPixelColor(sparkleIndex, sparkleColor);
    sparkleCountdown = random(3, 8);  // delay between sparkles
  } else {
    sparkleCountdown--;
  }

  strip.show();
  startHue++;       // Slowly shift hues
  delay(30);        // Speed of animation
}

*/

void commandHeard() {
   Serial.println("heard the command");
  for (int i = 0; i < 2; i++) {
    strip.clear();
    strip.show();
    delay(50);
    strip.fill(strip.Color(0, 100, 255));  // GreenBlue
    strip.show();
    delay(50);
  }
  delay(3000);
  strip.clear();
  strip.show();
}

void haloArmorHitEffect() {
 // Serial.println("💥 SHIELDS DOWN: Armor Hit! (safe red pre-flash)");

  // ---------- Phase 0: Turn all LEDs solid red ----------
  strip.fill(strip.Color(255, 0, 0));  // Solid red
  strip.show();
  delay(500);  // Hold for half a second before strobing

  // ---------- Phase 1: Safe strobe 3 Hz for 2 seconds ----------
  const uint8_t  R = 255, G = 0, B = 0;     // red
  const uint8_t  ON_MS = 100;               // 100 ms on
  const uint16_t OFF_MS = 233;              // 233 ms off -> ~3 Hz
  const uint8_t  FLASHES = 6;               // 2 seconds total

  for (uint8_t i = 0; i < FLASHES; i++) {
    strip.fill(strip.Color(R, G, B));  // ON
    strip.show();
    delay(ON_MS);

    strip.clear();                     // OFF
    strip.show();
    delay(OFF_MS);
  }

  // ---------- Phase 1b: Red "sparkle storm" ----------
  const uint16_t SPARKLE_DURATION_MS = 2500;  // 2.5 seconds
  const uint8_t  ACTIVE_FRACTION = 10;        // 10% of LEDs active per frame
  const uint8_t  MIN_BRIGHT = 80;
  const uint8_t  MAX_BRIGHT = 180;
  const uint16_t FRAME_MS = 40;               // ~25 FPS

  uint32_t t0 = millis();
  while (millis() - t0 < SPARKLE_DURATION_MS) {
    strip.clear();

    uint16_t count = max<uint16_t>(1, (NUM_LEDS * ACTIVE_FRACTION) / 100);
    for (uint16_t n = 0; n < count; n++) {
      uint16_t i = random(NUM_LEDS);
      uint8_t  v = random(MIN_BRIGHT, MAX_BRIGHT + 1);
      strip.setPixelColor(i, strip.Color(v, 0, 0));
    }
    strip.show();
    delay(FRAME_MS);
  }

  // ---------- Delay before blue recovery ----------
 // Serial.println("🕒 Delay before recovery...");
  delay(1500);

  // ---------- Phase 2: Turn blue gradually ----------
  //  Serial.println("💡 Phase 2: Activating blue LEDs one at a time");
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));  // Blue
    strip.show();
    delay(10);
  }

  //  Serial.println("⏳ Holding blue glow for 10 seconds");
  delay(10000);
}

void newZombieEffect() {
  // Serial.println("🧟 New Zombie!");

  // Base infection green across all LEDs
  strip.fill(strip.Color(0, 180, 0));  // sickly darker green
  strip.show();
  delay(200);

  // Blood spatter simulation
  for (int j = 0; j < 10; j++) {  // run spatter bursts
    int spatterCount = random(2, 6);  // a few random blood spots
    for (int k = 0; k < spatterCount; k++) {
      int idx = random(NUM_LEDS);  // pick a random LED
      strip.setPixelColor(idx, strip.Color(180, 0, 0));  // blood red
    }
    strip.show();
    delay(150);

    // fade back to green infection
    for (int k = 0; k < NUM_LEDS; k++) {
      strip.setPixelColor(k, strip.Color(0, random(120, 200), 0));  // varied green
    }
    strip.show();
    delay(200);
  }

  // End with a lingering infected glow
  for (int i = 0; i < 5; i++) {
    strip.fill(strip.Color(0, 220, 0));  // brighter sickly green
    strip.show();
    delay(300);
    strip.fill(strip.Color(0, 100, 0));  // dim, decayed green
    strip.show();
    delay(300);
  }

  // Clear out at the end
  strip.clear();
  strip.show();
}

void overshieldEffect() {
  // Serial.println("🛡️ Overshield!");

  // Pulse build-up (charging animation)
  for (int j = 0; j < 3; j++) {  // 3 charge-up cycles
    for (int b = 0; b < 255; b += 15) {  // fade in
      strip.fill(strip.Color(0, b/2, b));  // blue-ish glow
      strip.show();
      delay(20);
    }
    for (int b = 255; b >= 0; b -= 15) {  // fade out
      strip.fill(strip.Color(0, b/2, b));
      strip.show();
      delay(20);
    }
  }

  // Big flash of energy
  strip.fill(strip.Color(0, 255, 255));  // bright cyan/white
  strip.show();
  delay(500);

  // Stabilized overshield glow
  for (int i = 0; i < 8; i++) {  // pulse gently a few times
    strip.fill(strip.Color(0, 150, 255));  // base blue glow
    strip.show();
    delay(400);
    strip.fill(strip.Color(0, 80, 180));  // dimmer blue
    strip.show();
    delay(400);
  }

  // Clear after effect ends
  strip.clear();
  strip.show();
}

void headshotEffect() {
  // 💥 HEADSHOT: Instant kill → blood surge → slow death (30s cinematic version)

  // === PHASE 1: impact flash (0.5s) ===
  strip.fill(strip.Color(255, 255, 255));  // blinding white
  strip.show();
  delay(80);

  strip.fill(strip.Color(255, 20, 20));    // searing red takeover
  strip.show();
  delay(300);

  // === PHASE 2: blood surge shockwave (5s) ===
  for (int cycle = 0; cycle < 3; cycle++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.clear();
      for (int t = 0; t < 12; t++) {
        int pos = i - t;
        if (pos >= 0 && pos < NUM_LEDS) {
          uint8_t fade = max(0, 255 - (t * 22));
          strip.setPixelColor(pos, strip.Color(fade, 0, 0));  // deep blood trail
        }
      }
      strip.show();
      delay(12 + cycle * 4);  // slows each cycle slightly
    }
  }

  // === PHASE 3: slow heartbeat fade (8s) ===
  for (int beat = 0; beat < 16; beat++) {
    uint8_t intensity = (beat % 2 == 0) ? 220 : 80;
    strip.fill(strip.Color(intensity, 0, 0));
    strip.show();
    delay(250 + beat * 20);  // each beat slower than last
  }

  // === PHASE 4: chaotic flicker (10s) ===
  unsigned long flickerStart = millis();
  while (millis() - flickerStart < 10000) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (random(100) > 85) {
        uint8_t r = random(80, 180);
        strip.setPixelColor(i, strip.Color(r, 0, 0));
      } else if (random(100) > 90) {
        strip.setPixelColor(i, strip.Color(255, 60, 20));  // random ember
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
    strip.show();
    delay(random(40, 120));  // chaotic flicker timing
  }

  // === PHASE 5: power-drain fade to black (6s) ===
  for (int b = 180; b >= 0; b -= 2) {
    strip.fill(strip.Color(b, 0, 0));
    strip.show();
    delay(33);  // ~6 seconds total
  }

  strip.clear();
  strip.show();
}


void betrayalEffect() {
  // Serial.println("💥 Betrayal!");

  // Initial flash of shock
  strip.fill(strip.Color(255, 255, 255));  
  strip.show();
  delay(120);

  // Chaotic red flicker (blood spray feeling)
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (random(10) > 6) {  // ~30% chance for blood at each LED
        strip.setPixelColor(i, strip.Color(180, 0, 0));
      } else {
        strip.setPixelColor(i, strip.Color(50, 0, 0)); // faint background red
      }
    }
    strip.show();
    delay(100);
  }

  // Shivering red pulse (guilt/anger lingering)
  for (int p = 0; p < 8; p++) {
    strip.fill(strip.Color(255, 0, 0));  // intense red
    strip.show();
    delay(100);
    strip.fill(strip.Color(120, 0, 0));  // darker red
    strip.show();
    delay(100);
  }

  // Fade out into nothing
  for (int b = 200; b >= 0; b -= 10) {
    strip.fill(strip.Color(b, 0, 0));
    strip.show();
    delay(30);
  }

  strip.clear();
  strip.show();
}

// --- tiny helpers ---
static inline uint32_t dimColor(uint32_t c, uint8_t scale) {
  uint8_t r = (uint8_t)( (c >> 16) & 0xFF );
  uint8_t g = (uint8_t)( (c >>  8) & 0xFF );
  uint8_t b = (uint8_t)( (c >>  0) & 0xFF );
  r = (uint8_t)((r * scale) / 255);
  g = (uint8_t)((g * scale) / 255);
  b = (uint8_t)((b * scale) / 255);
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

static inline void addGlitter(uint8_t chance) {
  if (random(255) < chance) {
    int idx = random(NUM_LEDS);
    uint16_t hue = random(65535);
    strip.setPixelColor(idx, strip.gamma32(strip.ColorHSV(hue, 255, 255)));
  }
}

// --- the show ---
void killamanjaroEffect() {
  // Serial.println("🗻 KILLAMANJARO!");

  // Phase 1: escalating hype flashes (7 = ’Manjaro vibes)
  // Safer "purple sparkle burst" (~2 seconds, ~25 FPS, no full-screen flashes)
  {
    const uint16_t DURATION_MS     = 2000;   // total time
    const uint8_t  ACTIVE_FRACTION = 12;     // ~12% of pixels lit per frame
    const uint16_t FRAME_MS        = 40;     // ~25 FPS (not strobey)
    const uint8_t  MIN_R = 120, MAX_R = 200; // purple: red component range
    const uint8_t  MIN_B = 160, MAX_B = 255; // purple: blue component range
    const uint8_t  MIN_G = 0,   MAX_G = 20;  // keep green tiny so it never drifts toward white

    uint32_t t0 = millis();
    while (millis() - t0 < DURATION_MS) {
      strip.clear();

      // Light a random subset of pixels this frame
      uint16_t count = max<uint16_t>(1, (NUM_LEDS * ACTIVE_FRACTION) / 100);
      for (uint16_t n = 0; n < count; n++) {
        uint16_t i = random(NUM_LEDS);
        uint8_t r = random(MIN_R, MAX_R + 1);
        uint8_t g = random(MIN_G, MAX_G + 1);
        uint8_t b = random(MIN_B, MAX_B + 1);
        strip.setPixelColor(i, strip.Color(r, g, b));
      }

      strip.show();
      delay(FRAME_MS);  // consistent frame pacing, reads as motion not strobe
    }
  }

  // Phase 2: mirrored comet burst from the center (cyan & magenta), with fading trail
  int centerL = (NUM_LEDS - 1) / 2;
  int centerR = NUM_LEDS / 2;
  const uint8_t trailFade = 200;   // lower = longer trails
  for (int pass = 0; pass < 2; pass++) {
    for (int step = 0; step < max(centerL + 1, NUM_LEDS - centerR); step++) {
      // fade all pixels a bit each frame
      for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t c = strip.getPixelColor(i);
        strip.setPixelColor(i, dimColor(c, trailFade));
      }

      int leftPos  = centerL - step;
      int rightPos = centerR + step;

      if (leftPos  >= 0)          strip.setPixelColor(leftPos,  strip.Color(0, 255, 255));   // cyan comet
      if (leftPos+1 >= 0)         strip.setPixelColor(leftPos+1, dimColor(strip.Color(0,255,255), 140));
      if (rightPos < NUM_LEDS)    strip.setPixelColor(rightPos, strip.Color(255, 0, 255));   // magenta comet
      if (rightPos-1 < NUM_LEDS && rightPos-1 >= 0)
                                  strip.setPixelColor(rightPos-1, dimColor(strip.Color(255,0,255), 140));

      strip.show();
      delay(18);
    }
  }

  // Phase 3: rainbow theater chase (party time)
  for (int t = 0; t < 24; t++) { // a couple quick laps
    for (int q = 0; q < 3; q++) {
      strip.clear();
      for (int i = q; i < NUM_LEDS; i += 3) {
        uint16_t hue = (i * 65535UL / NUM_LEDS + t * 1092) & 0xFFFF; // smooth hue shift
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, 255, 255)));
      }
      strip.show();
      delay(50);
    }
  }

  // Phase 4: confetti fireworks on a gold base
  for (int f = 0; f < 25; f++) {
    // soft gold base, then sprinkle glitter pops
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(255, 180, 40)); // warm gold
    }
    addGlitter(90);   // lots of spark pops
    addGlitter(90);
    addGlitter(90);
    strip.show();
    delay(40);

    // slight fade to keep motion
    for (int i = 0; i < NUM_LEDS; i++) {
      uint32_t c = strip.getPixelColor(i);
      strip.setPixelColor(i, dimColor(c, 210));
    }
  }

  // Phase 5: safer finale — rainbow sweep + purple sparkles → hold gold
  const uint16_t DURATION_MS = 3000;  // how long the finale runs
  const uint16_t FRAME_MS    = 33;    // ~30 FPS (not strobey)
  const uint8_t  BASE_V      = 160;   // cap base brightness (0–255)
  const uint8_t  SPARKLE_FRAC= 12;    // % of pixels that sparkle per frame

  // Purple bounds (no white)
  const uint8_t PR_MIN = 120, PR_MAX = 200;  // red component
  const uint8_t PB_MIN = 160, PB_MAX = 255;  // blue component

  uint32_t t0 = millis();
  uint16_t baseHue = 0;               // advances each frame for a smooth sweep

  while (millis() - t0 < DURATION_MS) {
    // Base layer: soft rainbow gradient across the strip
    for (int i = 0; i < NUM_LEDS; i++) {
      uint16_t hue = baseHue + (uint32_t)i * 65535UL / NUM_LEDS;
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, 255, BASE_V)));
    }

    // Overlay: a few purple sparkles this frame (never the whole strip)
    uint16_t count = max<uint16_t>(1, (NUM_LEDS * SPARKLE_FRAC) / 100);
    for (uint16_t n = 0; n < count; n++) {
      int p = random(NUM_LEDS);
      uint8_t r = random(PR_MIN, PR_MAX + 1);
      uint8_t b = random(PB_MIN, PB_MAX + 1);
      strip.setPixelColor(p, strip.Color(r, 0, b));
    }

    strip.show();
    delay(FRAME_MS);
    baseHue += 256;                    // slow hue drift (≈ one full cycle in ~1.3 s)
  }

  // Gentle settle to "gold crown" (static, no strobe)
  const uint16_t GOLD_H = 0x2A00;      // warm gold/yellow hue
  const uint8_t  GOLD_S = 220;
  const uint8_t  GOLD_V = 200;         // comfy brightness
  strip.fill(strip.gamma32(strip.ColorHSV(GOLD_H, GOLD_S, GOLD_V)));
  strip.show();
  // (hold duration if you want)
  // delay(3000);


  // Crown it in purple for a beat
  strip.fill(strip.Color(105, 0, 255)); 
  strip.show();
  delay(600);

  // Clean exit
  strip.clear();
  strip.show();
}


void distractionEffect() {
  // Serial.println("💥 Distraction!");

  // Run the chaos for ~3 seconds
  for (int t = 0; t < 30; t++) {
    strip.clear();

    // Randomly light up between 3–8 LEDs each frame
    int burstCount = random(3, 9);
    for (int i = 0; i < burstCount; i++) {
      int idx = random(NUM_LEDS);
      int brightness = random(120, 255); // varied brightness
      // flicker between yellow/orange
      strip.setPixelColor(idx, strip.Color(brightness, brightness, random(0, 80)));
    }

    strip.show();
    delay(random(50, 150));  // erratic timing
  }

  // Phase: 15s swirl (smooth, non-strobe), then clean reset
  const uint32_t DURATION_MS = 15000;  // 15 seconds
  const uint16_t FRAME_MS    = 33;     // ~30 FPS
  const uint8_t  BASE_V      = 170;    // cap brightness (0-255)
  const float    ARMS        = 3.0f;   // number of swirl bands

  uint32_t t0 = millis();
  uint16_t phase = 0;                  // advances each frame

  while (millis() - t0 < DURATION_MS) {
    for (int i = 0; i < NUM_LEDS; i++) {
      float u = (float)i / (float)NUM_LEDS;     // 0..1 along the strip

      // Rainbow hue that drifts over time for motion
      uint16_t hue = (uint16_t)((u * 65535.0f)) + phase;

      // Swirl brightness modulation: soft cosine waves around the strip
      float theta = (u * ARMS * 6.283185f) + (phase * (2.0f * 3.14159f / 65535.0f));
      float wave  = 0.5f + 0.5f * cosf(theta);          // 0..1
      uint8_t v   = (uint8_t)(BASE_V * (0.35f + 0.65f * wave)); // avoid full bright/dark

      uint32_t c = strip.gamma32(strip.ColorHSV(hue, 255, v));
      strip.setPixelColor(i, c);
    }

    strip.show();
    delay(FRAME_MS);
    phase += 256;  // swirl speed; lower = slower, higher = faster
  }

  // Clean reset (as in your original)
  strip.clear();
  strip.show();

}


// Helper: blend two 24-bit colors (0..255)
static inline uint32_t lerpRGB(uint32_t c1, uint32_t c2, uint8_t t) {
  uint8_t r1 = (c1 >> 16) & 0xFF, g1 = (c1 >> 8) & 0xFF, b1 = c1 & 0xFF;
  uint8_t r2 = (c2 >> 16) & 0xFF, g2 = (c2 >> 8) & 0xFF, b2 = c2 & 0xFF;
  uint8_t r = r1 + ((int16_t)(r2 - r1) * t) / 255;
  uint8_t g = g1 + ((int16_t)(g2 - g1) * t) / 255;
  uint8_t b = b1 + ((int16_t)(b2 - b1) * t) / 255;
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

void kingOfTheHillEffect() {
 //  Serial.println("👑 King of the Hill!");

  const uint32_t YELLOW = strip.Color(255, 200, 40);
  const uint32_t ORANGE = strip.Color(255, 120, 20);
  const uint32_t RED    = strip.Color(220,  0,   0);
  const uint32_t GOLD   = strip.Color(255, 190, 60);

  // ----- Phase 1: Beacon breathing (yellow) -----
  for (int cycle = 0; cycle < 3; cycle++) {
    for (int b = 80; b <= 255; b += 10) {
      uint32_t c = strip.Color((uint8_t)(255UL * b / 255),
                               (uint8_t)(200UL * b / 255),
                               (uint8_t)( 40UL * b / 255));
      strip.fill(c);
      strip.show();
      delay(18);
    }
    for (int b = 255; b >= 80; b -= 10) {
      uint32_t c = strip.Color((uint8_t)(255UL * b / 255),
                               (uint8_t)(200UL * b / 255),
                               (uint8_t)( 40UL * b / 255));
      strip.fill(c);
      strip.show();
      delay(18);
    }
  }

  // ----- Phase 2: Capture fill (center outward, yellow -> red) -----
  int centerL = (NUM_LEDS - 1) / 2;
  int centerR = NUM_LEDS / 2;
  int maxStep = max(centerL + 1, NUM_LEDS - centerR);

  strip.clear();
  for (int step = 0; step < maxStep; step++) {
    // progress 0..255 for smooth color ramp
    uint8_t t = (uint8_t)min(255, (step * 255) / max(1, maxStep - 1));
    // yellow -> orange -> red
    uint32_t yo = lerpRGB(YELLOW, ORANGE, (uint8_t)min(255, t * 2));    // first half
    uint32_t target = (t < 128) ? yo : lerpRGB(ORANGE, RED, (uint8_t)((t - 128) * 2));

    int l = centerL - step;
    int r = centerR + step;
    if (l >= 0) strip.setPixelColor(l, target);
    if (r < NUM_LEDS) strip.setPixelColor(r, target);

    // keep previous pixels “captured” color
    strip.show();
    delay(22);
  }

  // ----- Phase 3: Contested stutter (yellow <-> red) -----
  for (int j = 0; j < 6; j++) {
    strip.fill((j & 1) ? RED : YELLOW);
    strip.show();
    delay(80);
  }

  // ----- Phase 4: Crown the hill (deep red base with gold pulses) -----
  strip.fill(RED); strip.show();

  // three gold pulses that sweep across the strip
  for (int pulse = 0; pulse < 3; pulse++) {
    // sweep left->right
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, GOLD);
      if (i > 0) strip.setPixelColor(i - 1, RED);
      strip.show();
      delay(10);
    }
    // sweep right->left
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
      strip.setPixelColor(i, GOLD);
      if (i < NUM_LEDS - 1) strip.setPixelColor(i + 1, RED);
      strip.show();
      delay(10);
    }
  }

  // Hold the hill for a beat
  strip.fill(RED); strip.show(); delay(500);

  // Clean exit
  strip.clear();
  strip.show();
}

void boofGangEffect() {
  // 🎉 Boof Gang FINALE (safe chases, comets, sparkles, rainbow; no white/strobe)

  const uint16_t BEAT = 60000UL / 96; // keep tempo for pacing
  const uint8_t  SAT  = 255;          // saturated color (no pastels)
  const uint8_t  VAL  = 200;          // comfy cap (avoid blinding)

  // --- tiny local helpers (same signatures you used) ---
  auto dim24 = [](uint32_t c, uint8_t scale)->uint32_t {
    uint8_t r = (c >> 16) & 0xFF, g = (c >> 8) & 0xFF, b = c & 0xFF;
    r = (uint8_t)((uint16_t)r * scale / 255);
    g = (uint8_t)((uint16_t)g * scale / 255);
    b = (uint8_t)((uint16_t)b * scale / 255);
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  };
  auto fadeAll = [&](uint8_t scale){
    for (uint16_t i=0; i<NUM_LEDS; i++){
      uint32_t c = strip.getPixelColor(i);
      strip.setPixelColor(i, dim24(c, scale));
    }
  };
  auto strideFor = [&](uint16_t targetSteps)->uint16_t {
    uint16_t s = (NUM_LEDS + targetSteps - 1) / targetSteps; // ceil
    return s < 1 ? 1 : s;
  };

  // ==== PHASE 0: Soft rainbow theater chase (no flashes) - 8 beats ====
  for (uint8_t beat=0; beat<8; beat++){
    strip.clear();
    for (uint8_t lane=0; lane<3; lane++){
      for (uint16_t i=(beat+lane)%3; i<NUM_LEDS; i+=3){
        uint16_t hue = (uint32_t)i * 65535UL / (NUM_LEDS?NUM_LEDS:1);
        hue = (hue + beat * 3500U) & 0xFFFF;
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, SAT, VAL)));
      }
    }
    strip.show();
    delay(BEAT); // ~625 ms per frame => gentle motion
  }

  // ==== PHASE 1: Mirrored rainbow comets (build, no downbeat flashes) - 16 beats ====
  {
    int32_t cL = (NUM_LEDS - 1) / 2;
    int32_t cR = NUM_LEDS / 2;
    uint16_t span   = (uint16_t)max(cL + 1, (int32_t)NUM_LEDS - cR);
    uint16_t stepInc= strideFor(120);
    uint16_t frames = (uint16_t)(16 * (BEAT / 10)); // ~1600 frames
    uint16_t pos    = 0;

    for (uint16_t f=0; f<frames; f++){
      fadeAll(NUM_LEDS >= 600 ? 242 : 225);  // crisp trails

      int32_t L = cL - (int32_t)pos;
      int32_t R = cR + (int32_t)pos;

      uint16_t hueL = (uint16_t)((f*64) & 0xFFFF);
      uint16_t hueR = (uint16_t)(((f*64) + 32768U) & 0xFFFF);

      if (L >= 0)                 strip.setPixelColor((uint16_t)L, strip.gamma32(strip.ColorHSV(hueL, SAT, VAL)));
      if (L+1 >= 0)               strip.setPixelColor((uint16_t)max(0L,L+1), dim24(strip.getPixelColor((uint16_t)max(0L,L+1)), 180));
      if (R < NUM_LEDS)           strip.setPixelColor((uint16_t)R, strip.gamma32(strip.ColorHSV(hueR, SAT, VAL)));
      if (R-1 >= 0 && R-1 < NUM_LEDS)
                                  strip.setPixelColor((uint16_t)(R-1), dim24(strip.getPixelColor((uint16_t)(R-1)), 180));

      strip.show();
      delay(10); // ~100 FPS engine, but trails keep it non-strobey

      pos += stepInc;
      if (pos >= span) pos = 0;
    }
  }

  // ==== PHASE 2: Rainbow wave + purple/gold confetti (no white) - 20 beats ====
  for (uint8_t beat=0; beat<20; beat++){
    for (uint8_t sub=0; sub<4; sub++){ // ~4 frames per beat
      for (uint16_t i=0; i<NUM_LEDS; i++){
        uint16_t hue = ((uint32_t)i * 65535UL / (NUM_LEDS?NUM_LEDS:1) + (beat*2048U + sub*1024U)) & 0xFFFF;
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, SAT, VAL)));
      }
      strip.show();
      delay(BEAT/8); // ~78 ms => ~13 FPS for base wave (soft motion)
    }
    // Downbeat: sparse confetti (purple/gold), never full screen
    if ((beat % 4) == 0) {
      uint16_t step = strideFor(80);
      for (uint16_t i=0; i<NUM_LEDS; i+=step) {
        bool gold = (random(3)==0);
        uint32_t c = gold
          ? strip.Color(255,190,60)  // gold
          : strip.Color(random(120,200), 0, random(160,255)); // purple family
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(BEAT/4);
    }
  }

  // ==== PHASE 3: Speed chase + sweep mix (smooth, no flashes) - 12 beats ====
  for (uint8_t beat=0; beat<12; beat++){
    uint16_t stride = strideFor(120);
    // left->right sweep
    for (uint16_t i=0; i<NUM_LEDS; i+=stride){
      if (i>=stride) strip.setPixelColor(i-stride, 0);
      uint16_t hue = ((uint32_t)i * 65535UL / (NUM_LEDS?NUM_LEDS:1) + beat*4096U) & 0xFFFF;
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, SAT, VAL)));
      strip.show();
      delay(6); // smooth movement, not strobe
    }
    // tiny rainbow sparkles (sparse)
    for (uint8_t s=0; s<max<uint8_t>(3, NUM_LEDS/150); s++){
      strip.setPixelColor(random(NUM_LEDS), strip.gamma32(strip.ColorHSV(random(65535), SAT, VAL)));
    }
    strip.show();
    delay(BEAT/2);

    // bounce back
    for (int32_t i=NUM_LEDS-1; i>=0; i-=stride){
      if (i+stride<NUM_LEDS) strip.setPixelColor(i+stride, 0);
      uint16_t hue = ((uint32_t)i * 65535UL / (NUM_LEDS?NUM_LEDS:1) + beat*4096U + 20000U) & 0xFFFF;
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, SAT, VAL)));
      strip.show();
      delay(6);
    }
    strip.show();
    delay(BEAT/2);
  }

  // ==== BUTTON (safe): 15s swirl → gold hold → gentle fade ====
  {
    const uint32_t DURATION_MS = 15000; // 15 seconds
    const uint16_t FRAME_MS    = 33;    // ~30 FPS
    const uint8_t  BASE_V      = 180;   // comfy cap
    const float    ARMS        = 3.0f;

    uint32_t t0 = millis();
    uint16_t phase = 0;

    while (millis() - t0 < DURATION_MS) {
      for (int i = 0; i < NUM_LEDS; i++) {
        float u = (float)i / (float)NUM_LEDS;
        uint16_t hue = (uint16_t)((u * 65535.0f)) + phase;
        float theta = (u * ARMS * 6.283185f) + (phase * (2.0f * 3.14159f / 65535.0f));
        float wave  = 0.5f + 0.5f * cosf(theta);
        uint8_t v   = (uint8_t)(BASE_V * (0.35f + 0.65f * wave));
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue, SAT, v)));
      }
      strip.show();
      delay(FRAME_MS);
      phase += 256;
    }

    // Gold hold (no white)
    strip.fill(strip.Color(255,190,60));
    strip.show();
    delay(800);

    // Gentle fade out (color-preserving dim)
    for (int s=255; s>=0; s-=14){ fadeAll((uint8_t)s); strip.show(); delay(16); }
    strip.clear(); strip.show();
  }
}


