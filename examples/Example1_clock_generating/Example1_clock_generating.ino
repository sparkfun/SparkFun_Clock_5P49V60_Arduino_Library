#include "SparkFun_5P49V60.h"
#include <Wire.h>

SparkFun_5P49V60 clock();

void setup(){

  Wire.begin();
  Serial.begin(115200);

  if (clock.begin() == false){
    Serial.println("Clock Generator Ready.");
  }
  else{
    Serial.println("Could not communicate with the SparkFun Clock Generator.");
    while(1);
  }

}

void loop(){
 delay(100); 
}
