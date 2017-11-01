#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 // #include "pins_arduino.h"
 // #include "WConstants.h"
#endif

#include <OneWire.h>
#include "PCA9633.h"

PCA9633::PCA9633() {}

static void _i2c_wordwrite(uint8_t address, uint8_t cmd, uint16_t theWord) {
  //  Send output register address
  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.write(highByte(theWord));  //  high byte
  Wire.write(lowByte(theWord));  //  send low byte of word data
  Wire.endTransmission();
}

static void _i2c_write(uint8_t address, uint8_t cmd, uint8_t data) {
  //  Send output register address
  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.write(data);  //  send byte data
  Wire.endTransmission();
}

static uint16_t _i2c_wordread(uint8_t address, uint8_t cmd) {
  uint16_t result;
  uint8_t xlo, xhi;

  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.endTransmission();

  uint8_t readbytes = Wire.requestFrom(address, 2); // request two bytes
  xhi = Wire.read();
  xlo = Wire.read();

  result = xhi << 8; // hi byte
  result = result | xlo; // add in the low byte

  return result;
}

static uint8_t _i2c_read(uint8_t address, uint8_t cmd) {
  uint8_t result = 0;

  Wire.beginTransmission(address);
  Wire.write(cmd); // control register
  Wire.endTransmission();

  uint8_t readbytes = Wire.requestFrom(address, (size_t)1, (bool)true); // request cnt bytes

  result  = Wire.read();

  return result;
}

void PCA9633::chipinit(void) { // setup chip with desired operating parameters
  uint8_t m1 = 0x00; // set sleep = 0, turn on oscillator, disable allcall and subaddrs
  uint8_t m2 = ((INVRT) | (OUTDRV)); // output inverted, totem pole drivers enabled
  uint8_t ldout = 0xFF; // all outputs under individual and group control

  _i2c_write(_pcaAddr, MODE1, m1);
  _i2c_write(_pcaAddr, MODE2, m2);
  _i2c_write(_pcaAddr, LEDOUT, ldout);
}

void PCA9633::begin(uint8_t addr) { // lets get started
	_pcaAddr = addr;
	// Wire.begin(); // don't need this maybe?
	chipinit(); // setup chip
}

void PCA9633::setrgbw(uint8_t p0, uint8_t p1, uint8_t p2, uint8_t p3) {
  _i2c_write(_pcaAddr, PWM0, p0);
  _i2c_write(_pcaAddr, PWM1, p1);
  _i2c_write(_pcaAddr, PWM2, p2);
  _i2c_write(_pcaAddr, PWM3, p3);
}

void PCA9633::setpwm(uint8_t pwmaddr, uint8_t pwmval) {
  _i2c_write(_pcaAddr, (pwmaddr + 2), pwmval);
}

void PCA9633::setgrouppwm(uint8_t pwm) {
  _i2c_write(_pcaAddr, GRPPWM, pwm);
}
