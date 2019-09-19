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

  uint8_t reg_val = _readRegister(OTP_CONTROL_REG);
  reg_val &= ~MASK_ONE;
  return reg_val;

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

// INCOMPLETE, This function returns the total load capacitance on the given
// pin of the onboard crystal.
float SparkFun_Clock::readCrystalCapVal(uint8_t pin){

  uint8_t _register;

  if (pin == XIN)
    _register = LOAD_CAP_REG_ONE;
  else if (pin == XOUT)
    _register = LOAD_CAP_REG_TWO;
  else return;

  uint8_t reg_val = _readRegister(_register);
  reg_val &= MASK_THREE;

  if (reg_val == 1){
    return cap_arr[0];
  }
  else if (reg_val == 2){
    return cap_arr[1];
  }
  else if (reg_val == 3){
    return cap_arr[0] + cap_arr[1];
  }
  else if (reg_val == 4){
    return cap_arr[2];
  }
  else if (reg_val == 5){
    return cap_arr[0] + capArray[2];
  }
  else if (reg_val == 6){
    return cap_arr[1] + cap_arr[2];
  }
  else return ERROR;

}


// This function changes the primary source of the clock: either crystal
// or CLKIN. The product is set by default in hardware to use the crystal.
void SparkFun_Clock::changeSource(uint8_t source){

  if (source == CLOCK_SOURCE || source == XTAL_SOURCE) {}
  else return;

  _writeRegister(LOAD_CAP_REG_TWO, MASK_THREE, CLOCK_SOURCE, POS_ONE);

}

// This function returns the currently selected source of the clock: either crystal
// or CLKIN. The product is set by default in hardware to use the crystal.
uint8_t SparkFun_Clock::readSource(){

  uint8_t reg_val = _readRegister(LOAD_CAP_REG_TWO);
  reg_val &= (~MASK_TWO);
  reg_val >>= POS_ONE;
  return reg_val;

}

// REG 0x15, bits[7:0] This selects which divider value shall be used for the clock
// output. This is strange, but the two divider is in the eighth bit position,
// and values of 3-127 are written to bits[6:0]. If the two divider is enabled
// then the other divider is disabled - this bit can not be set and then the
// others as well. 
void SparkFun_Clock::selectRefDivider(uint8_t div_val){

  if (div_val < 2 || div_val > 127) 
    return;

  if (div_val == 2)
    _writeRegister(REF_DIVIDER_REG, MASK_EIGHT_MSB, state, POS_SEVEN);
  else 
    _writeRegister(REF_DIVIDER_REG, (~MASK_EIGHT_MSB), div_val, POS_ZERO); 

}

// REG 0x15, bits[7:0] This selects which divider value is used for the clock
// output. 
uint8_t SparkFun_Clock::readRefDivider(){

  uint8_t reg_val = _readRegister(REF_DIVIDER_REG);

  // Divider 2 disables other divider. 
  if ((reg_val & (~MASK_EIGHT_MSB)) >> POS_SEVEN == 1)
    return 2; 
  else 
    return reg_val &= MASK_EIGHT_MSB;

}

// REG 0x16, bit[7] This function enables the function to  bypass the clock pre-divider, 
// which is essentially a divide by 1.  
void SparkFun_Clock::bypassRefDivider(uint8_t div){

  if( div == DISABLE || div == ENABLE) {}
  else return; 

  _writeRegister(DIVIDER_VCO_REG, MASK_EIGHT_MSB, div, POS_SEVEN);

}

// REG 0x16, bit[7] This function reads whether the clock pre-divider is being
// bypassed. 
uint8_t SparkFun_Clock::readBypassDivider(){

  uint8_t reg_val = _readRegister(DIVIDER_VCO_REG);
  reg_val &= (~MASK_EIGHT_MSB);
  reg_val >>= POS_SEVEN; 

  return reg_val; 

}

// REG 0x11, bit[5] This function turns on the VCO test mode.
void SparkFun_Clock::vcoTestControl(uint8_t cont){
  
  if (cont == ENABLE || cont == DISABLE){}
  else return; 

  _writeRegister(VCO_BAND_REG, MASK_TWO_MSB, cont, POS_FIVE);

}

// REG 0x11, bit[5] This function checks to see if the VCO test mode is running. 
uint8_t SparkFun_Clock::readTestControl(){
  
  uint8_t reg_val = _readRegister(VCO_BAND_REG); 
  reg_val &= (~MASK_TWO);
  reg_val >>= POS_FIVE; 
  return reg_val; 

}

// Reg 0x17 and 0x16, bits[7:0] and bits[7:4] respectively - largest value that
// can be set: 4095. 
void SparkFun_Clock::setPllFeedbackIntDiv(uint16_t divider_val){

  if (divider_val < 0 || divider_val > 4095)
    return;

  if (divider_val < 15){
    _writeRegister(FDB_INT_DIV_REG_TWO, MASK_FIFT_MSB, divider_val, POS_FOUR);
    _writeRegister(FDB_INT_DIV_REG_TWO, MASK_ALL, 0, POS_ZERO); 
  }
  else {
    uint16_t lsb_div_val = divider_val & MASK_FIFT;
    _writeRegister(FDB_INT_DIV_REG_TWO, MASK_FIFT_MSB, lsb_div_val, POS_FOUR); 
    uint16_t = (divider_val & MASK_FIFT_MSB) >> POS_FOUR; 
    _writeRegister(FDB_INT_DIV_REG_ONE, MASK_ALL, msb_div_val, POS_ZERO); 
  }

}

// REG 0x19, 0x1A, and 0x1B, bits[8:0] for all three registers.
void SparkFun_Clock::setPllFeedBackFractDiv(uint32_t divider_val){

  if (divider_val < 0 || divider_val > 16,777,215)
    return;

  if (divider_val <= 255){
    _writeRegister(FDB_FRAC_DIV_REG_THR, MASK_ALL, divider_val, POS_ZERO);
    _writeRegister(FDB_FRAC_DIV_REG_TWO, MASK_ALL, 0, POS_ZERO);
    _writeRegister(FDB_FRAC_DIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 65535){
    uint32_t lsb_div_val = divider_val & MASK_FIFT_MSB; 
    _writeRegister(FDB_FRAC_DIV_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    uint32_t msb_div_val = divider_val & MASK_ALL; 
    msb_div_val >>= POS_SEVEN;
    _writeRegister(FDB_FRAC_DIV_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);
    uint32_t mmsb_div_val = divider_val & MASK_FIFT_MSB; 
    _writeRegister(FDB_FRAC_DIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
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
  _reg_value = _i2cPort->read();
  return(_reg_value);

}
