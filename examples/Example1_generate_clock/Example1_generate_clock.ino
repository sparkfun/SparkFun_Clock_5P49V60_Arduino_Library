/*
  This example code first sets the internal oscillator to 1600MHz using the
  following equation:
  (Desired Frequency)/(Crystal Frequency) = Divider Value
  This value divides the Phase Lock Loop within the Clock Generator to get the
  desired internal voltage controlled oscillator frequency. 
  From here we can then use the equation for the OUTPUT divider to determine the
  OUTPUT value of Clock One:
  ((Internal Oscillator Frequncy)/2)/(Desired OUTPUT) = Divider Value
  Pages from Datasheet of Interest: 
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

  // Fist, Setting the internal oscillator to a known value that makes for easy
  // division: 1600MHz
  clockGen.setVcoFrequency(1600.0); // Give values in MHz - A given value of 16

  // Clock One -----------------------------------------------------
  // To get 16MHz Output = (1600MHz/2)/16MHz = 50
  clockGen.setIntDivOutOne(50);
  Serial.print("FOD One Divider: ");
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
