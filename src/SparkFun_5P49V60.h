#ifndef _SPARKFUN_CLOCK_5P49V60_
#define _SPARKFUN_CLOCK_5P49V60_

#include <Wire.h>
#include <Arduino.h>

// 7 bit unshifted addresses:
#define DEF_ADDR 0x6A
#define ALT_ADDR 0x68

#define  ALT            0x01
#define  DEF            0x00
#define  SLOW           0x00
#define  FAST           0x01
#define  DISABLE        0x00
#define  ENABLE         0x01
#define  XIN            0x01
#define  XOUT           0x02
#define  ERROR          0xFF
#define  CLOCK_SOURCE   0x00
#define  XTAL_SOURCE    0x01
#define  ONE_EIGHT_V    0x00
#define  TWO_FIVE_V     0x02
#define  THREE_THREE_V  0x03

#define  LVPECL_MODE    0x00
#define  CMOS_MODE      0x01
#define  HCSL33_MODE    0x02
#define  LVDS_MODE      0x03
#define  CMOS2_MODE     0x04
#define  CMOSD_MODE     0x05
#define  HCSL25_MODE    0x06

#define UNKNOWN_ERROR   0xFF
#define UNKNOWN_ERROR_F 255.0

//                            1 , 2   , 4   , 8    , 16   , 32
static float _cap_arr[6] = {.43 , .43 , .86 , 1.73 , 3.46 , 6.92};

enum REGISTER_INDEX {

  OTP_CONTROL_REG            = 0x00,
  SHUTDOWN_REG               = 0x10,
  VCO_BAND_REG               = 0x11,
  LOAD_CAP_REG_ONE           = 0x12,
  LOAD_CAP_REG_TWO           = 0x13,
  REF_DIVIDER_REG            = 0x15,
  DIVIDER_VCO_REG            = 0x16,

  FDB_INT_DIV_REG_ONE        = 0x17,
  FDB_INT_DIV_REG_TWO        = 0x18,

  FDB_FRAC_DIV_REG_ONE       = 0x19,
  FDB_FRAC_DIV_REG_TWO       = 0x1A,
  FDB_FRAC_DIV_REG_THR       = 0x1B,

  VC_CONTROL_REG             = 0x1C,

  RC_CONTR_REG_ONE           = 0x1D,
  RC_CONTR_REG_TWO           = 0x1E,
  RC_CONTR_REG_THR           = 0x1F,

  DIV_ONE_CONTROL_REG        = 0x21,

  OUT_FDIV_REG_ONE           = 0x22,
  OUT_FDIV_REG_TWO           = 0x23,
  OUT_FDIV_REG_THR           = 0x24,
  OUT_FDIV_REG_FOUR          = 0x25,

  OUT_STEP_SPRD_REG_ONE      = 0x26,
  OUT_STEP_SPRD_REG_TWO      = 0x27,
  OUT_STEP_SPRD_REG_THR      = 0x28,

  OUT_SPRD_RATE_REG_ONE      = 0x29,
  OUT_SPRD_RATE_REG_TWO      = 0x2A,

  OUT_ISKEW_REG_ONE          = 0x2B,
  OUT_ISKEW_REG_TWO          = 0x2C,

  OUT_IDIV_REG_ONE           = 0x2D,
  OUT_IDIV_REG_TWO           = 0x2E,

  OUT_FSKEW_REG              = 0x2F,

  DIV_TWO_CONTROL_REG        = 0x31,

  OUT_FDIV_TWO_REG_ONE       = 0x32,
  OUT_FDIV_TWO_REG_TWO       = 0x33,
  OUT_FDIV_TWO_REG_THR       = 0x34,
  OUT_FDIV_TWO_REG_FOUR      = 0x35,

  OUT_STEP_SPRD_TWO_REG_ONE  = 0x36,
  OUT_STEP_SPRD_TWO_REG_TWO  = 0x37,
  OUT_STEP_SPRD_TWO_REG_THR  = 0x38,

  OUT_SPRD_RATE_TWO_REG_ONE  = 0x39,
  OUT_SPRD_RATE_TWO_REG_TWO  = 0x3A,

  OUT_ISKEW_TWO_REG_ONE      = 0x3B,
  OUT_ISKEW_TWO_REG_TWO      = 0x3C,

