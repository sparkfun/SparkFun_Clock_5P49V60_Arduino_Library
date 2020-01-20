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

  // Clock One -----------------------------------------------------
  // To get 16MHz Output = (1600MHz/2)/22MHz = 36.3636
  // Integer portion = 36
  clockGen.setIntDivOutOne(36);
  Serial.print("FOD One Integer Divider: ");
  Serial.println(clockGen.readIntDivOutOne());
  clockGen.muxPllToFodOne();
  // There are many OUTPUT modes available for each clock - this example uses
  // LVPECL (Low voltage Positive Emitter Coupled Logic) mode and terminates 
  // the clock with a 100Ohm resistance to GND.
  clockGen.clockOneConfigMode(LVPECL_MODE);
  clockGen.clockOneControl(ENABLE);
  // --------------------------------------------------------------
}

void loop(){
  delay(1000); 
}
