/*
  This example code first sets the internal oscillator to 1600MHz.
  It then sets the output for Clock One to 8MHz and the output for 
  Clock Two to 16MHz. Finally the output MODE is set to LVPECL (Low Voltage
  Positive Emitter Coupled Logic). 

  Available Output Modes: 
  * LVPECL_MODE
  * CMOS_MODE  
  * HCSL33_MODE
  * LVDS_MODE  
  * CMOS2_MODE 
  * CMOSD_MODE 
  * HCSL25_MODE

  Pages from Hardware Datasheet of Interest: 
  Pg. 22 Transmission Output Termination Setup and Values

  SparkFun Electronics
  Date: February, 2020
  Author: Elias Santistevan
*/

#include <Wire.h>
#include "SparkFun_5P49V60.h"

// Uses default address at 0x6A, alternate available at 0x68
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

  // Fist, Setting the internal oscillator to a known value. 
  Serial.println("Setting Internal Clock Frequency to 1600MHz.");
  clockGen.setVcoFrequency(1600.0); // Give float value in MHz 

  // Clock One----------------------------------------------------
  // Use internal phase lock loop for clock output calculation.
  clockGen.muxPllToFodOne();
  Serial.println("Setting Output Mode for Clock One to LVPECL.");
  // There are many OUTPUT modes available for each clock - this example uses
  // LVPECL (Low voltage Positive Emitter Coupled Logic) mode.
  clockGen.clockOneConfigMode(LVPECL_MODE);
  Serial.println("Setting Clock One Frequency to 8MHz.");
  clockGen.setClockOneFreq(8.0); // Give float value in MHz, 8.0 = 8000000Hz or 8MHz
  // --------------------------------------------------------------

  // Clock Two--------------------------------------------------
  // Use internal phase lock loop for clock output calculation.
  clockGen.muxPllToFodOne();
  Serial.println("Setting Output Mode for Clock Two to LVPECL.");
  clockGen.clockTwoConfigMode(LVPECL_MODE);
  Serial.println("Setting Clock Two Frequency to 16MHz.");
  clockGen.setClockOneFreq(16.0); // Give float value in MHz, 16.0 = 16000000Hz or 16MHz
  // -----------------------------------------------------------

}

void loop(){
  delay(1000); 
}
