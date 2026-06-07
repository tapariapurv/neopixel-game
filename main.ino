#include <Adafruit_NeoPixel.h>

#define PIN_MATRIX 6
#define PIN_LEFT   3
#define PIN_RIGHT  2
#define NUMPIXELS  64
#define MAX_DOTS   2 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_MATRIX, NEO_GRB + NEO_KHZ800);

// --- Game Constants ---
const int FPS = 40;               // Frames per second (higher = smoother paddle)
const int FRAME_DELAY = 1000 / FPS; 

// --- Game Variables ---
int score = 0;
bool gameRunning = true;
unsigned long gameStartTime;
unsigned long lastFrameUpdate = 0;
unsigned long lastDropMove = 0;
unsigned long lastSpeedUpdate = 0;
float dropDelay = 400.0; 

// --- Paddle ---
int paddleX = 2; 
bool lastLeftState = LOW;
bool lastRightState = LOW;

// --- Falling Dots ---
int dropX[MAX_DOTS], dropY[MAX_DOTS];
uint32_t dropColor[MAX_DOTS];
bool dropActive[MAX_DOTS];

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LEFT, INPUT);  
  pinMode(PIN_RIGHT, INPUT); 

  pixels.begin();
  pixels.setBrightness(30);
  randomSeed(analogRead(0));

  runCountdown(); 

  gameStartTime = millis();
  resetDrop(0);
  dropActive[0] = true;
  dropActive[1] = false;
}

void loop() {
  if (!gameRunning) return;

  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - gameStartTime;

  // 1. Check Game Over
  if (elapsed >= 30000) { gameOver(); return; }

  // 2. Input Handling (Checked every loop for maximum responsiveness)
  bool currentLeft = digitalRead(PIN_LEFT);
  bool currentRight = digitalRead(PIN_RIGHT);

  if (currentLeft == HIGH && lastLeftState == LOW) {
    if (paddleX > 0) paddleX--;
  }
  lastLeftState = currentLeft;

  if (currentRight == HIGH && lastRightState == LOW) {
    if (paddleX < 5) paddleX++; 
  }
  lastRightState = currentRight;

  // 3. Physics & Difficulty (Separated from Frame Rate)
  if (elapsed > 10000 && !dropActive[1]) { 
    resetDrop(1); 
    dropActive[1] = true; 
  }
  
  if (currentMillis - lastSpeedUpdate >= 1000) { 
    dropDelay *= 0.97; 
    lastSpeedUpdate = currentMillis; 
  }

  if (currentMillis - lastDropMove > (long)dropDelay) {
    lastDropMove = currentMillis;
    for (int i = 0; i < MAX_DOTS; i++) {
      if (dropActive[i]) {
        dropY[i]++;
        if (dropY[i] > 7) { 
          checkCollision(i); 
          resetDrop(i); 
        }
      }
    }
  }

  // 4. Rendering (The "Smooth" part)
  // We only update the LEDs at a fixed frame rate to prevent flickering
  if (currentMillis - lastFrameUpdate > FRAME_DELAY) {
    lastFrameUpdate = currentMillis;
    
    pixels.clear();
    
    // Draw Paddle
    setPixel(paddleX, 7, pixels.Color(0, 0, 255));
    setPixel(paddleX+1, 7, pixels.Color(0, 0, 255));
    setPixel(paddleX+2, 7, pixels.Color(0, 0, 255));
    
    // Draw Dots
    for (int i = 0; i < MAX_DOTS; i++) {
      if (dropActive[i]) setPixel(dropX[i], dropY[i], dropColor[i]);
    }
    
    pixels.show();
  }
}

// --- Helpers ---

void runCountdown() {
  for (int num = 3; num > 0; num--) {
    pixels.clear();
    // Fill rows to represent the number visually
    for(int i=0; i < (num * 16); i++) pixels.setPixelColor(i, pixels.Color(255, 150, 0));
    pixels.show();
    delay(1000);
  }
  pixels.clear();
  pixels.show();
}

void checkCollision(int i) {
  if (dropX[i] >= paddleX && dropX[i] <= paddleX + 2) {
    score++;
  } else {
    score -= 2;
  }
  Serial.print("Score: "); Serial.println(score);
}

void resetDrop(int i) {
  dropX[i] = random(0, 8); 
  dropY[i] = 0;
  dropColor[i] = pixels.Color(random(100,255), random(100,255), random(100,255));
}

void setPixel(int x, int y, uint32_t pColor) {
  int idx = y * 8 + x;
  if (idx >= 0 && idx < 64) pixels.setPixelColor(idx, pColor);
}

void gameOver() {
  gameRunning = false;
  pixels.clear();
  uint32_t finalCol = (score >= 15) ? pixels.Color(0, 255, 0) : pixels.Color(255, 0, 0);
  for(int i=0; i<64; i++) pixels.setPixelColor(i, finalCol);
  pixels.show();
  Serial.print("Final Score: "); Serial.println(score);
}