/*
  This is a library for...
  By: Elias Santistevan
  Date: 
  License: This code is public domain but you buy me a beer if you use this and 
  we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
 */

#include "SparkFun_Clock.h"

SparkFun_Clock::Class_Name(uint8_t address){  _address = address; } //Constructor for I2C

bool SparkFun_Clock::begin( TwoWire &wirePort )
{
  
  _i2cPort = &wirePort;

  _i2cPort->beginTransmission(_address);
  delay(100); // 100ms Startup time
  uint8_t _ret = _i2cPort->endTransmission();
  if( !_ret )
    return true; 
  else 
    return false; 

}

//
void SparkFun_Clock:changeI2CAddress(uint8_t addr_selec){

  if (addr_selec != 0 || addr_selec != 1)
    return;
  
  _writeRegister(OTP_CONTROL_REG, OTP_ADDR_MASK, addr_selec, POS_ZERO);

}

//
uint8_t SparkFun_Clock:readI2CAddress(){

  uint8_t regVal = _readRegister(OTP_CONTROL_REG);
  regVal &= ~OTP_ADDR_MASK;
  return regVal;

}

//
void SparkFun_Clock::sdActiveState(uint8_t state){
  
  if (state != HIGH || state != LOW)
    return;

  _writeRegister(SHUTDOWN_REG, 
}
// This generic function handles I2C write commands for modifying individual
// bits in an eight bit register. Paramaters include the register's address, a mask 
// for bits that are ignored, the bits to write, and the bits' starting
// position.
void SparkFun_Clock::_writeRegister(uint8_t _wReg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition)
{
  _i2cWrite = readRegister(_wReg); // Get the current value of the register
  _i2cWrite &= (_mask); // Mask the position we want to write to.
  _i2cWrite |= (_bits << _startPosition);  // Write the given bits to the variable
  _i2cPort->beginTransmission(_address); // Start communication.
  _i2cPort->write(_wReg); // at register....
  _i2cPort->write(_i2cWrite); // Write register...
  _i2cPort->endTransmission(); // End communcation.
}

// This generic function reads an eight bit register. It takes the register's
// address as its' parameter. 
uint8_t SparkFun_Clock::_readRegister(uint8_t _reg)
{

  _i2cPort->write(_reg); // Moves pointer to register.
  _i2cPort->endTransmission(false); // 'False' here sends a restart message so that bus is not released
  _i2cPort->requestFrom(_address, 1); // Read the register, only ever once. 
  _regValue = _i2cPort->read();
  return(_regValue);

}
