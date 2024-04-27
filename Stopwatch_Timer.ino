#include "SevSeg.h"
SevSeg sevenSegment;

const int A = 9;
const int B = 8;
const int C = 7;
const int D = 6;
const int E = 5;
const int F = 4;
const int G = 3;
const int DP = 2;
const int D1 = 10;
const int D2 = 11;
const int D3 = 12;
const int D4 = 13;

const int start = A0;
const int plus = A1;
const int minus = A2;
const int modeChange = A3;
const int resetPin = A4;

unsigned long lastMillisStopwatch = 0;
const long intervalStopwatch = 100;
unsigned long lastMillisTimer = 0;
const long intervalTimer = 1000;
int stopwatchNumber = 0;
int timerNumber = 0;
bool stopwatchState = false;
bool timerState = false;
bool currentMode = false;
unsigned long debounceDelay = 20;
unsigned long lastDebounceTime = 0;

void setup() {
  sevenSegmentSetup();
  buttonsSetup();
}

void loop() {
  if (!currentMode) {
    stopwatch();
  } else {
    timer();
  }

  changeMode();
  resetState();
}

void sevenSegmentSetup() {
  byte numDigits = 4;
  byte digitPins[] = { D1, D2, D3, D4 };
  byte segmentPins[] = { A, B, C, D, E, F, G, DP };
  bool resistorsOnSegments = 1;
  sevenSegment.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevenSegment.setBrightness(90);
}

void buttonsSetup() {
  pinMode(start, INPUT_PULLUP);
  pinMode(plus, INPUT_PULLUP);
  pinMode(minus, INPUT_PULLUP);
  pinMode(modeChange, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
}

void stopwatch() {
  int startStopwatch = digitalRead(start);
  if (startStopwatch == LOW) {
    stopwatchState = !stopwatchState;
    while (startStopwatch == LOW) {
      startStopwatch = digitalRead(start);
      sevenSegment.setNumber(stopwatchNumber, 1);
      sevenSegment.refreshDisplay();
    }
  }

  if (stopwatchState) {
    unsigned long currentMillisStopwatch = millis();
    if (currentMillisStopwatch - lastMillisStopwatch >= intervalStopwatch) {
      lastMillisStopwatch = currentMillisStopwatch;
      if(stopwatchNumber < 9999) {
        stopwatchNumber++;
      } 
    }
  }
  sevenSegment.setNumber(stopwatchNumber, 1);
  sevenSegment.refreshDisplay();
}

void timer() {
  int plusTime = digitalRead(plus);
  int minusTime = digitalRead(minus);
  int startTimer = digitalRead(start);

  if (plusTime == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    if (timerNumber < 9999) timerNumber++;
    lastDebounceTime = millis();
  }

  if (minusTime == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    if (timerNumber > 0) timerNumber--;
    lastDebounceTime = millis();
  }

  if (startTimer == LOW) {
    timerState = !timerState;
    while (startTimer == LOW) {
      startTimer = digitalRead(start);
      sevenSegment.setNumber(timerNumber);
      sevenSegment.refreshDisplay();
    }
  }

  if (timerState) {
    unsigned long currentMillisTimer = millis();
    if (currentMillisTimer - lastMillisTimer >= intervalTimer) {
      lastMillisTimer = currentMillisTimer;
      if (timerNumber > 0) {
        timerNumber--;
      } else {
        timerNumber = 0;
        timerState = !timerState;
      }
    }
  }

  sevenSegment.setNumber(timerNumber);
  sevenSegment.refreshDisplay();
}

void changeMode() {
  int modeChangeState = digitalRead(modeChange);
  if (modeChangeState == LOW) {
    currentMode = !currentMode;
    while (modeChangeState == LOW) {
      modeChangeState = digitalRead(modeChange);
      if (currentMode) {
        sevenSegment.setNumber(stopwatchNumber, 1);
        sevenSegment.refreshDisplay();
      } else {
        sevenSegment.setNumber(timerNumber);
        sevenSegment.refreshDisplay();
      }
    };
  }
}

void resetState() {
  int resetStatePin = digitalRead(resetPin);
  if (resetStatePin == LOW) {
    stopwatchState = 0;
    timerState = 0;
    if (!currentMode) {
      stopwatchNumber = 0;
      sevenSegment.setNumber(stopwatchNumber, 1);
      sevenSegment.refreshDisplay();
    } else {
      timerNumber = 0;
      sevenSegment.setNumber(timerNumber);
      sevenSegment.refreshDisplay();
    }
  }
}
