/*
  This is a library for...
  By: Elias Santistevan
  Date:
  License: This code is public domain but you buy me a beer if you use this and
  we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
 */

#include "SparkFun_5P49V60.h"

SparkFun_5P49V60::SparkFun_5P49V60(uint8_t address) : _address(address)
{ }

bool SparkFun_5P49V60::begin(TwoWire &wirePort)
{
  _i2cPort = &wirePort;
  _i2cPort->beginTransmission(_address);
  delay(100); // 100ms Startup time
  uint8_t _ret = _i2cPort->endTransmission();
  if (!_ret)
    return true;
  else
    return false;
}

// Reg 0x00, bit[0]
void SparkFun_5P49V60::changeI2CAddress(uint8_t addr_selec){
  if (addr_selec == 0 || addr_selec == 1)
    _writeRegister(OTP_CONTROL_REG, MASK_ONE, addr_selec, POS_ZERO);
}

// Reg 0x00, bit[0]
uint8_t SparkFun_5P49V60::readI2CAddress(){
  uint8_t reg_val = _readRegister(OTP_CONTROL_REG);
  reg_val &= ~MASK_ONE;
  return reg_val;
}

// Reg 0x00, bit[7]
uint8_t SparkFun_5P49V60::readBurnedBit(){
  uint8_t reg_val = _readRegister(OTP_CONTROL_REG);
  reg_val &= ~MASK_EIGHT_MSB;
  reg_val >>= POS_SEVEN;
  return reg_val;
}

// Reg 0x10, bit[7]
void SparkFun_5P49V60::xtalControl(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(SHUTDOWN_REG, MASK_EIGHT_MSB, control, POS_SEVEN);
}

// Reg 0x10, bit[6]
void SparkFun_5P49V60::clockInControl(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(SHUTDOWN_REG, MASK_FOUR_MSB, control, POS_SIX);
}

// Reg 0x10, bit[3]
void SparkFun_5P49V60::doubleRefFreqControl(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(SHUTDOWN_REG, MASK_EIGHT, control, POS_THREE);
}

// Reg 0x10, bit[2]
void SparkFun_5P49V60::refModeControl(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(SHUTDOWN_REG, MASK_FOUR, control, POS_TWO);
}

// Reg 0x10, bit[1]
void SparkFun_5P49V60::sdInputPinControl(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(SHUTDOWN_REG, MASK_TWO, control, POS_ONE);
}

// Reg 0x10, bit[0]
void SparkFun_5P49V60::globalSdControl(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(SHUTDOWN_REG, MASK_ONE, control, POS_ZERO);
}

// Reg 0x68, bits[7:3]
// This function allows the given clock line to remain on when the SparkFun
// Clock Generator is put into shutdown mode.
void SparkFun_5P49V60::persEnableClock(uint8_t clock){

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
  else if (clock == 4){
    _bit_pos = POS_THREE;
    _mask = MASK_FOUR;
  }

  _writeRegister(CLK_OE_FUNC_REG, _mask, clock, _bit_pos);
}

// Reg 0x68, bit[2]
void SparkFun_5P49V60::clockZeroSlewRate(uint8_t rate)
{
  if (rate == FAST || rate == SLOW){
    _writeRegister(CLK_OE_FUNC_REG, MASK_FOUR, rate, POS_TWO);
  }
}

// Reg 0x68, bit[2]. Possible parameters 18 (1.8V), 25 (2.5V), or 33 (3.3V).
// Why not just have float as paramater, we're saving just a wee bit of space
// this way.
void SparkFun_5P49V60::clockZeroPwrSel(uint8_t voltage)
{
  if (voltage == 18){
    _writeRegister(CLK_OE_FUNC_REG, MASK_THREE, ONE_EIGHT_V, POS_ZERO);
  }
  else if (voltage == 25){
    _writeRegister(CLK_OE_FUNC_REG, MASK_THREE, TWO_FIVE_V, POS_ZERO);
  }
  else if (voltage == 33){
    _writeRegister(CLK_OE_FUNC_REG, MASK_THREE, THREE_THREE_V, POS_ZERO);
  }
  else
    return;
}

