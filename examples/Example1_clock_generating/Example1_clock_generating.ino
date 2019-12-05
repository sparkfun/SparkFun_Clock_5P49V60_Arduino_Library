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

  Serial.println("Integer Divider: ");
  Serial.println(clockGen._readRegister(FDB_INT_DIV_REG_ONE), BIN);
  Serial.println(clockGen._readRegister(FDB_INT_DIV_REG_TWO), BIN);

  Serial.println("Fractional Divider: ");
  Serial.println(clockGen._readRegister(FDB_FRAC_DIV_REG_ONE), BIN);
  Serial.println(clockGen._readRegister(FDB_FRAC_DIV_REG_TWO), BIN);
  Serial.println(clockGen._readRegister(FDB_FRAC_DIV_REG_THR), BIN);

  Serial.println("Shudown Register:");
  clockGen.refModeControl(ENABLE);
  Serial.println(clockGen._readRegister(SHUTDOWN_REG), BIN);

}

void loop(){
 delay(100); 
}