  OUT_IDIV_TWO_REG_ONE       = 0x3D,
  OUT_IDIV_TWO_REG_TWO       = 0x3E,

  OUT_FSKEW_TWO_REG          = 0x3F,

  DIV_THR_CONTROL_REG        = 0x41,

  OUT_FDIV_THR_REG_ONE      = 0x42,
  OUT_FDIV_THR_REG_TWO      = 0x43,
  OUT_FDIV_THR_REG_THR      = 0x44,
  OUT_FDIV_THR_REG_FOUR     = 0x45,

  OUT_STEP_SPRD_FOUR_REG_ONE = 0x46,
  OUT_STEP_SPRD_FOUR_REG_TWO = 0x47,
  OUT_STEP_SPRD_FOUR_REG_THR = 0x48,

  OUT_SPRD_RATE_FOUR_REG_ONE = 0x49,
  OUT_SPRD_RATE_FOUR_REG_TWO = 0x4A,

  OUT_ISKEW_THR_REG_ONE      = 0x4B,
  OUT_ISKEW_THR_REG_TWO      = 0x4C,

  OUT_IDIV_THR_REG_ONE       = 0x4D,
  OUT_IDIV_THR_REG_TWO       = 0x4E,

  OUT_FSKEW_THR_REG          = 0x4F,

  DIV_FOUR_CONTROL_REG        = 0x51,

  OUT_FDIV_FOUR_REG_ONE      = 0x52,
  OUT_FDIV_FOUR_REG_TWO      = 0x53,
  OUT_FDIV_FOUR_REG_THR      = 0x54,
  OUT_FDIV_FOUR_REG_FOUR     = 0x55,

  OUT_ISKEW_FOUR_REG_ONE     = 0x5B,
  OUT_ISKEW_FOUR_REG_TWO     = 0x5C,

  OUT_IDIV_FOUR_REG_ONE      = 0x5D,
  OUT_IDIV_FOUR_REG_TWO      = 0x5E,

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
  CLK_OS_FUNC_REG            = 0x69,

  GLOBAL_RESET_REG           = 0x76

};

enum MASK_INDEX {
  // The names reference the number position that will be changed
  // MASK_FOUR = 1111 1011, or MASK_FOUR_MSB = 1011 111, or MASK_TWO = 1111 1101
  MASK_ONE        = 0xFE, 
  MASK_TWO        = 0xFD,
  MASK_THREE      = 0xFC,
  MASK_FOUR       = 0xFB,
  MASK_FIVE       = 0xFA,
  MASK_SIX        = 0xF9,
  MASK_SEVEN      = 0xF8,
  MASK_EIGHT      = 0xF7,
  MASK_NINE       = 0xF6,
  MASK_TEN        = 0xF5,
  MASK_ELEVEN     = 0xF4,
  MASK_TWELVE     = 0xF3,
  MASK_THIRT      = 0xF2,
  MASK_FOURT      = 0xF1,
  MASK_FIFT       = 0xF0,

  MASK_FIFT_MSB   = 0x0F,
  MASK_THIRT_MSB  = 0x1F,
  MASK_EIGHT_MSB  = 0x7F,
  MASK_FOUR_MSB   = 0xBF,
  MASK_THR_MSB    = 0x8F,
  MASK_TWO_MSB    = 0xDF,
  MASK_ONE_MSB    = 0xEF,

  MASK_ALL        = 0x00,
  MASK_ALL_8_BIT  = 0x0000FF,
  MASK_ALL_12_BIT = 0xFF0,
  MASK_ALL_16_BIT = 0x00FFFF,
  MASK_ALL_24_BIT = 0xFF0000,

  MASK_ENDS       = 0x81
};

enum BIT_POS_INDEX {

  POS_ZERO = 0x00,
  POS_ONE,
  POS_TWO,
  POS_THREE,
  POS_FOUR,
  POS_FIVE,
  POS_SIX,
  POS_SEVEN,
  POS_EIGHT,
  POS_FIFT = 0xF,
  POS_SIXT

};

class SparkFun_5P49V60
{
  public:

    // Public Variables

    //Function declarations
    SparkFun_5P49V60(uint8_t address = DEF_ADDR); // I2C Constructor

    bool begin(TwoWire &wirePort = Wire); 

    void changeI2CAddress(uint8_t);

    uint8_t readI2CAddress();

