/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/
  ----> http://www.adafruit.com/products/

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "mbed.h"
#include "Adafruit_LEDBackpack.h"

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif

static const uint8_t numbertable[] = {
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
};

void Adafruit_LEDBackpack::setBrightness(uint8_t b) {
  if (b > 15) b = 15;
  uint8_t c = 0xE0 | b;
  char foo[1];
  foo[0] = c;
  _i2c->write(i2c_addr, foo, 1); 
}

void Adafruit_LEDBackpack::blinkRate(uint8_t b) {
  if (b > 3) b = 0; // turn off if not sure
  uint8_t c = HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1);
  char foo[1];
  foo[0] = c;
  _i2c->write(i2c_addr, foo, 1);
}

Adafruit_LEDBackpack::Adafruit_LEDBackpack(I2C *i2c): _i2c(i2c) {
}

void Adafruit_LEDBackpack::begin(uint8_t _addr = 0x70) {
  i2c_addr = _addr << 1;
 
  char foo[1];
  foo[0] = 0x21;
 
  _i2c->write(i2c_addr, foo, 1);  // turn on oscillator
 
  blinkRate(HT16K33_BLINK_OFF);
  
  setBrightness(15); // max brightness
}

void Adafruit_LEDBackpack::writeDisplay(void) {
  char foo[17];
  foo[0] = 0x00;
  int j = 0;
  for (uint8_t i=1; i<=16; i+=2) {
    int x = displaybuffer[j] & 0xFF;
    foo[i] = x;
    int x2 = displaybuffer[j] >> 8;
    foo[i+1] = x2;
    j++;
  }
  _i2c->write(i2c_addr, foo, 17);
}

void Adafruit_LEDBackpack::clear(void) {
  for (uint8_t i=0; i<8; i++) {
    displaybuffer[i] = 0;
  }
}

/******************************* 24 BARGRAPH OBJECT */

Adafruit_24bargraph::Adafruit_24bargraph(I2C *i2c): Adafruit_LEDBackpack(i2c) {

}

void Adafruit_24bargraph::setBar(uint8_t bar, uint8_t color) {
  uint16_t a, c;
 
  if (bar < 12)
    c = bar / 4;
  else 
    c = (bar - 12) / 4;

  a = bar % 4;
  if (bar >= 12)
    a += 4;
    
  //Serial.print("Ano = "); Serial.print(a); Serial.print(" Cath = "); Serial.println(c);
  if (color == LED_RED) {
    displaybuffer[c] |= _BV(a) ;
  } else if (color == LED_YELLOW) {
    displaybuffer[c] |= _BV(a) | _BV(a+8);
  } else if (color == LED_OFF) {
    displaybuffer[c] &= ~_BV(a) & ~_BV(a+8);
  } else if (color == LED_GREEN) {
    displaybuffer[c] |= _BV(a+8) ;
  } 
}

