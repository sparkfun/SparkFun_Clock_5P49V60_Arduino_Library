#ifndef _SPARKFUN_CLOCK_GEN_5P49V60_
#define _SPARKFUN_CLOCK_GEN_5P49V60_

#include <Wire.h>
#include <Arduino.h>

#define 
#define 
#define 

enum {
};

class SparkFun_Clock
{  
  public:
    
    // Public Variables
    
    //Function declarations
    SparkFun_Clock(uint8_t address); // I2C Constructor

    bool begin(TwoWire &wirePort = Wire); // begin function

  private:
    
    // Private Variables
    uint8_t _address;

    // This generic function handles I2C write commands for modifying individual
    // bits in an eight bit register. Paramaters include the register's address, a mask 
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void writeRegister(uint8_t, uint8_t, uint8_t, uint8_t);

    // This generic function reads an eight bit register. It takes the register's
    // address as its' parameter. 
    uint8_t readRegister(uint8_t);

    TwoWire *_i2cPort;
};
#endif