    uint8_t readBurnedBit();

    void xtalControl(uint8_t control);

    void clockInControl(uint8_t control);

    void doubleRefFreqControl(uint8_t control);

    void refModeControl(uint8_t control);

    void sdInputPinControl(uint8_t control);

    void globalSdControl(uint8_t control);

    void persEnableClock(uint8_t clock);

    void clockZeroSlewRate(uint8_t rate);

    void clockZeroPwrSel(uint8_t voltage);

    void addCrystalLoadCap(uint8_t, float);

    float readCrystalCapVal(uint8_t);

    void changeSource(uint8_t);

    uint8_t readSource();

    void selectRefDivider(uint8_t);

    uint8_t readRefDivider();

    void bypassRefDivider(uint8_t);

    uint8_t readBypassDivider();

    void vcoTestControl(uint8_t);

    uint8_t readTestControl();
    
    void setPllFeedbackIntDiv(uint16_t);

    uint16_t readPllFeedBackIntDiv();

    void setSigmaDeltaMod(uint8_t);
    
    void setPllFeedBackFractDiv(uint32_t);

    uint32_t readPllFeedBackFractDiv();

    void calibrateVco();

    void setPllFilterResOne(uint16_t);
    
    void setPllFilterCapOne(uint8_t);

    void setPllFilterResTwo(uint16_t);

    void bypassPllFilterThree(bool);

    void setPllFilterCapTwo(float);

    void setPllFilterChargePump(uint8_t);

    uint16_t readPllFilterResOne();

    uint8_t readPllFilterCapOne();

    uint16_t readPllFilterResTwo();

    float readPllFilterCapTwo();

    void bypassThirdFilter(uint8_t control);

    void resetFodOne();

    void muxPllToFodOne(uint8_t control = ENABLE);

    void muxRefClockToOutOne();

    void muxRefClockToFodOne();

    void integerModeOne(uint8_t);

    void auxControlOne(uint8_t);

    void setFodOneFractDiv(uint32_t);

    void setIntDivOutOne(uint8_t);

    uint16_t readIntDivOutOne();

    void resetFodTwo();

    void muxPllToFodTwo(uint8_t control = ENABLE);

    void muxOutOneToOutTwo();

    void muxFodOneToFodTwo();

    void integerModeTwo(uint8_t);

    void setFodTwoFractDiv(uint32_t);

    void setIntDivSkewTwo(uint8_t);

    uint16_t readIntDivSkewTwo();
    
    void auxControlTwo(uint8_t);

    void setIntDivOutTwo(uint8_t);

    uint16_t readIntDivOutTwo();

    void resetFodThree();

    void muxPllToFodThree(uint8_t control = ENABLE);

    void muxOutTwoToOutThree();

    void muxOutTwoToFodThree();

    void integerModeThree(uint8_t control);

    void setFodThrFractDiv(uint32_t);

    void auxControlThree(uint8_t control);

    void setIntDivOutThree(uint8_t);

    uint16_t readIntDivOutThree();

    void resetFodFour();

    void muxPllToFodFour(uint8_t control = ENABLE);

    void muxOutThreeToOutFour();

    void muxOutThreeToFodFour();

    void integerModeFour(uint8_t control);

    void setFodFourFractDiv(uint32_t);

    void auxControlFour(uint8_t control);

    void setIntDivOutFour(uint8_t);

    uint16_t readIntDivOutFour();

    uint8_t _readRegister(uint8_t);

    void clockOneConfigMode(uint8_t);

    void clockOneSlew(uint8_t rate);

    void clockOneControl(uint8_t);

    void resetClockOne();

    void clockTwoControl(uint8_t);

    void resetClockTwo();

    void clockTwoConfigMode(uint8_t);

    void clockThrControl(uint8_t);

    void clockThrConfigMode(uint8_t);

    void clockFourControl(uint8_t);

    void clockFourConfigMode(uint8_t);

    void globalReset();

  private:

    // Private Variables
    uint8_t _address;

    // This generic function handles I2C write commands for modifying individual
    // bits in an eight bit register. Paramaters include the register's address, a mask
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void _writeRegister(uint8_t, uint8_t, uint8_t, uint8_t);

    // This generic function reads an eight bit register. It takes the register's
    // address as its' parameter.

    TwoWire *_i2cPort;
};
#endif
