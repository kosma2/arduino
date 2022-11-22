//DISPLAY

#include "LedControl.h"
LedControl lc = LedControl(12, 10, 11, 1);
int delaytime = 150;
int shiPosX = 3;
int terLef[] = { -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 2, 2, 2, 3, 3, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 2, 1, 0, 1, 2, 3, 2, 1, 0};     // terrain contours on left side of screen
int terRig[] = {8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 6, 7, 7, 6, 7, 7, 6, 6, 6, 5, 5, 4, 4, 5, 6, 6, 6, 7, 7, 6, 5, 6, 6, 6, 6, 7, 7, 7, 7, 6, 7, 7, 7,};    // right side
int scrolCount = 0;

//TURN KNOB

volatile boolean TurnDetected;  // need volatile for Interrupts
volatile boolean rotationdirection;  // CW or CCW rotation

const int PinCLK = 2; // Generating interrupts using CLK signal
const int PinDT = 3;  // Reading DT signal
const int PinSW = 4;  // Reading Push Button
int RotaryPosition = 0;

void isr ()  {
  delay(4);  // delay for Debouncing
  if (digitalRead(PinCLK))
    rotationdirection = digitalRead(PinDT);
  else
    rotationdirection = !digitalRead(PinDT);
  TurnDetected = true;
}

void setup() {
  //DISPLAY
  lc.shutdown(0, false);        // no to timed shutdown
  lc.setIntensity(0, 2);        //Set the brightness to a medium values

  //TURN KNOB
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(PinSW, INPUT);
  digitalWrite(PinSW, HIGH); // Pull-Up resistor for switch
  attachInterrupt (0, isr, FALLING); // interrupt 0 always connected to pin 2 on Arduino UNO
}

void loop() {
  lc.clearDisplay(0);           //clear the display

  //TURN KNOB

  // Runs if rotation was detected
  if (TurnDetected)  {
    if (rotationdirection) {
      RotaryPosition = RotaryPosition - 1;     // decrase Position by 1
    }
    else {
      RotaryPosition = RotaryPosition + 1;     // increase Position by 1
    }
    TurnDetected = false;  // do NOT repeat IF loop until new rotation detected
  }

  shiPosX = shiPosX + RotaryPosition;       //updating the ship's position with control input

  if (shiPosX < 5 || shiPosX > 2) {
    RotaryPosition = 0;
  }

  // TERRAIN

  for (int x = 0; x <= 7; x++) {
    for (int a = 0; a <= terLef[scrolCount + x]; a++) {     //Terrain on left side
      lc.setLed(0, a, x, true);
      if (x == 2) {
        if (a == shiPosX) {                                 // if there is terrain at ship position
          scrolCount = 0;                                   // restart scroll
          lc.clearDisplay(0);                               // clear display
        }
      }
    }

    for (int a = terRig[scrolCount + x]; a <= 7; a++) {     //Terrain on right side
      lc.setLed(0, a, x, true);
      if (x == 2) {
        if (a == shiPosX) {                                 // if there is terrain at ship position
          scrolCount = 0;                                   // restart scroll
          lc.clearDisplay(0);                               // clear display
        }
      }
    }
  }

  lc.setLed(0, shiPosX, 2, true);                           // display ship
  lc.setLed(0, shiPosX, 1, true);

  scrolCount++;
  if (scrolCount > 40) {                                    // Lenght of the terrain arrays
    scrolCount = 0;                                         // reset scroll
  }

  delay(delaytime);                                         // delay between each loop
}
