# NeoPixel Game

An interactive real-time game built with Arduino and Adafruit NeoPixel LEDs. Control a paddle to catch falling colored dots on an 8x8 LED matrix and maximize your score!

## Overview

This project combines Arduino microcontroller programming with addressable RGB LEDs to create an engaging, hands-on game experience. The game runs for 30 seconds, with increasing difficulty as falling dots accelerate over time.

## Features

- **Interactive Gameplay**: Move a paddle left and right using physical buttons
- **8x8 LED Matrix Display**: Smooth 40 FPS rendering on a 64-pixel NeoPixel matrix
- **Progressive Difficulty**: Game speed increases every second after the first 10 seconds
- **Dual Falling Objects**: After 10 seconds, a second dot spawns for added challenge
- **Score System**: Earn points for catching dots, lose points for misses
- **Visual Feedback**: 
  - 3-second countdown before game starts (orange LEDs)
  - Green display for good performance (score ��� 15)
  - Red display for lower scores
  - Colorful random dots

## Hardware Requirements

- Arduino microcontroller (tested on Arduino Uno)
- 64 WS2812B addressable RGB LEDs (8x8 matrix layout)
- 2 push buttons (for left/right controls)
- Resistors and power supply for LED strip
- Connecting wires

## Pin Configuration

| Component | Arduino Pin |
|-----------|------------|
| NeoPixel Data | Pin 6 |
| Left Button | Pin 3 |
| Right Button | Pin 2 |

## Game Mechanics

### Gameplay Duration
- **Total Time**: 30 seconds
- **Difficulty Ramp**: 
  - First 10 seconds: Single falling dot
  - After 10 seconds: Second dot added
  - Continuous: Speed increases 3% every second after 10 seconds

### Scoring
- **Hit**: +1 point (catch dot with paddle)
- **Miss**: -2 points (dot reaches bottom without being caught)

### Controls
- **Left Button**: Move paddle left
- **Right Button**: Move paddle right

### Winning Condition
- **Good Performance**: Score ≥ 15 → Green LED display
- **Lower Score**: Score < 15 → Red LED display

## Software Architecture

### Key Constants
```cpp
FPS = 40              // Smooth 40 frames per second rendering
NUMPIXELS = 64        // 8x8 LED matrix
MAX_DOTS = 2          // Maximum falling dots
Initial dropDelay = 400ms  // Starting fall speed
