#include "LedControl.h"
LedControl lc = LedControl(12, 10, 11, 1);
int shiPos = 5;
int butPin = 8;
int pewX[5] = {0, 0, 0, 0, 0};
int pewY[5] = {0, 0, 0, 0, 0};
int pewCount = 0;

//TURN KNOB

volatile boolean turnOn;  // need volatile for Interrupts
volatile boolean spinDir;  // CW or CCW rotation

const int CLKpin = 2; // Generating interrupts using CLK signal
const int DTpin = 3;  // Reading DT signal
const int PinSW = 4;  // Reading Push Button
int turnPos = 0;

void isr ()  {
  delay(4);  // delay for Debouncing
  if (digitalRead(CLKpin)){
    spinDir = digitalRead(DTpin);
  }else{
    spinDir = !digitalRead(DTpin);
  turnOn = true;}
}
void setup() {
  //TURN KNOB
  pinMode(CLKpin, INPUT);
  pinMode(DTpin, INPUT);
  pinMode(PinSW, INPUT);
  attachInterrupt (0, isr, FALLING); // interrupt 0 always connected to pin 2 on Arduino UNO

  pinMode(butPin, INPUT_PULLUP);
  lc.shutdown(0, false);        // no to timed shutdown
  lc.setIntensity(0, 2);        //Set the brightness to a medium values
  Serial.begin(9600);
}

void loop() {
  lc.clearDisplay(0);           //clear the display

  //TURN KNOB

  // Runs if rotation was detected
  if (turnOn)  {
    if (spinDir) {
      turnPos = turnPos - 1;     // decrase Position by 1
    }
    else {
      turnPos = turnPos + 1;     // increase Position by 1
    }
    turnOn = false;  // do NOT repeat IF loop until new rotation detected
  }

  shiPos = shiPos + turnPos;       //updating the ship's position with control input
  
  if (shiPos < 5 || shiPos > 2) {
    turnPos = 0;
  }
  
  //  SHIP
  lc.setLed(0, shiPos, 0, true);
  lc.setLed(0, shiPos, 1, true);
  lc.setLed(0, shiPos + 1, 0, true);
  lc.setLed(0, shiPos - 1, 0, true);

  // PEWS
  if (digitalRead(butPin) == LOW) {
    pewX[pewCount] = shiPos;
    pewY[pewCount] = 1;
    pewCount++;
    //  Serial.println(pewCount);
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
