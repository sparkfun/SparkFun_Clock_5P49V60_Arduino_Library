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
  // 1600/2/OUTx = 100 -> 800/OUTx = 100 -> 8MHz
  clockGen.setIntDivOutOne(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutOne());

  clockGen.setIntDivOutTwo(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutTwo());
  clockGen.muxPllToFodTwo();
  clockGen.clockTwoConfigMode(CMOS_MODE);
  clockGen.clockTwoControl(ENABLE);

  Serial.println("Setting Slew Rate.");
  clockGen.setIntDivSkewTwo(1000);
  Serial.print("Slew Rate Set: ");
  Serial.println(clockGen.readIntDivSkewTwo());
  clockGen.globalReset();

}

void loop(){
  delay(1000); 
}
