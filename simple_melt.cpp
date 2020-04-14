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

void read_pins(unsigned int*, unsigned int*, unsigned int*);
void set_neutral_pulse(unsigned int*);

int main (){
  //Vars live here
  unsigned int pot = 0;
  //unsigned int hold = 0;
  //unsigned int timer=0;
  unsigned int throttle=0, steering=0, ch3=0;
  unsigned int motor1Out, motor2Out;
  unsigned int neutralPulse;

  //Set Up
  set_motors(0, 0);
  red_led(0);
  set_digital_output(headlight_pin, HIGH);
  delay_ms(500);
  set_digital_output(headlight_pin, LOW);
  set_analog_mode(MODE_10_BIT); //trimpot output will be 0-1023
  set_neutral_pulse(&neutralPulse);

  //Perma-loop
  while(1){
    pot =  analog_read_average(TRIMPOT, 5);
    //hold = FAST_SPIN + (SLOW_SPIN-FAST_SPIN)*((1023-pot)/1023);
    //timer = (timer + 1)%hold;
    read_pins(&throttle, &steering, &ch3);
    if ((throttle - neutralPulse) > 1000){
      //spin
      red_led(1);
      set_motors(128, 128);
    }
    else{
      red_led(0);
      set_motors(0, 0);
    }
  }
  return 0;
}

void set_neutral_pulse(unsigned int* neutralPulse){
  static struct PulseInputStruct pulseInfo;
  long int counter=0;

  do{
    get_pulse_info(0, &pulseInfo);
    counter++;
    if(counter%1000 == 0){
      if(counter%2000 == 0){
        set_digital_output(headlight_pin, LOW);
        counter = 0;
      }
      else{
        set_digital_output(headlight_pin, HIGH);
      }
    }
    delay_ms(1);
  }while((get_ticks()-pulseInfo.lastPCTime) > 10000);

  *neutralPulse=(pulse_to_microseconds(pulseInfo.lastHighPulse));
  set_digital_output(headlight_pin, LOW);

}

void read_pins(unsigned int* throttle, unsigned int* steering, unsigned int* ch3){

  red_led(1);

  static struct PulseInputStruct pulseInfo;
  get_pulse_info(0, &pulseInfo);

  //freeze if signal drops
  if((get_ticks()-pulseInfo.lastPCTime) > 10000){
    *throttle = 0;
    *steering = 0;
    return;
  }

  *throttle = (pulse_to_microseconds(pulseInfo.lastHighPulse));

  get_pulse_info(1, &pulseInfo);
  *steering = (pulse_to_microseconds(pulseInfo.lastHighPulse));

  if((get_ticks()-pulseInfo.lastPCTime) > 10000){
    *throttle = 0;
    *steering = 0;
    return;
  }

  get_pulse_info(2, &pulseInfo);
  *ch3 = (pulse_to_microseconds(pulseInfo.lastHighPulse));


  red_led(0);
  return;
}
