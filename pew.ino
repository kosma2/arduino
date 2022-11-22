#include "LedControl.h"
LedControl lc = LedControl(12, 10, 11, 1);
int shiPos = 5;
int butPin = 8;
int pewX[5] = {0, 0, 0, 0, 0};
int pewY[5] = {0, 0, 0, 0, 0};
int alien[] = {4, 7};
int pewCount = 0;
int alienDir = 1;
bool alienHit = false;
bool shipHit = false;

//TURN KNOB

volatile boolean turnOn;  // need volatile for Interrupts
volatile boolean spinDir;  // CW or CCW rotation

const int CLKpin = 2; // Generating interrupts using CLK signal
const int DTpin = 3;  // Reading DT signal
int turnPos = 0;

void isr ()  {
  delay(4);  // delay for Debouncing
  if (digitalRead(CLKpin)) {
    spinDir = digitalRead(DTpin);
  } else {
    spinDir = !digitalRead(DTpin);
    turnOn = true;
  }
}
void setup() {
  //TURN KNOB
  pinMode(CLKpin, INPUT);
  pinMode(DTpin, INPUT);
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

  if (shiPos < 5 || shiPos > 2) {   // Stop it from getting off screen
    turnPos = 0;
  }

  //  SHIP  -   displaying the four pixels that make up the ship
  lc.setLed(0, shiPos, 0, true);
  lc.setLed(0, shiPos, 1, true);
  lc.setLed(0, shiPos + 1, 0, true);
  lc.setLed(0, shiPos - 1, 0, true);
  
  //  ALIENS    -   a three pixel alien ship
  lc.setLed(0, alien[0], alien[1], true);
  lc.setLed(0, alien[0] + 1, alien[1], true);
  lc.setLed(0, alien[0] + 1, alien[1] - 1, true);

  if (alienHit == false) {                // as long as alien ship not hit
    int rando = random(0, 3);             // generate a random number between 0 and 2
    
    if (rando == 0) {                     // if its 0
      if (alien[0] < 6) {                 // and the ship is within the right screen border
        alien[0] = alien[0] + 1;          // add 1 to its X coordinate
      }
    }
    if (rando == 1) {                     // if its 1
      if (alien[0] > 0) {                 // and the ship is within the left screen border
        alien[0] = alien[0] - 1;          // subract 1 from its X coordinate
      }
    }
    if (rando == 2) {                     // if its 2
      alien[1] = alien[1] - 1;            // subract 1 from its Y coordinate
    }
  }
  if (alien[1] < 0) {                     // when it gets to the bottom
    alien[1] = 8;                         // bring it to the top
  }
  


  // PEWS
  if (digitalRead(butPin) == LOW) {       // if button pressed
    pewX[pewCount] = shiPos;              // projectile's X position is ship X position
    pewY[pewCount] = 1;
    if (pewCount < 5) {                   // 5 shots at a time
      pewCount++;
    } else {
      pewCount = 0;
      pewCount++;
    }
  }

  for (int a = 0; a < pewCount; a++) {    // for all the shots currently on screen
    if (pewY[a] < 11) {                   // stop at Y 11
      pewY[a] = pewY[a] + 1;              // add 1 to Y
    }
    lc.setLed(0, pewX[a], pewY[a], true); // display the pew

    
    // hit testing for each of the alien pixel
    
    if (pewX[a] == alien[0] && pewY[a] == alien[1]) {   
      alienHit = true;
    }
    if (pewX[a] == alien[0] + 1 && pewY[a] == alien[1]) {
      alienHit = true;
    }
    if (pewX[a] == alien[0] + 1 && pewY[a] == alien[1] - 1) {
      alienHit = true;
    }
  }
  if (alienHit == true) {                 //if hit blink 5 times
    for (int a = 0; a < 5; a++) {
      lc.setLed(0, alien[0], alien[1], false);
      lc.setLed(0, alien[0] + 1, alien[1], false);
      lc.setLed(0, alien[0] + 1, alien[1] - 1, false);
      delay(200);
      lc.setLed(0, alien[0], alien[1], true);
      lc.setLed(0, alien[0] + 1, alien[1], true);
      lc.setLed(0, alien[0] + 1, alien[1] - 1, true);
      delay(200);

    }
    alien[1] = 9;                         // send alien to top
    alienHit = false;
  }
  
  // hit testing each of the ship's pixels
  
  if (alien[0] == shiPos && alien[1] == 0) {
    shipHit = true;
  }
   if (alien[0] == shiPos + 1 && alien[1] == 0) {
    shipHit = true;
  }
  if (alien[0] == shiPos - 1 && alien[1] == 0) {
    shipHit = true;
  }

 if (alien[0] + 1 == shiPos && alien[1] == 0) {
    shipHit = true;
  }
  if (alien[0] + 1 == shiPos + 1 && alien[1] == 0) {
    shipHit = true;
  }
  if (alien[0] + 1 == shiPos - 1 && alien[1] == 0) {
    shipHit = true;
  }

  if (alien[0] + 1 == shiPos && alien[1] - 1 == 0) {
    shipHit = true;
  }
  if (alien[0] + 1 == shiPos + 1 && alien[1] - 1 == 0) {
    shipHit = true;
  }
  if (alien[0] + 1 == shiPos - 1 && alien[1] - 1 == 0) {
    shipHit = true;
  }

  if (shipHit == true) {                          // if hit, blink 5 times
    for (int a = 0; a < 5; a++) {
      lc.setLed(0, shiPos, 0, false);
      lc.setLed(0, shiPos, 1, false);
      lc.setLed(0, shiPos + 1, 0, false);
      lc.setLed(0, shiPos - 1, 0, false);
      delay(150);
      lc.setLed(0, shiPos, 0, true);
      lc.setLed(0, shiPos, 1, true);
      lc.setLed(0, shiPos + 1, 0, true);
      lc.setLed(0, shiPos - 1, 0, true);
      delay(150);
    }
    alien[1] = 9;                               // send alien to top
    shipHit = false;
  }

  delay(150);
}
