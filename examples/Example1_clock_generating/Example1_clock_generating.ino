#include <Wire.h>
#include "SparkFun_5P49V60.h"

SparkFun_5P49V60 clockGen;
uint16_t feedBack; 

void setup(){

  Wire.begin();
  Serial.begin(115200);

  if (clockGen.begin() == true){
    Serial.println("Clock Generator Ready.");
  }
  else {
    Serial.println("Could not communicate with the SparkFun Clock Generator.");
    while(1);
  }

  // PLL Divider - To get 1600MHz: 1600MHz/16MHz (Clock) = 100
  clockGen.setPllFeedbackIntDiv(100);
  Serial.print("Feedback Integer Divider for PLL: ");
  Serial.println(clockGen.readPllFeedBackIntDiv());
  clockGen.calibrateVco();
  clockGen.setIntDivOutOne(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutOne());
  clockGen.muxOutOneToOutTwo();
  clockGen.clockTwoConfigMode(CMOS_MODE);
  clockGen.muxOutTwoToOutThree();
  clockGen.clockThrConfigMode(CMOS_MODE);
  clockGen.muxOutThreeToFodFour();
  clockGen.clockFourConfigMode(CMOS_MODE);


}

void loop(){
  delay(1000); 
}
