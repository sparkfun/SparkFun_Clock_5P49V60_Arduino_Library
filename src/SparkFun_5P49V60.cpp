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

  _writeRegister(OTP_CONTROL_REG, MASK_ONE, addr_selec, POS_ZERO);

}

//
uint8_t SparkFun_Clock:readI2CAddress(){

  uint8_t regVal = _readRegister(OTP_CONTROL_REG);
  regVal &= ~MASK_ONE;
  return regVal;

}

//
void SparkFun_Clock::sdActiveState(uint8_t state){

  if (state != HIGH || state != LOW)
    return;

  _writeRegister(SHUTDOWN_REG, MASK_ONE, state, POS_ONE)

}

// This function allows the given clock line to remain on when the SparkFun
// Clock Generator is put into shutdown mode.
void SparkFun_Clock::persEnableClock(uint8_t clock){

  uint8_t _bit_pos;
  uint8_t _mask;

  if (clock < 0 || clock > 4)
    return;

  if (clock == 0) {
    _bit_pos = POS_SEVEN;
    _mask = MASK_EIGHT_MSB;
  }
  else if (clock == 1){
    _bit_pos = POS_SIX;
    _mask = MASK_FOUR_MSB;
  }
  else if (clock == 2) {
    _bit_pos = POS_FIVE;
    _mask = MASK_TWO_MSB;
  }
  else if (clock == 3){
    _bit_pos = POS_FOUR;
    _mask = MASK_ONE_MSB;
  }
  else (clock == 4){
    _bit_pos = POS_THREE;
    _mask = MASK_FOUR;
  }

  _writeRegister(CLK_OE_FUNC_REG, _mask, clock, _bit_pos);
}


// This function adds the available internal capacitors to the given pin on the
// crystal. The two available pins can be taken directly from the datasheet, or
// schematic: XIN and XOUT.
void SparkFun_Clock::addCrystalLoadCap(uint8_t pin, float capVal){

  uint8_t _register;
  uint8_t _bit_pos;
  uint8_t _mask;

  if (pin == XIN)
    _register = LOAD_CAP_REG_ONE;
  else if (pin == XOUT)
    _register = LOAD_CAP_REG_TWO;
  else return;

  if (capVal == 1){
    _bit_pos = POS_TWO;
    _mask = MASK_FOUR;
  }
  else if (capVal == 2){
    _bit_pos = POS_THREE;
    _mask = MASK_EIGHT;
  }
  else if (capVal == 3){
    _bit_pos = POS_FOUR;
    _mask = MASK_ONE_MSB;
  }
  else if (capVal == 4){
    _bit_pos = POS_FIVE;
    _mask = MASK_TWO_MSB;
  }
  else if (capVal == 5){
    _bit_pos = POS_SIX;
    _mask = MASK_FOUR_MSB;
  }
  else if (capVal == 6){
    _bit_pos = POS_SEVEN;
    _mask = MASK_EIGHT_MSB;
  }
  else return;

  _writeRegister(_register, _mask, capVal, _bit_pos);

}

//
float SparkFun_Clock::readCrystalCapVal(uint8_t pin){

  uint8_t _register;

  if (pin == XIN)
    _register = LOAD_CAP_REG_ONE;
  else if (pin == XOUT)
    _register = LOAD_CAP_REG_TWO;
  else return;

  uint8_t regVal = _readRegister(_register);
  regVal &= MASK_THREE;

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
