/*
  This example code goes through the manual steps of setting the internal
  oscillator. The first two examples demonstrate how to set these values
  without doing math but it's certainly worthwile and even necessary in
  some cases to know how this is done behind the scenes. 
  The following equation is used to determine the divider value to set the
  internal oscillator:  

  (Desired Frequency)/(Crystal Frequency) = Divider Value

  This value divides the Phase Lock Loop within the Clock Generator to get the
  desired internal voltage controlled oscillator frequency. 
  From here we use the equation for the OUTPUT divider to determine the
  OUTPUT value of Clock One (Note: the OUTPUT clock is dependent on the
  internal oscillator):

  ((Internal Oscillator Frequency)/2)/(Desired OUTPUT) = Divider Value

  Finally, set the OUTPUT mode to the desired transmission setup. 

  Available Output Modes: 
  * LVPECL_MODE
  * CMOS_MODE
  * HCSL33_MODE
  * LVDS_MODE
  * CMOS2_MODE
  * CMOSD_MODE
  * HCSL25_MODE

  Pages from Datasheet of Interest: 
  Pg. 22 Transmission Output Termination Setup and Values

  Pages from Programming Guide: 
  Pg. 21 Integer Feedback Divider Calculation
  Pg. 26 Fractional Feedback Divider Calculation

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
  // division: 1600MHz. 1600MHz/16MHz = 100
  Serial.println("Setting Integer Divider.");
  clockGen.setPllFeedbackIntDiv(100);
  Serial.print("Integer Divider set to: ");
  uint16_t fbVal = clockGen.readPllFeedBackIntDiv();
  Serial.println(fbVal);

  // Clock One -----------------------------------------------------
  // To get 16MHz Output = (1600MHz/2)/22MHz = 36.3636
  // Integer portion = 36
  // Fractional portion = .36 -> Need to convert to a HEX value
  // 2^24 * .36 = 6039796.76
  // Round the value to closest integer = 6039797
  clockGen.setIntDivOutOne(36);
  clockGen.setFractDivFodOne(6039797);
  Serial.print("FOD One Integer Divider: ");
  Serial.println(clockGen.readIntDivOutOne());
  Serial.print("FOD One Fractional Divider: ");
  Serial.println(clockGen.readFractDivFodOne());
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
