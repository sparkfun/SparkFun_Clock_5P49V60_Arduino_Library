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
  
  Serial.print("0x10 Before: ");
  Serial.println(clockGen._readRegister(0x10), BIN);
  clockGen.xtalControl(DISABLE);
  clockGen.clockInControl(ENABLE);
  Serial.print("0x10 After: ");
  Serial.println(clockGen._readRegister(0x10), BIN);

  // PLL Divider - To get 1200MHz: 1200MHz/12MHz (Clock) = 100
  clockGen.setPllFeedbackIntDiv(100);
  Serial.print("Feedback Integer Divider for PLL: ");
  Serial.println(clockGen.readPllFeedBackIntDiv());
  // Enable the VCO with the new settings. 
  clockGen.calibrateVco();
  // Clock One
  // 1200/2/OUTx = 100 -> 600/OUTx = 100 -> 6MHz
  clockGen.setIntDivOutOne(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutOne());

}

void loop(){
  delay(1000); 
}
