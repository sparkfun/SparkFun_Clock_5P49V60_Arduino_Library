#include <Wire.h>
#include "SparkFun_5P49V60.h"
#define DEF_ADDR 0xD4 

SparkFun_5P49V60 clockGen(DEF_ADDR);

void setup(){

  Wire.begin();
  Serial.begin(115200);

  if (clockGen.begin() == false){
    Serial.println("Clock Generator Ready.");
  }
  else{
    Serial.println("Could not communicate with the SparkFun Clock Generator.");
    while(1);
  }
  uint8_t value = clockGen.readBurnedBit();
  Serial.print("Burned bit: ");
  Serial.println(value, BIN);

}

void loop(){
 delay(100); 
}