// This function adds the available internal capacitors to the given pin on the
// crystal. The two available pins can be taken directly from the datasheet, or
// schematic: XIN and XOUT.
void SparkFun_5P49V60::addCrystalLoadCap(uint8_t pin, float capVal){

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
float SparkFun_5P49V60::readCrystalCapVal(uint8_t pin){

  uint8_t _register;

  if (pin == XIN)
    _register = LOAD_CAP_REG_ONE;
  else if (pin == XOUT)
    _register = LOAD_CAP_REG_TWO;
  else return 0.0;

  uint8_t reg_val = _readRegister(_register);
  reg_val &= MASK_THREE;

  if (reg_val == 1){
    return _cap_arr[0];
  }
  else if (reg_val == 2){
    return _cap_arr[1];
  }
  else if (reg_val == 3){
    return _cap_arr[0] + _cap_arr[1];
  }
  else if (reg_val == 4){
    return _cap_arr[2];
  }
  else if (reg_val == 5){
    return _cap_arr[0] + _cap_arr[2];
  }
  else if (reg_val == 6){
    return _cap_arr[1] + _cap_arr[2];
  }
  else
    return 0.0;

}


// This function changes the primary source of the clock: either crystal
// or CLKIN. The product is set by default in hardware to use the crystal.
void SparkFun_5P49V60::changeSource(uint8_t source){

  if (source == CLOCK_SOURCE || source == XTAL_SOURCE) {}
  else return;

  _writeRegister(LOAD_CAP_REG_TWO, MASK_THREE, CLOCK_SOURCE, POS_ONE);

}

// This function returns the currently selected source of the clock: either crystal
// or CLKIN. The product is set by default in hardware to use the crystal.
uint8_t SparkFun_5P49V60::readSource(){

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
void SparkFun_5P49V60::selectRefDivider(uint8_t div_val){

  if (div_val < 2 || div_val > 127)
    return;

  if (div_val == 2)
    _writeRegister(REF_DIVIDER_REG, MASK_EIGHT_MSB, div_val, POS_SEVEN);
  else
    _writeRegister(REF_DIVIDER_REG, (~MASK_EIGHT_MSB), div_val, POS_ZERO);

}

// REG 0x15, bits[7:0] This selects which divider value is used for the clock
// output.
uint8_t SparkFun_5P49V60::readRefDivider(){

  uint8_t reg_val = _readRegister(REF_DIVIDER_REG);

  // Divider 2 disables other divider.
  if ((reg_val & (~MASK_EIGHT_MSB)) >> POS_SEVEN == 1)
    return 2;
  else
    return reg_val &= MASK_EIGHT_MSB;

}

// REG 0x16, bit[7] This function enables the function to  bypass the clock pre-divider,
// which is essentially a divide by 1.
void SparkFun_5P49V60::bypassRefDivider(uint8_t div){
  if( div == DISABLE || div == ENABLE)
    _writeRegister(DIVIDER_VCO_REG, MASK_EIGHT_MSB, div, POS_SEVEN);
}

// REG 0x16, bit[7] This function reads whether the clock pre-divider is being
// bypassed.
uint8_t SparkFun_5P49V60::readBypassDivider(){

  uint8_t reg_val = _readRegister(DIVIDER_VCO_REG);
  reg_val &= (~MASK_EIGHT_MSB);
  reg_val >>= POS_SEVEN;

  return reg_val;

}

// REG 0x11, bit[5] This function turns on the VCO test mode.
void SparkFun_5P49V60::vcoTestControl(uint8_t cont){
  if (cont == ENABLE || cont == DISABLE)
    _writeRegister(VCO_BAND_REG, MASK_TWO_MSB, cont, POS_FIVE);
}

// REG 0x11, bit[5] This function checks to see if the VCO test mode is running.
uint8_t SparkFun_5P49V60::readTestControl(){

  uint8_t reg_val = _readRegister(VCO_BAND_REG);
  reg_val &= (~MASK_TWO);
  reg_val >>= POS_FIVE;
  return reg_val;

}

// Reg 0x17 and 0x18, bits[7:0] and bits[7:4] respectively - largest value that
// can be set: 4,095. To determine this value you'll divide your desired output
// by the value of the clock source, 16MHz by default.
// e.g. 1600 MHz VCO/16MHz Clock = 100 DEC => 0x64 => 0b00001100100
void SparkFun_5P49V60::setPllFeedbackIntDiv(uint16_t divider_val){

  if (divider_val < 0 || divider_val > 4095)
    return;

  if (divider_val <= 15){
    // LSB in 0x18
    _writeRegister(FDB_INT_DIV_REG_TWO, MASK_FIFT_MSB, divider_val, POS_FOUR);
    _writeRegister(FDB_INT_DIV_REG_TWO, MASK_ALL, 0, POS_ZERO);
  }
  else {
    // MSB in 0x17, LSB in 0x18
    uint16_t lsb_div_val = divider_val & MASK_FIFT_MSB;
    _writeRegister(FDB_INT_DIV_REG_TWO, MASK_FIFT_MSB, lsb_div_val, POS_FOUR);
    uint16_t msb_div_val = (divider_val & MASK_ALL_12_BIT) >> POS_FOUR;
    Serial.println(msb_div_val, BIN);
    _writeRegister(FDB_INT_DIV_REG_ONE, MASK_ALL, msb_div_val, POS_ZERO);
  }

}

// REG 0x18, bits[3:2], Sigma Delta Modulator Setting: bypass, order one through
// three.
void SparkFun_5P49V60::setSigmaDeltaMod(uint8_t order){
  if (order >= 0 && order <= 3)
    _writeRegister(FDB_FRAC_DIV_REG_ONE, MASK_TEN, order, POS_TWO);
  else
    return;
}

// REG 0x19, 0x1A, and 0x1B, bits[8:0] for all three registers.
void SparkFun_5P49V60::setPllFeedBackFractDiv(uint32_t divider_val){

  uint32_t lsb_div_val; // Least significant BYTE
  uint32_t msb_div_val;
  uint32_t mmsb_div_val;

  if (divider_val < 0 || divider_val > 16,777,215)
    return;

  if (divider_val <= 255){
    _writeRegister(FDB_FRAC_DIV_REG_THR, MASK_ALL, divider_val, POS_ZERO);
    _writeRegister(FDB_FRAC_DIV_REG_TWO, MASK_ALL, 0, POS_ZERO);
    _writeRegister(FDB_FRAC_DIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 65535){
    lsb_div_val = divider_val & MASK_ALL_8_BIT;
    _writeRegister(FDB_FRAC_DIV_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);

    msb_div_val = divider_val & MASK_ALL_16_BIT;
    msb_div_val >>= POS_EIGHT;
    _writeRegister(FDB_FRAC_DIV_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);

    _writeRegister(FDB_FRAC_DIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {
    lsb_div_val = divider_val & MASK_ALL_8_BIT;
    _writeRegister(FDB_FRAC_DIV_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);

    msb_div_val = divider_val & MASK_ALL_16_BIT;
    msb_div_val >>= POS_EIGHT;
    _writeRegister(FDB_FRAC_DIV_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);

    mmsb_div_val = divider_val & MASK_ALL_24_BIT;
    mmsb_div_val >>= POS_SIXT;
    _writeRegister(FDB_FRAC_DIV_REG_ONE, MASK_ALL, mmsb_div_val, POS_ZERO);
  }
}

// REG 0x19, 0x1A, and 0x1B, bits[8:0] for all three registers.
uint32_t SparkFun_5P49V60::readPllFeedBackFractDiv(){

  uint32_t lsb_div_val;
  uint32_t msb_div_val;
  uint32_t mmsb_div_val;

  lsb_div_val  = _readRegister(FDB_FRAC_DIV_REG_THR);
  msb_div_val  = _readRegister(FDB_FRAC_DIV_REG_TWO);
  msb_div_val <<= POS_EIGHT;
  mmsb_div_val = _readRegister(FDB_FRAC_DIV_REG_ONE);
  mmsb_div_val <<= POS_SIXT;

  mmsb_div_val |= msb_div_val;
  mmsb_div_val |= lsb_div_val;

  return mmsb_div_val;

}

// REG 0x1C, bit[7] This forces the VCO band to manually calibrate.
void SparkFun_5P49V60::calibrateVco(){
  _writeRegister(VC_CONTROL_REG, MASK_EIGHT_MSB, DISABLE, POS_SEVEN);
  delay(1);
  _writeRegister(VC_CONTROL_REG, MASK_EIGHT_MSB, ENABLE, POS_SEVEN);
}

// REG 0x1E, bits[7:3]
void SparkFun_5P49V60::setPllFilterResOne(uint16_t res_val){

  uint8_t _bits;

  if (res_val == 1500){
    _bits = 0b11110;
    _writeRegister(RC_CONTR_REG_TWO, 0x07, res_val, POS_THREE);
  }
  else if (res_val == 46500){
    _bits = 0b0;
    _writeRegister(RC_CONTR_REG_TWO, 0x07, res_val, POS_THREE);
  }
  else return;

}

// REG 0x1E, bits[3:0]
void SparkFun_5P49V60::setPllFilterCapOne(uint8_t cap_val){

  uint8_t _bits;

  if (cap_val == 12){
    _bits = 0b000;
    _writeRegister(RC_CONTR_REG_TWO, MASK_SEVEN, _bits, POS_ZERO);
  }
  else if (cap_val == 16){
    _bits = 0b001;
    _writeRegister(RC_CONTR_REG_TWO, MASK_SEVEN, _bits, POS_ZERO);
  }
  else if (cap_val == 20){
    _bits = 0b010;
    _writeRegister(RC_CONTR_REG_TWO, MASK_SEVEN, _bits, POS_ZERO);
  }
  else if (cap_val == 24){
    _bits = 0b011;
    _writeRegister(RC_CONTR_REG_TWO, MASK_SEVEN, _bits, POS_ZERO);
  }
  else if (cap_val == 28){
    _bits = 0b100;
    _writeRegister(RC_CONTR_REG_TWO, MASK_SEVEN, _bits, POS_ZERO);
  }
  else return;

}

// REG 0x1F, bits[3:1]
void SparkFun_5P49V60::setPllFilterResTwo(uint16_t res_val){

  uint8_t _bits;

  if (res_val == 1000){
    _bits = 0b100;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else if (res_val == 1450){
    _bits = 0b111;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else if (res_val == 1600){
    _bits = 0b011;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else if (res_val == 2000){
    _bits = 0b001;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else if (res_val == 5300){
    _bits = 0b110;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else if (res_val == 7000){
    _bits = 0b101;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else if (res_val == 8000){
    _bits = 0b010;
    _writeRegister(RC_CONTR_REG_THR, MASK_FOURT, _bits, POS_ONE);
  }
  else return;

}

//REG 0x1F, bit[7]
void SparkFun_5P49V60::bypassPllFilterThree(bool bypass){

  if (bypass)
    _writeRegister(RC_CONTR_REG_THR, MASK_EIGHT_MSB, bypass, POS_SEVEN);
  else
    _writeRegister(RC_CONTR_REG_THR, MASK_EIGHT_MSB, bypass, POS_SEVEN);

}

// REG 0x1F, bits[6:4]
void SparkFun_5P49V60::setPllFilterCapTwo(float cap_val){

  uint8_t _bits;

  if (cap_val == 1.8){
    _bits = 0b001;
    _writeRegister(RC_CONTR_REG_THR, MASK_THR_MSB, _bits, POS_FOUR);
  }
  else if (cap_val == 3.6){
    _bits = 0b011;
    _writeRegister(RC_CONTR_REG_THR, MASK_THR_MSB, _bits, POS_FOUR);
  }
  else if (cap_val == 5.4){
    _bits = 0b111;
    _writeRegister(RC_CONTR_REG_THR, MASK_THR_MSB, _bits, POS_FOUR);
  }
  else return;
}

// REG 0x1D, bits[:] - SKIPPED
void SparkFun_5P49V60::setPllFilterChargePump(uint8_t pump_val){
}

// REG 0x1E, bits[7:3]
uint16_t SparkFun_5P49V60::readPllFilterResOne(){

  uint8_t res_val = _readRegister(RC_CONTR_REG_TWO);
  res_val &= MASK_SEVEN;
  res_val >>= POS_THREE;

  if (res_val == 0b11110) return 1500;
  else if (res_val == 0)  return 46500;
  else return UNKNOWN_ERROR;

}

// REG 0x1E, bits[2:0]
uint8_t SparkFun_5P49V60::readPllFilterCapOne(){

  uint8_t cap_val = _readRegister(RC_CONTR_REG_TWO);
  cap_val &= (~MASK_SEVEN);

  if (cap_val == 0b000)      return 12;
  else if (cap_val == 0b001) return 16;
  else if (cap_val == 0b010) return 20;
  else if (cap_val == 0b011) return 24;
  else if (cap_val == 0b100) return 28;
  else return UNKNOWN_ERROR;

}

// REG 0x1F, bits[3:1]
uint16_t SparkFun_5P49V60::readPllFilterResTwo(){

  uint8_t res_val = _readRegister(RC_CONTR_REG_THR);
  res_val &= (~MASK_FOURT);

  // Values in Ohms
  if (res_val == 0b100)       return 1000;
  else if (res_val == 0b111)  return 1450;
  else if (res_val == 0b011)  return 1600;
  else if (res_val == 0b001)  return 2000;
  else if (res_val == 0b110)  return 5300;
  else if (res_val == 0b101)  return 7000;
  else if (res_val == 0b010)  return 8000;
  else return UNKNOWN_ERROR;

}

// REG 0x1F, bits[6:4]
float SparkFun_5P49V60::readPllFilterCapTwo(){

  uint8_t cap_val = _readRegister(RC_CONTR_REG_THR);
  cap_val &= (~MASK_THR_MSB);
  cap_val >>= POS_FOUR;

  if (cap_val == 0b001)      return 1.8;
  else if (cap_val== 0b011)  return 3.6;
  else if (cap_val == 0b111) return 5.4;
  else return UNKNOWN_ERROR_F;
}
// REG 0x1F, bits[7]
void SparkFun_5P49V60::bypassThirdFilter(uint8_t control){

  if (control == ENABLE || control == DISABLE)
    _writeRegister(RC_CONTR_REG_THR, MASK_FOUR_MSB, control, POS_SEVEN);

}
// REG 0x21, bits[7]
void SparkFun_5P49V60::resetFodOne(){
  _writeRegister(DIV_ONE_CONTROL_REG, MASK_FOUR_MSB, DISABLE, POS_SEVEN);
  delay(5);
  _writeRegister(DIV_ONE_CONTROL_REG, MASK_FOUR_MSB, ENABLE, POS_SEVEN);
}

// REG 0x21, bits[3:0], 0b0000
void SparkFun_5P49V60::fodOutOneCont(uint8_t control){
  if (control == DISABLE || control == ENABLE)
    _writeRegister(DIV_ONE_CONTROL_REG, MASK_FIFT_MSB, control, POS_ZERO);
}

// REG 0x21, bits[3:0], 0b00x1
void SparkFun_5P49V60::fodPllOutFodOne(){
  _writeRegister(DIV_ONE_CONTROL_REG, MASK_THIRT, 0x01, POS_TWO);
}

// REG 0x21, bits[3:0], 0b1100
void SparkFun_5P49V60::fodDisOutOutOne(){
  refModeControl(DISABLE);
  _writeRegister(DIV_ONE_CONTROL_REG, MASK_TWO, 0xF0, POS_ZERO);
}

// REG 0x21, bits[3:0], 0b1111
void SparkFun_5P49V60::fodOutOutFodOne(){
  refModeControl(DISABLE);
  _writeRegister(DIV_ONE_CONTROL_REG, MASK_FIFT, 0x0F, POS_ZERO);
}

// REG 0x21, bit[1]
void SparkFun_5P49V60::integModeContOne(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(DIV_ONE_CONTROL_REG, MASK_TWO, control, POS_ONE);
}


// REG 0x22, 0x23, 0x24, 0x25 bits[7:0] in the first three registers and
// bits[7:2] in 0x25.
void SparkFun_5P49V60::setFodOneFractDiv(uint32_t divider_val){

  uint32_t llsb_div_val; //  Least least significant BYTE: 0x25
  uint32_t lsb_div_val; // Least significant BYTE: 0x24
  uint32_t msb_div_val; // 0x23
  uint32_t mmsb_div_val; // 0x22

  if (divider_val < 0 || divider_val > 536,870,911)
    return;

  if (divider_val <= 31){
    // 0x25
    _writeRegister(OUT_FDIV_REG_FOUR, MASK_ALL, divider_val, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_THR, MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_TWO, MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 8191){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;

    // 0x25, 0x24
    _writeRegister(OUT_FDIV_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_TWO, MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 2,097,151){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);

    // 0x25, 0x24, 0x23
    _writeRegister(OUT_FDIV_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);
    mmsb_div_val = (divider_val & 0x1FFFFFFF) >> (POS_FIVE + 16);

    // 0x25, 0x24, 0x23, 0x22
    _writeRegister(OUT_FDIV_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_REG_ONE, MASK_ALL, mmsb_div_val, POS_ZERO);
  }
}

//REG 0x2C, bit[0]
void SparkFun_5P49V60::auxControlOne(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(OUT_ISKEW_REG_TWO, MASK_ONE, control, POS_ZERO);
}

//REG 0x2D and 0x2E, bits[7:0] and bits[7:4] respectively. Maximum value that
// that can be set: 4,095.
void SparkFun_5P49V60::setIntDivOutOne(uint8_t divider_val ){

  if (divider_val < 0 || divider_val > 4095)
    return;

  if (divider_val <= 15){
    // LSB in 0x2E
    _writeRegister(OUT_IDIV_REG_TWO, MASK_FIFT_MSB, divider_val, POS_FOUR);
    _writeRegister(OUT_IDIV_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {
    // MSB in 0x2D, LSB in 0x2E
    uint16_t lsb_div_val = divider_val & MASK_FIFT_MSB;
    _writeRegister(OUT_IDIV_REG_TWO, MASK_FIFT_MSB, lsb_div_val, POS_FOUR);
    uint16_t msb_div_val = (divider_val & MASK_ALL_12_BIT) >> POS_FOUR;
    _writeRegister(OUT_IDIV_REG_ONE, MASK_ALL, msb_div_val, POS_ZERO);
  }

}

// REG 0x31, bits[7]
void SparkFun_5P49V60::resetFodTwo(){
  _writeRegister(DIV_TWO_CONTROL_REG, MASK_FOUR_MSB, DISABLE, POS_SEVEN);
  delay(5);
  _writeRegister(DIV_TWO_CONTROL_REG, MASK_FOUR_MSB, ENABLE, POS_SEVEN);
}

// REG 0x31, bits[3:0]
void SparkFun_5P49V60::fodOutTwoCont(uint8_t control){
  if (control == DISABLE || control == ENABLE)
    _writeRegister(DIV_TWO_CONTROL_REG, MASK_FIFT, control, POS_ZERO);
}

// REG 0x31, bits[3:0] 0b00x1
void SparkFun_5P49V60::fodPllOutFodTwo(){
  _writeRegister(DIV_TWO_CONTROL_REG, MASK_THIRT, 0x01, POS_ZERO);
}

// REG 0x31, bits[3:0] 0b1100
void SparkFun_5P49V60::fodOutOutTwo(){
  auxControlOne(ENABLE);
  _writeRegister(DIV_TWO_CONTROL_REG, MASK_FIFT, 0x03, POS_TWO);
}

// REG 0x31, bits[3:0] 0b1111
void SparkFun_5P49V60::fodOutOutFodTwo(){
  auxControlOne(ENABLE);
  _writeRegister(DIV_TWO_CONTROL_REG, MASK_FIFT, 0x0F, POS_ZERO);
}

// REG 0x31, bit[1]
void SparkFun_5P49V60::integModeContTwo(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(DIV_TWO_CONTROL_REG, MASK_TWO, control, POS_ONE);
}

// REG 0x32, 0x33, 0x34, 0x35 bits[7:0] in the first three registers and
// bits[7:2] in 0x35.
void SparkFun_5P49V60::setFodTwoFractDiv(uint32_t divider_val){

  uint32_t llsb_div_val; //  Least least significant BYTE: 0x35
  uint32_t lsb_div_val; // Least significant BYTE: 0x34
  uint32_t msb_div_val; // 0x33
  uint32_t mmsb_div_val; // 0x32

  if (divider_val < 0 || divider_val > 536,870,911)
    return;

  if (divider_val <= 31){
    // 0x35
    _writeRegister(OUT_FDIV_TWO_REG_FOUR, MASK_ALL, divider_val, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_THR , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_TWO , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 8191){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;

    // 0x35, 0x34
    _writeRegister(OUT_FDIV_TWO_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_TWO_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_TWO, MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 2,097,151){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);

    // 0x35, 0x34, 0x33
    _writeRegister(OUT_FDIV_TWO_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_TWO_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);
    mmsb_div_val = (divider_val & 0x1FFFFFFF) >> (POS_FIVE + 16);

    // 0x35, 0x34, 0x33, 0x32
    _writeRegister(OUT_FDIV_TWO_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_TWO_REG_THR, MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_TWO, MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_TWO_REG_ONE, MASK_ALL, mmsb_div_val, POS_ZERO);
  }
}

//REG 0x3C, bit[0]
void SparkFun_5P49V60::auxControlTwo(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(OUT_ISKEW_TWO_REG_TWO, MASK_ONE, control, POS_ZERO);
}

//REG 0x3D and 0x3E, bits[7:0] and bits[7:4] respectively. Maximum value that
// that can be set: 4,095.
void SparkFun_5P49V60::setIntDivOutTwo(uint8_t divider_val ){

  if (divider_val < 0 || divider_val > 4095)
    return;

  if (divider_val <= 15){
    // LSB in 0x3E
    _writeRegister(OUT_IDIV_TWO_REG_TWO, MASK_FIFT_MSB, divider_val, POS_FOUR);
    _writeRegister(OUT_IDIV_TWO_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {
    // MSB in 0x3D, LSB in 0x3E
    uint16_t lsb_div_val = divider_val & MASK_FIFT_MSB;
    _writeRegister(OUT_IDIV_TWO_REG_TWO, MASK_FIFT_MSB, lsb_div_val, POS_FOUR);
    uint16_t msb_div_val = (divider_val & MASK_ALL_12_BIT) >> POS_FOUR;
    _writeRegister(OUT_IDIV_TWO_REG_ONE, MASK_ALL, msb_div_val, POS_ZERO);
  }

}

// REG 0x41, bits[7]
void SparkFun_5P49V60::resetFodThree(){
  _writeRegister(DIV_THR_CONTROL_REG, MASK_FOUR_MSB, DISABLE, POS_SEVEN);
  delay(5);
  _writeRegister(DIV_THR_CONTROL_REG, MASK_FOUR_MSB, ENABLE, POS_SEVEN);
}

// REG 0x41, bits[3:0]
void SparkFun_5P49V60::fodOutThreeCont(uint8_t control){
  if (control == DISABLE || control == ENABLE)
    _writeRegister(DIV_THR_CONTROL_REG, MASK_FIFT_MSB, control, POS_ZERO);
}

// REG 0x41, bits[3:0] 0b00x1
void SparkFun_5P49V60::fodPllOutFodThree(){
  _writeRegister(DIV_THR_CONTROL_REG, MASK_THIRT, 0x01, POS_TWO);
}

// REG 0x41, bits[3:0] 0b1100
void SparkFun_5P49V60::fodOutOutThree(){
  auxControlTwo(ENABLE);
  _writeRegister(DIV_THR_CONTROL_REG, MASK_FIFT, 0x03, POS_TWO);
}

// REG 0x41, bits[3:0] 0b1111
void SparkFun_5P49V60::fodOutOutFodThree(){
  auxControlTwo(ENABLE);
  _writeRegister(DIV_THR_CONTROL_REG, MASK_FIFT, 0x0F, POS_ZERO);
}

// REG 0x41, bit[1]
void SparkFun_5P49V60::integModeContThree(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(DIV_THR_CONTROL_REG, MASK_TWO, control, POS_ONE);
}

// REG 0x42, 0x43, 0x44, 0x45 bits[7:0] in the first three registers and
// bits[7:2] in 0x45.
void SparkFun_5P49V60::setFodThrFractDiv(uint32_t divider_val){

  uint32_t llsb_div_val; //  Least least significant BYTE: 0x45
  uint32_t lsb_div_val; // Least significant BYTE: 0x44
  uint32_t msb_div_val; // 0x43
  uint32_t mmsb_div_val; // 0x42

  if (divider_val < 0 || divider_val > 536,870,911)
    return;

  if (divider_val <= 31){
    // 0x45
    _writeRegister(OUT_FDIV_THR_REG_FOUR, MASK_ALL, divider_val, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_THR , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_TWO , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 8191){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;

    // 0x45, 0x44
    _writeRegister(OUT_FDIV_THR_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_THR_REG_THR , MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_TWO , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 2,097,151){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);

    // 0x45, 0x44, 0x43
    _writeRegister(OUT_FDIV_THR_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_THR_REG_THR , MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_TWO , MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else {

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);
    mmsb_div_val = (divider_val & 0x1FFFFFFF) >> (POS_FIVE + 16);

    // 0x45, 0x44, 0x43, 0x42
    _writeRegister(OUT_FDIV_THR_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_THR_REG_THR , MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_TWO , MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_THR_REG_ONE , MASK_ALL, mmsb_div_val, POS_ZERO);
  }
}

//REG 0x4C, bit[0]
void SparkFun_5P49V60::auxControlThree(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(OUT_ISKEW_THR_REG_TWO, MASK_ONE, control, POS_ZERO);
}

//REG 0x4D and 0x4E, bits[7:0] and bits[7:4] respectively. Maximum value that
// that can be set: 4,095.
void SparkFun_5P49V60::setIntDivOutThree(uint8_t divider_val ){

  if (divider_val < 0 || divider_val > 4095)
    return;

  if (divider_val <= 15){
    // LSB in 0x4E
    _writeRegister(OUT_IDIV_THR_REG_TWO, MASK_FIFT_MSB, divider_val, POS_FOUR);
    _writeRegister(OUT_IDIV_THR_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {
    // MSB in 0x4D, LSB in 0x4E
    uint16_t lsb_div_val = divider_val & MASK_FIFT_MSB;
    _writeRegister(OUT_IDIV_THR_REG_TWO, MASK_FIFT_MSB, lsb_div_val, POS_FOUR);
    uint16_t msb_div_val = (divider_val & MASK_ALL_12_BIT) >> POS_FOUR;
    _writeRegister(OUT_IDIV_THR_REG_ONE, MASK_ALL, msb_div_val, POS_ZERO);
  }

}
// REG 0x51, bits[7]
void SparkFun_5P49V60::resetFodFour(){
  _writeRegister(DIV_FOUR_CONTROL_REG, MASK_FOUR_MSB, DISABLE, POS_SEVEN);
  delay(5);
  _writeRegister(DIV_FOUR_CONTROL_REG, MASK_FOUR_MSB, ENABLE, POS_SEVEN);
}


// REG 0x51, bits[3:0]
void SparkFun_5P49V60::fodOutFourCont(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(DIV_FOUR_CONTROL_REG, MASK_FIFT_MSB, control, POS_ZERO);
}


// REG 0x51, bits[3:0] 0b00x1
void SparkFun_5P49V60::fodPllOutFodFour(){
  _writeRegister(DIV_FOUR_CONTROL_REG, MASK_THIRT, 0x01, POS_TWO);
}

// REG 0x51, bits[3:0] 0b1100
void SparkFun_5P49V60::fodOutOutFour(){
  auxControlOne(ENABLE);
  auxControlTwo(ENABLE);
  auxControlThree(ENABLE);
  _writeRegister(DIV_FOUR_CONTROL_REG, MASK_FIFT, 0x03, POS_TWO);
}

// REG 0x51, bits[3:0] 0b1111
void SparkFun_5P49V60::fodOutOutFodFour(){
  auxControlThree(ENABLE);
  _writeRegister(DIV_FOUR_CONTROL_REG, MASK_FIFT, 0x0F, POS_ZERO);
}

// REG 0x51, bit[1]
void SparkFun_5P49V60::integModeContFour(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(DIV_FOUR_CONTROL_REG, MASK_TWO, control, POS_ONE);
}

// REG 0x52, 0x53, 0x54, 0x55 bits[7:0] in the first three registers and
// bits[7:2] in 0x55.
void SparkFun_5P49V60::setFodFourFractDiv(uint32_t divider_val){

  uint32_t llsb_div_val; //  Least least significant BYTE: 0x55
  uint32_t lsb_div_val; // Least significant BYTE: 0x54
  uint32_t msb_div_val; // 0x53
  uint32_t mmsb_div_val; // 0x52

  if (divider_val < 0 || divider_val > 536,870,911)
    return;

  if (divider_val <= 31){
    // 0x55
    _writeRegister(OUT_FDIV_FOUR_REG_FOUR, MASK_ALL, divider_val, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_THR , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_TWO , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 8191){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;

    // 0x55, 0x54
    _writeRegister(OUT_FDIV_FOUR_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_FOUR_REG_THR , MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_TWO , MASK_ALL, 0, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else if (divider_val <= 2,097,151){

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);

    // 0x55, 0x54, 0x53
    _writeRegister(OUT_FDIV_FOUR_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_FOUR_REG_THR , MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_TWO , MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_ONE , MASK_ALL, 0, POS_ZERO);
  }
  else {

    llsb_div_val = divider_val & 0x1F;
    llsb_div_val = (divider_val & 0x1FFF) >> POS_FIVE;
    msb_div_val = (divider_val & 0x1FFFFF) >> (POS_FIVE + 8);
    mmsb_div_val = (divider_val & 0x1FFFFFFF) >> (POS_FIVE + 16);

    // 0x55, 0x54, 0x53, 0x52
    _writeRegister(OUT_FDIV_FOUR_REG_FOUR, MASK_ALL, llsb_div_val, POS_TWO);
    _writeRegister(OUT_FDIV_FOUR_REG_THR , MASK_ALL, lsb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_TWO , MASK_ALL, msb_div_val, POS_ZERO);
    _writeRegister(OUT_FDIV_FOUR_REG_ONE , MASK_ALL, mmsb_div_val, POS_ZERO);
  }
}

//REG 0x5C, bit[0]
void SparkFun_5P49V60::auxControlFour(uint8_t control){
  if (control == ENABLE || control == DISABLE)
    _writeRegister(OUT_ISKEW_FOUR_REG_TWO, MASK_ONE, control, POS_ZERO);
}

//REG 0x5D and 0x5E, bits[7:0] and bits[7:4] respectively. Maximum value that
// that can be set: 4,095.
void SparkFun_5P49V60::setIntDivOutFour(uint8_t divider_val ){

  if (divider_val < 0 || divider_val > 4095)
    return;

  if (divider_val <= 15){
    // LSB in 0x5E
    _writeRegister(OUT_IDIV_FOUR_REG_TWO, MASK_FIFT_MSB, divider_val, POS_FOUR);
    _writeRegister(OUT_IDIV_FOUR_REG_ONE, MASK_ALL, 0, POS_ZERO);
  }
  else {
    // MSB in 0x5D, LSB in 0x5E
    uint16_t lsb_div_val = divider_val & MASK_FIFT_MSB;
    _writeRegister(OUT_IDIV_FOUR_REG_TWO, MASK_FIFT_MSB, lsb_div_val, POS_FOUR);
    uint16_t msb_div_val = (divider_val & MASK_ALL_12_BIT) >> POS_FOUR;
    _writeRegister(OUT_IDIV_FOUR_REG_ONE, MASK_ALL, msb_div_val, POS_ZERO);
  }

}
//uint16_t SparkFun_5P49V60::calculateFracDivider(uint16_t clock_hertz){
  //if (clock_hertz > k
//}

// This generic function handles I2C write commands for modifying individual
// bits in an eight bit register. Paramaters include the register's address, a mask
// for bits that are ignored, the bits to write, and the bits' starting
// position.
void SparkFun_5P49V60::_writeRegister(uint8_t _wReg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition) {

  uint8_t _i2cWrite;
  _i2cWrite = _readRegister(_wReg);
  _i2cWrite &= (_mask);
  _i2cWrite |= (_bits << _startPosition);

  _i2cPort->beginTransmission(_address);
  _i2cPort->write(_wReg);
  _i2cPort->write(_i2cWrite);
  _i2cPort->endTransmission();

}

// This generic function reads an eight bit register. It takes the register's
// address as its' parameter.
uint8_t SparkFun_5P49V60::_readRegister(uint8_t _reg) {

  _i2cPort->beginTransmission(_address);
  _i2cPort->write(_reg); // Moves pointer to register.
  _i2cPort->endTransmission(false);
  _i2cPort->requestFrom(_address, static_cast<uint8_t>(1));
  uint8_t _reg_value = _i2cPort->read();
  return(_reg_value);

}

