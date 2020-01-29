/*
  This example code first sets the internal oscillator to 1600MHz.
  It then sets the output for Clock One to 16MHz and then sets the output
  MODE to LVPECL (Low Voltage Positive Emitter Coupled Logic). 
  
  The frequency from clock one is multiplexed to the output divider of clock two.
  This method is the only method for creating signals below 1MHz. The
  divider settings for clock one are explicitly done, as demonstrated in example 3,
  so that the function calls to further divide clock one's output with clock
  two's output divider aren't hidden behind higher order function calls. 

  Finally, the frequency from clock two is multiplexed to the output of clock
  three. This bypasses clock three's output divider and so clock
  three doesn't need to be enabled. This method is handy when you want to
  use one generated clock signal on up to four outputs!
  

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

  Pages from the Programming Guide:
  Pg. 30-32 Output Divider Control Register Settings

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
  clockGen.setVcoFrequency(1600.0); // Give float value in MHz.

  // Clock One -----------------------------------------------------
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

  // Clock Two -----------------------------------------------------
  // This function call multiplexes the settings from Clock One's Output
  // Divider to the Output Divider of Clock Two. In this way you can divide a
  // low frequency to get even lower frequencies. The settings for the Clock Two are
  // set to zero so will output a 16MHz frequency.  
  clockGen.muxFodOneToFodTwo();
  clockGen.clockTwoConfigMode(LVPECL_MODE);
  clockGen.clockTwoControl(ENABLE);
  // --------------------------------------------------------------

  // Clock Three -----------------------------------------------------
  // This function call multiplexes the output from clock two to the output of
  // clock three. If you want to duplicate a signal this may be the best
  // option. Notice that you don't need to enable clock three to see an output
  // on this line. 
  clockGen.muxOutTwoToOutThree();
  clockGen.clockThrConfigMode(LVPECL_MODE);
  // --------------------------------------------------------------

}

void loop(){
  delay(1000); 
}
