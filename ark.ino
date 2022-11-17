// INCLUDE LIBRARY

#include "LedControl.h"

// SET PINS FOR THE DISPLAY
LedControl lc=LedControl(12,10,11,1);

// DEFINE VARIABLES
int delaytime=100;
int ballVelX = 1;
int ballVelY = 1;
int ballPosX = 2;
int ballPosY = 6;
int padPosX = 4;
int padSpeed =1;

//TURN KNOB

volatile boolean TurnDetected;  // need volatile for Interrupts
volatile boolean rotationDirection;  // CW or CCW rotation

const int PinCLK=2;   // Generating interrupts using CLK signal
const int PinDT=3;    // Reading DT signal
const int PinSW=4;    // Reading Push Button 
int rotaryPosition=0; 


void isr ()  {
  delay(4);  // delay for Debouncing
  if (digitalRead(PinCLK))
    rotationDirection= digitalRead(PinDT);
  else
    rotationDirection= !digitalRead(PinDT);
  TurnDetected = true;
}

void setup() {
  //DISPLAY
  lc.shutdown(0,false);           //  Power-save mode off
  lc.setIntensity(0,2);           //  Set the brightness to a medium values
 
  //TURN KNOB
  pinMode(PinCLK,INPUT);
  pinMode(PinDT,INPUT);  
  pinMode(PinSW,INPUT);
  digitalWrite(PinSW, HIGH); //  Pull-Up resistor for switch
  attachInterrupt (0,isr,FALLING); //  interrupt 0 always connected to pin 2 on Arduino UNO
}

void loop() {
  lc.clearDisplay(0);

  //TURN KNOB

  if (TurnDetected)  {                  // Runs if rotation was detected
    if (rotationDirection) {
      rotaryPosition = rotaryPosition-1;} // decrase Position by 1
    else {
      rotaryPosition = rotaryPosition+1;} // increase Position by 1
      TurnDetected = false;               // do NOT repeat IF loop until new rotation detected
  }
  
// PADDLE
  
  padPosX = padPosX+rotaryPosition;       // paddle position X is old position + turn know rotation
  
  if (padPosX < 5 || padPosX >2) {        // if paddle at edge of screen do not read the knob
    rotaryPosition = 0;
  }
  
  int pad[] = {padPosX-1, padPosX , padPosX +1};   // paddle is middle LED and two on each side
  for (int a = 0; a < 3; a++) {
    lc.setLed(0,pad[a],0,true);                     // display the paddle
  }
  
// BALL

  if (ballPosX > 7 || ballPosX < 0) {             // if the ball goes out of X bounds, reverse its X velocity
   ballVelX =ballVelX*-1;
  }
  
 if (ballPosY > 7 || ballPosY < 0) {               // if the ball goes out of Y bounds, reverse its Y velocity
   ballVelY =ballVelY*-1;
  }
  
  for(int x=0; x < 3; x++) {                      //  if the ball touches the paddle, reverse the velocity
      if (ballPosX == pad[x] && ballPosY == 1 && ballVelY == -1) {
        ballVelY =ballVelY*-1;
    }
  }
   ballPosX = ballPosX + ballVelX;                // move ball on the X axis
   ballPosY = ballPosY + ballVelY;                // move ball on the Y axis
   
   lc.setLed(0,ballPosX,ballPosY,true);           // display the ball
   delay(delaytime);

}
