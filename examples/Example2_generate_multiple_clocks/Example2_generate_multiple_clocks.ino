/*
  This example code first sets the internal oscillator to 1600MHz using the
  following equation:
  (Desired Frequency)/(Crystal Frequency) = Divider Value
  This value divides the Phase Lock Loop within the Clock Generator to get the
  desired internal voltage controlled oscillator frequency. 
  From here we can then use the equation for the OUTPUT divider to determine the
  OUTPUT value of each clock:
  ((Internal Oscillator Frequncy)/2)/(Desired OUTPUT) = Divider Value
  Pages from Hardware Datasheet of Interest: 
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

  // Clock One----------------------------------------------------
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

  // Clock Two--------------------------------------------------
  // To get 16MHz Output = (1600MHz/2)/16MHz = 50
  clockGen.setIntDivOutTwo(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutTwo());
  clockGen.muxPllToFodTwo();
  clockGen.clockTwoConfigMode(LVPECL_MODE);
  clockGen.clockTwoControl(ENABLE);
  // -----------------------------------------------------------

  // Clock Three------------------------------------------------
  // To get 16MHz Output = (1600MHz/2)/16MHz = 50
  clockGen.setIntDivOutThree(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutThree());
  clockGen.muxPllToFodThree();
  clockGen.clockThrConfigMode(LVPECL_MODE);
  clockGen.clockThrControl(ENABLE);
  // -----------------------------------------------------------

  // Clock Four-------------------------------------------------
  // To get 16MHz Output = (1600MHz/2)/16MHz = 50
  clockGen.setIntDivOutFour(100);
  Serial.print("FOD One Divider: ");
  Serial.println(clockGen.readIntDivOutFour());
  clockGen.muxPllToFodFour();
  clockGen.clockFourConfigMode(LVPECL_MODE);
  clockGen.clockFourControl(ENABLE);
  // -----------------------------------------------------------

}

void loop(){
  delay(1000); 
}
