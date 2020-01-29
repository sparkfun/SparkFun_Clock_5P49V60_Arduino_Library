/*
  This example code first disables the xtal as the reference clock for the
  internal voltage controlled osicllator. The clock generator can be fed a
  single CMOS or LVPECL Driver through the mini PTH hole labled "ClKIN" or a 
  with a differential clock through CLKIN and CLKINB (HCSL/LVPECL/LVDS). 
  The relevant datasheet pages on how this is done are listed below. Keep 
  in mind that the clock must not exceed 1.2V peak to peak. 

  Available Output Modes: 
  * LVPECL_MODE
  * CMOS_MODE
  * HCSL33_MODE
  * LVDS_MODE
  * CMOS2_MODE
  * CMOSD_MODE
  * HCSL25_MODE

  Pages from Datasheet of Interest:
  Pg. 20 - 22 Input - Driving with XIN/REF or CLKIN
  Pg. 22 Transmission Output Termination Setup and Values

  SparkFun Electronics
  Date: February, 2020
  Author: Elias Santistevan
*/

#include <Wire.h>
#include "SparkFun_5P49V60.h"

// Uses default address at 0x6A, alternate available at 0x68
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
  
  clockGen.clockInControl(ENABLE);
  // Assuming 12MHz input: 
  Serial.println("Setting Internal Clock Frequency to 1600MHz.");
  clockGen.setVcoFrequency(1200.0); // Give float value in MHz.

  // Clock One -----------------------------------------------------
  // Use internal phase lock loop for clock output calculation.
  clockGen.muxPllToFodOne();
  Serial.println("Setting Output Mode to LVPECL.");
  // There are many OUTPUT modes available for each clock - this example uses
  // LVPECL (Low voltage Positive Emitter Coupled Logic) mode.
  clockGen.clockOneConfigMode(LVPECL_MODE);
  Serial.println("Setting Clock One Frequency to 16MHz.");
  clockGen.setClockOneFreq(6.0); // Give float value in MHz, 6.0 = 6000000Hz or 6MHz
  // --------------------------------------------------------------

}

void loop(){

  delay(1000); 

}
