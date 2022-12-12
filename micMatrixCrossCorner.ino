// include the library and assign the pins

#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);

// declare variables

int analogValue = 0;
int analogValueAbs = 0;
int digitalValue = 0;
int voLevelMin = 398;
int voLevelMax = 500;
int counter = 0;
int y = 0;
int mapAnal = 0;
int mapAnalRad = 0;
int lagFlag = false;
int lagCounter = 0;

void setup() {
  Serial.begin(9600);
  //initiate LCD screen

  lc.shutdown(0, false);
  lc.setIntensity(0, 2);

  // setup pins for the mike

  pinMode(7, INPUT);
}

void loop() {
  lc.clearDisplay(0);

  //GET VALUES FOR FROM THE MIKE
  analogValue = analogRead(A1);
  digitalValue = digitalRead(7);

  // GET ABSOLUTE VALUE FROM THE ANALOG SOUND READ ELIMINATING NEGATIVE VALUES
  analogValueAbs = abs(analogValue);

  // MAP VALUES FOR THE 2 DISPLAY MODES
  mapAnal = map(analogValueAbs, voLevelMin, voLevelMax, 1, 3);
  mapAnalRad = map(analogValueAbs, voLevelMin, voLevelMax, 0, 7);

  // LAG FLAG SWITCHES BETWEEN THE TWO DISPLAY MODES
  if (lagFlag == true) {

    // RADIANT CORNER PATTERN

    lc.setLed(0, mapAnalRad, 0, true);
    lc.setLed(0, 0, mapAnalRad, true);
    lc.setLed(0, mapAnalRad, mapAnalRad, true);
  } else {

    // 4 ARM STAR PATTERN
    //1.30 OCLOCK
    lc.setLed(0, 3 + mapAnal, 3 + mapAnal, true);
    //4.30 OCLOCK
    lc.setLed(0, 3 + mapAnal, 4 - mapAnal, true);
    //7.30 OCLOCK
    lc.setLed(0, 4 - mapAnal, 4 - mapAnal, true);
    //10.30 OCLOCK
    lc.setLed(0, 4 - mapAnal, 3 + mapAnal, true);
  }

  // ONCE lagCounter REACHES 20, IT SWITCHES THE lagFlag
  lagCounter++;
  if (lagCounter > 20) {
    if (lagFlag == true) {
      lagFlag = false;
    } else {
      lagFlag = true;
    }
    lagCounter = 0;
  }
  delay(150);
}
