#include "LedControl.h"
LedControl lc = LedControl(12, 10, 11, 1);
int shiPos = 5;
int butPin = 8;
int knob = 0;
int knobMap;
int pewX[5] = {0, 0, 0, 0, 0};
int pewY[5] = {0, 0, 0, 0, 0};
int pewCount = 0;

void setup() {
  pinMode(butPin, INPUT_PULLUP);
  lc.shutdown(0, false);        // no to timed shutdown
  lc.setIntensity(0, 2);        //Set the brightness to a medium values
  Serial.begin(9600);
}

void loop() {
  lc.clearDisplay(0);           //clear the display
  //  SHIP
  lc.setLed(0, shiPos, 0, true);
  lc.setLed(0, shiPos, 1, true);
  lc.setLed(0, shiPos + 1, 0, true);
  lc.setLed(0, shiPos - 1, 0, true);
  knob = analogRead(A0);
  knobMap = map(knob, 0, 1022, 0, 7);
   Serial.println(knob);
  shiPos = knobMap;

  // PEWS
  if (digitalRead(butPin) == LOW) {
    pewX[pewCount] = shiPos;
    pewY[pewCount] = 1;
    pewCount++;
   // Serial.println(pewCount);
  }
  if (pewCount > 5) {
    pewCount = 0;
  }
  for (int a = 0; a < pewCount; a++) {
    if (pewY[a] < 8) {
      pewY[a] = pewY[a] + 1;
    }
    lc.setLed(0, pewX[a], pewY[a], true);
  }


  delay(150);
}
