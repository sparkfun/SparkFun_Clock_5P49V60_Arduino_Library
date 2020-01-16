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
  // Enable the VCO with the new settings. 
  clockGen.calibrateVco();
  // Clock One
  clockGen.setIntDivOutOne(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutOne());
  // Clock Two--------------------------------------------------
  clockGen.setIntDivOutTwo(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutTwo());
  clockGen.muxPllToFodTwo();
  clockGen.clockTwoConfigMode(CMOS_MODE);
  clockGen.clockTwoControl(ENABLE);
  // -----------------------------------------------------------
  // Clock Three------------------------------------------------
  clockGen.setIntDivOutThree(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutThree());
  clockGen.muxPllToFodThree();
  clockGen.clockThrConfigMode(CMOS_MODE);
  clockGen.clockThrControl(ENABLE);
  // -----------------------------------------------------------
  // Clock Four-------------------------------------------------
  clockGen.setIntDivOutFour(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutFour());
  clockGen.muxPllToFodFour();
  clockGen.clockFourConfigMode(CMOS_MODE);
  clockGen.clockFourControl(ENABLE);
  // -----------------------------------------------------------

}

void loop(){
  delay(1000); 
}
