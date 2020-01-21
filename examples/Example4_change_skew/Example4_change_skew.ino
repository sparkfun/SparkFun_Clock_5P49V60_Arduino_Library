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

  // Fist, Setting the internal oscillator to a value that makes for easier
  // divisors (100 in this case).
  // PLL Divider - To get 1600MHz (1.6GHz): 1600MHz/16MHz (Clock) = 100
  clockGen.setPllFeedbackIntDiv(100);
  Serial.print("Feedback Integer Divider for PLL: ");
  Serial.println(clockGen.readPllFeedBackIntDiv());

  // Enable the VCO with the new settings. 
  // PLL will not take new settings without this function call.
  clockGen.calibrateVco();

  // Clock One General Settings------------------------------------
  // To get 16MHz Output = (1600MHz/2)/16MHz = 50
  clockGen.setIntDivOutOne(50);
  Serial.print("FOD One Integer Divider: ");
  Serial.println(clockGen.readIntDivOutOne());
  clockGen.muxPllToFodOne();
  // There are many OUTPUT modes available for each clock - this example uses
  // LVPECL (Low voltage Positive Emitter Coupled Logic) mode and terminates 
  // the clock with a 100Ohm resistance to GND.
  clockGen.clockOneConfigMode(LVPECL_MODE);
  clockGen.clockOneControl(ENABLE);
  // --------------------------------------------------------------

  // Clock Two General Settings------------------------------------
  // To get 16MHz Output = (1600MHz/2)/16MHz = 50
  clockGen.setIntDivOutOne(50);
  Serial.print("FOD One Integer Divider: ");
  Serial.println(clockGen.readIntDivOutTwo());
  clockGen.muxPllToFodTwo();
  // There are many OUTPUT modes available for each clock - this example uses
  // LVPECL (Low voltage Positive Emitter Coupled Logic) mode and terminates 
  // the clock with a 100Ohm resistance to GND.
  clockGen.clockTwoConfigMode(LVPECL_MODE);
  clockGen.clockTwoControl(ENABLE);
  // --------------------------------------------------------------
  
  // Clock One Skew------------------------------------------------
  // Time of one oscillation of clock one and two is  (1/f = t) = 62.5ns
  // Desired skew = 10ns 
  // 360 degress * (10ns/62.5ns) = 57.6 degrees
  // Output Divider for VCO = 100
  // 360 degrees /100 = 36 degrees
  // Total Skew = 57.6/36 = 1.6 degrees 
  // Integer Portion = 1
  // Fractional Portion = .6 
  clockGen.setIntDivSkewOne(1);
  clockGen.setFractDivSkewOne(.6);
  clockGen.globalReset();
  Serial.print("Integer portion of Skew One: ");
  Serial.println(clockGen.readIntDivSkewOne());
  Serial.print("Fractional portion of Skew One: ");
  Serial.println(clockGen.readFractDivSkewOne());
  //---------------------------------------------------------------
}

void loop(){
  delay(1000); 
}
