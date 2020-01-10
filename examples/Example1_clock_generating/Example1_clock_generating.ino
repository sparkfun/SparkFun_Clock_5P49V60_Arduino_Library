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

  // PLL Divider - we'll assume is 1600MHz BECAUSE 1600MHz/16MHz = 100
  clockGen.setPllFeedbackIntDiv(100);
  Serial.print("0x18 (LSB): ");
  Serial.println(clockGen._readRegister(0x18), BIN);
  Serial.print("0x17 (MSB): ");
  Serial.println(clockGen._readRegister(0x17), BIN);
  Serial.print("Combined: ");
  feedBack = uint16_t(clockGen._readRegister(0x17)) << 3;
  feedBack |= clockGen._readRegister(0x18) >> 4;
  Serial.println(feedBack, BIN);
  clockGen.setIntDivOutOne(25);
  Serial.print("0x18 (LSB): ");
  Serial.println(clockGen._readRegister(0x2E), BIN);
  Serial.print("0x17 (MSB): ");
  Serial.println(clockGen._readRegister(0x2D), BIN);
  Serial.print("Combined: ");
  feedBack = uint16_t(clockGen._readRegister(0x2D)) << 3;
  feedBack |= clockGen._readRegister(0x2E) >> 4;
  Serial.println(feedBack, BIN);
  //clockGen.muxPlltoFodThree();
  //clockGen.clockThrConfigMode(CMOS_MODE);
  //clockGen.clockThrControl(ENABLE);
  //clockGen.setIntDivOutOne(0);

}

void loop(){
  delay(1000); 
}
