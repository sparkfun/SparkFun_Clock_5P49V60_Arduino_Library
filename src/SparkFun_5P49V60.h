#ifndef _SPARKFUN_CLOCK_GEN_5P49V60_
#define _SPARKFUN_CLOCK_GEN_5P49V60_

#include <Wire.h>
#include <Arduino.h>

// 7 bit unshifted addresses: 
#define DEF_ADDR 0xD4 
#define ALT_ADDR 0xD0

#define ALT  0x01
#define DEF  0x00
#define LOW  0x00
#define HIGH 0x01

enum REGISTER_INDEX {
  
  OTP_CONTROL_REG            = 0x00,
  SHUTDOWN_REG               = 0x10,
  VCO_BAND_REG               = 0x11, 
  LOAD_CAP_REG               = 0x12,
  REF_DIVIDER_REG            = 0x15,
  DIVIDER_VCO_REG            = 0x16, 

  FDB_INT_DIV_REG_ONE        = 0x17, 
  FDB_INT_DIV_REG_TWO        = 0x18, 

  FDB_FRAC_DIV_REG_ONE       = 0x19, 
  FDB_FRAC_DIV_REG_TWO       = 0x1A, 
  FDB_FRAC_DIV_REG_THR       = 0x1B, 

  RC_CONTR_REG_ONE           = 0x1B, 
  RC_CONTR_REG_TWO           = 0x1C, 

  OUT_IDIV_REG_ONE           = 0x2D, 
  OUT_IDIV_REG_TWO           = 0x2E, 

  OUT_IDIV_TWO_REG_ONE       = 0x3D, 
  OUT_IDIV_TWO_REG_TWO       = 0x3E, 

  OUT_IDIV_THR_REG_ONE       = 0x4D, 
  OUT_IDIV_THR_REG_TWO       = 0x4E, 

  OUT_IDIV_FOUR_REG_ONE      = 0x5D, 
  OUT_IDIV_FOUR_REG_TWO      = 0x5E, 

  OUT_FDIV_REG_ONE           = 0x22, 
  OUT_FDIV_REG_TWO           = 0x23, 
  OUT_FDIV_REG_THR           = 0x24, 
  OUT_FDIV_REG_FOUR          = 0x25, 

  OUT_STEP_SPRD_REG_ONE      = 0x26, 
  OUT_STEP_SPRD_REG_TWO      = 0x27, 
  OUT_STEP_SPRD_REG_THR      = 0x28, 

  OUT_SPRD_RATE_REG_ONE      = 0x29, 
  OUT_SPRD_RATE_REG_TWO      = 0x2A, 

  OUT_FDIV_TWO_REG_ONE       = 0x32, 
  OUT_FDIV_TWO_REG_TWO       = 0x33, 
  OUT_FDIV_TWO_REG_THR       = 0x34, 
  OUT_FDIV_TWO_REG_FOUR      = 0x35, 

  OUT_STEP_SPRD_TWO_REG_ONE  = 0x36, 
  OUT_STEP_SPRD_TWO_REG_TWO  = 0x37, 
  OUT_STEP_SPRD_TWO_REG_THR  = 0x38, 

  OUT_SPRD_RATE_TWO_REG_ONE  = 0x39, 
  OUT_SPRD_RATE_TWO_REG_TWO  = 0x3A, 

  OUT_FDIV_THR_REG_ONE       = 0x42, 
  OUT_FDIV_THR_REG_TWO       = 0x43, 
  OUT_FDIV_THR_REG_THR       = 0x44, 
  OUT_FDIV_THR_REG_FOUR      = 0x45, 

  OUT_STEP_SPRD_THR_REG_ONE  = 0x46, 
  OUT_STEP_SPRD_THR_REG_TWO  = 0x47, 
  OUT_STEP_SPRD_THR_REG_THR  = 0x48, 

  OUT_SPRD_RATE_THR_REG_ONE  = 0x49, 
  OUT_SPRD_RATE_THR_REG_TWO  = 0x4A, 

  OUT_FDIV_FOUR_REG_ONE      = 0x42, 
  OUT_FDIV_FOUR_REG_TWO      = 0x43, 
  OUT_FDIV_FOUR_REG_THR      = 0x44, 
  OUT_FDIV_FOUR_REG_FOUR     = 0x45, 

  OUT_STEP_SPRD_FOUR_REG_ONE = 0x46, 
  OUT_STEP_SPRD_FOUR_REG_TWO = 0x47, 
  OUT_STEP_SPRD_FOUR_REG_THR = 0x48, 

  OUT_SPRD_RATE_FOUR_REG_ONE = 0x49, 
  OUT_SPRD_RATE_FOUR_REG_TWO = 0x4A, 
  
  OUT_ISKEW_REG_ONE          = 0x2B, 
  OUT_ISKEW_REG_TWO          = 0x2C, 

  OUT_FSKEW_REG              = 0x2F, 

  OUT_ISKEW_TWO_REG_ONE      = 0x3B, 
  OUT_ISKEW_TWO_REG_TWO      = 0x3C, 

  OUT_FSKEW_TWO_REG          = 0x3F, 

  OUT_ISKEW_THR_REG_ONE      = 0x4B, 
  OUT_ISKEW_THR_REG_TWO      = 0x4C, 

  OUT_FSKEW_THR_REG          = 0x4F, 

  OUT_ISKEW_FOUR_REG_ONE     = 0x5B, 
  OUT_ISKEW_FOUR_REG_TWO     = 0x5C, 

  OUT_FSKEW_FOUR_REG         = 0x5F, 
  
  CLK_ONE_OUT_CNFIG_REG_ONE  = 0x60,
  CLK_ONE_OUT_CNFIG_REG_TWO  = 0x61,

  CLK_TWO_OUT_CNFIG_REG_ONE  = 0x62,
  CLK_TWO_OUT_CNFIG_REG_TWO  = 0x63,

  CLK_THR_OUT_CNFIG_REG_ONE  = 0x64,
  CLK_THR_OUT_CNFIG_REG_TWO  = 0x65,

  CLK_FOUR_OUT_CNFIG_REG_ONE = 0x66,
  CLK_FOUR_OUT_CNFIG_REG_TWO = 0x67,
  
  CLK_OE_FUNC_REG            = 0x68,
  CLK_OS_FUNC_REG            = 0x69

};

enum MASK_INDEX {
  
  OTP_ADDR_MASK  = 0xFE,
  GLOB_SHUT_MASK = 0xFE,

  MASK_ONE       = 0xF1,
  MASK_TWO       = 0xF2,
  MASK_THREE     = 0xF3,
  MASK_FOUR      = 0xF4,
  MASK_FIVE      = 0xF5,
  MASK_SIX       = 0xF6,
  MASK_SEVEN     = 0xF7,
  MASK_EIGHT     = 0xF8,
  MASK_NINE      = 0xF9,
  MASK_TEN       = 0xFA,
  MASK_ELEVEN    = 0xFB,
  MASK_TWELVE    = 0xFC,
  MASK_THIRT     = 0xFD,
  MASK_FOURT     = 0xFE,
  MASK_FIFT      = 0xFF

};
  
enum BIT_POS_INDEX {
  
  POS_ZERO = 0x00, 
  POS_ONE, 
  POS_TWO, 
  POS_THREE, 
  POS_FOUR, 
  POS_FIVE, 
  POS_SIX, 
  POS_SEVEN 

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
