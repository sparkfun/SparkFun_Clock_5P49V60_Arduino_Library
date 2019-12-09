#include <Wire.h>
#include "SparkFun_5P49V60.h"

SparkFun_5P49V60 clockGen;

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

 // Serial.println("Feedback Integer Divider: ");
 // Serial.println(clockGen._readRegister(FDB_INT_DIV_REG_ONE), BIN);
 // Serial.println(clockGen._readRegister(FDB_INT_DIV_REG_TWO), BIN);

 // Serial.println("Fractional Divider: ");
 // Serial.println(clockGen._readRegister(FDB_FRAC_DIV_REG_ONE), BIN);
 // Serial.println(clockGen._readRegister(FDB_FRAC_DIV_REG_TWO), BIN);
 // Serial.println(clockGen._readRegister(FDB_FRAC_DIV_REG_THR), BIN);

  Serial.println("Shudown Register:");
  clockGen.refModeControl(ENABLE);
  Serial.println(clockGen._readRegister(SHUTDOWN_REG), BIN);


  clockGen.setPllFeedbackIntDiv(62);
  Serial.println("Feedback Integer Divider: ");
  Serial.println(clockGen._readRegister(FDB_INT_DIV_REG_ONE), BIN);
  Serial.println(clockGen._readRegister(FDB_INT_DIV_REG_TWO), BIN);
  //clockGen.calibrateVco();
  clockGen.setIntDivOutOne(16);
  Serial.println("Integer Divide for FOD1:");
  Serial.println(clockGen._readRegister(OUT_IDIV_REG_ONE), BIN);
  Serial.println(clockGen._readRegister(OUT_IDIV_REG_TWO), BIN);

  clockGen.fodOutOneCont(ENABLE);
  clockGen.fodOutOutFodTwo();
  //clockGen.fodOutOutThree();
  //clockGen.fodOutOutFour();

  clockGen.resetFodOne();
  clockGen.resetFodTwo();
  //clockGen.resetFodThree();
  //clockGen.resetFodFour();

  Serial.println("Clock Out One: ");
  Serial.println(clockGen._readRegister(DIV_ONE_CONTROL_REG), BIN);
  Serial.println("Clock Out Two: ");
  Serial.println(clockGen._readRegister(DIV_TWO_CONTROL_REG), BIN);
  Serial.println("Clock Out Three: ");
  Serial.println(clockGen._readRegister(DIV_THR_CONTROL_REG), BIN);
  Serial.println("Clock Out Four: ");
  Serial.println(clockGen._readRegister(DIV_FOUR_CONTROL_REG), BIN);
  Serial.println("Aux Enable One: ");
  Serial.println(clockGen._readRegister(OUT_ISKEW_REG_TWO), BIN);
  Serial.println("Aux Enable Two: ");
  Serial.println(clockGen._readRegister(OUT_ISKEW_TWO_REG_TWO), BIN);
  Serial.println("Aux Enable Three: ");
  Serial.println(clockGen._readRegister(OUT_ISKEW_THR_REG_TWO), BIN);
  Serial.println("Aux Enable Four: ");
  Serial.println(clockGen._readRegister(OUT_ISKEW_FOUR_REG_TWO), BIN);
}

void loop(){
 delay(100); 
}
