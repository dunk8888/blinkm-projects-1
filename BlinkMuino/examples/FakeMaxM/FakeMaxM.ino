 /**
  * FakeMaxM -- super simple example of how to do an I2C slave 
  *             and use analogWrite() to create an incredibly
  *             naive emulation of basic BlinkM MaxM functionality
  *
  * Some code take from TinyWireM/examples
  *
  * NOTE: Pullup resistors should be used on SDA & SCL lines
  * 
  * Thanks to BroHogan for TinyWireS: http://playground.arduino.cc/Code/USIi2c
  * Credit and thanks to Don Blake for his usiTwiSlave code. 
  * More on TinyWireS usage - see TinyWireS.h
  *
  * 2013, Tod E. Kurt, http://todbot.com/blog/ http://thingm.com/
  *
  */

#include "TinyWireS.h"                  // wrapper class for I2C slave routines

// BlinkM MaxM pins (see below for where this came from)
const int redPin = 8;   // PB2 OC0A
const int grnPin = 7;   // PA7 OC0B
const int bluPin = 5;   // PA5 OC1B
const int sdaPin = 6;   // PA6 
const int sclPin = 9;   // PA4
const int in0Pin = A0;  // PA0
const int in1Pin = A1;  // PA1
const int in2Pin = A2;  // PA2
const int in3Pin = A3;  // PA3

// From: blinkm/variants/tiny14/pins_arduino.h  
// (originally from: http://hlt.media.mit.edu/?p=1695 )
// ATMEL ATTINY84 / ARDUINO
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//             (D 10)  PB0  2|    |13  AREF (D  0)
//             (D  9)  PB1  3|    |12  PA1  (D  1) 
//                     PB3  4|    |11  PA2  (D  2) 
//  PWM  INT0  (D  8)  PB2  5|    |10  PA3  (D  3) 
//  PWM        (D  7)  PA7  6|    |9   PA4  (D  4) 
//  PWM        (D  6)  PA6  7|    |8   PA5  (D  5)        PWM
//                           +----+



#define I2C_SLAVE_ADDR  0x09     // default BlinkM addr

byte cmd;
byte r,g,b;

void setup()
{
  TinyWireS.begin(I2C_SLAVE_ADDR); 

  pinMode( redPin, OUTPUT);   
  pinMode( grnPin, OUTPUT);  
  pinMode( bluPin, OUTPUT);  
  blink( redPin,1);
  blink( grnPin,1);
  blink( bluPin,1);
}

void loop()
{

  if( TinyWireS.available() ) { 

    cmd = TinyWireS.receive();    // first byte is command
    if( cmd == 'c' || cmd == 'n' ) {  // "fade to color" & "set color now" cmds
      r = TinyWireS.receive();
      g = TinyWireS.receive();
      b = TinyWireS.receive();
      analogWrite( redPin, r );
      analogWrite( grnPin, g );
      analogWrite( bluPin, b );
    }
    else if( cmd == 'C' ) { // "random rgb color" cmd
      byte dr = TinyWireS.receive();
      byte dg = TinyWireS.receive();
      byte db = TinyWireS.receive();
      r = r + (rand() % dr ) - (dr/2);  // random around previsous color
      g = g + (rand() % dg ) - (dg/2);
      b = b + (rand() % db ) - (db/2);
      analogWrite( redPin, r );
      analogWrite( grnPin, g );
      analogWrite( bluPin, b );
    }
    // else if( cmd == 'h' ) { // "fade to HSB color" cmd
    //}
    else {
      // unknown cmd
    }
  }
}


//
void blink(byte pin, byte times)
{ 
  for (byte i=0; i< times; i++){
    digitalWrite(pin,HIGH);
    delay (200);
    digitalWrite(pin,LOW);
    delay (100);
  }
}



