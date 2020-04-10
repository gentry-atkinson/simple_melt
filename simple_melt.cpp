/************************************************
*Author: Gentry Atkinson
*Organization: Bobcat Robotics
*Date: 10 Apr 2020
*Description: this is a driver for Uncle Traveling Matt, the melty brain with a
*  busted accelerometer. Turn the pot to tune the spin rate. Good enough right?
************************************************/

//Expected rpm at the wheels is 800-2000
//Wheel diamter = 32mm
//Wheel circumfrence = 100.5mm
//Wheel to wheel diameter = 95mm
//Drive circle = 298.5mm
//Wheel to body ration = 0.337
//Expected rate of spin = 270 - 674rpm
//Expected ms per rotation = 222 - 89

#define SLOW_SPIN 222
#define FAST_SPIN 89

#include <pololu/orangutan>

//Connected pins on Baby Orangutan. Change to fit your setup
#define headlight_pin IO_D7
#define throttle_pin IO_B1
#define steering_pin IO_B0
#define ch3_pin IO_B2

#define sda_pin IO_C4
#define scl_pin IO_C5
#define adc_read_pin IO_A7

#define adc_channel 7

//For pulse_in
#define LOW_PULSE   1
#define HIGH_PULSE  2
#define ANY_PULSE   3

int main (){
  //Set Up
  set_motors(0, 0);
  set_digital_output(headlight_pin, LOW);
  set_analog_mode(MODE_10_BIT); //trimpot output will be 0-1023

  int pot = 0;
  //Perma-loop
  while(1){
    pot = read_trimpot();
  }
  return 0;
}
