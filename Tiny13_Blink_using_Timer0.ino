// original code, https://github.com/MCUdude/MicroCore/
// install this broad, Arduino IDE, example code, blink_using_timer0.ino, it is original for Tiny13
// port to Tiny45
// port to ATmega168P

/************************************************************* 
 This sketch makes the pin LED pin PB2 (digital pin 2) toggle every
 second (internal oscillator running at 9.6 MHz). It uses Timer0
 or Timer0B, and divide the clock frequncy by 1024.
 The divided frequencies period is multiplied with the
 number of counts Timer0/Timer0B can take before it overflows.
 The number is then multiplied by 37, and gives approximately
 1 second.  

 9.6MHz / 1024 = 9370 Hz        We divide the 9.6 MHz clock by 1024
 1/9370 = 0.0001067s            Period time
 256 * 0.0001067 = 0.027315    
 0.027315 * 37 = 1.01065 ≈ 1s   
 X = 1.01065 / 0.027315 where X is approximately 37
**************************************************************/

// Xiaolaba testing,
// uses microcore 2.1.0 / 2.2.0 library, tested
// AttinyCore tiny45, MicroCore Tiny13 tested, upload via Arduino/usbtiny is ok
// PicoCore, Tiny13 only, upload via Arduino/usbtiny is NG
// Burn bootloader via Arduino IDE first, FUSES will be programme correctly
// then upload the skecth via Arduino IDE, F_CPU will be proper. 


#if defined (__AVR_ATtiny13__) || (__AVR_ATtiny13A__) || (__AVR_ATtiny13V__)
#define OSC 9600000UL // tiny13 internal RC OSC 9.6MHz
#define DDRport DDRB
#define LEDport PORTB
#define LED PB1 // Attiny13, OC0B & PB1
#define ISR_vector TIM0_OVF_vect
#endif

#if defined (__AVR_ATtiny25__) || (__AVR_ATtiny45__) || (__AVR_ATtiny85__)
#define OSC 8000000UL // tiny25/45/85 internal RC OSC 8MHz
#define TIMSK0 TIMSK // TIMSK0 for tiny13 only, TIMKS is same register for tiny25/45/85
#define DDRport DDRB
#define LEDport PORTB
#define LED PB1 // Attiny45, OC0B & PB1
#define ISR_vector TIM0_OVF_vect
#endif

#if defined (__AVR_ATmega168P__) || (__AVR_ATmega168V__) || (__AVR_ATmega328P__)
#define OSC 16000000UL // tiny25/45/85 internal RC OSC 8MHz
//#define DDRport DDRB // Attmega168/328, Uno / Nano, D13
//#define LEDport PORTB
//#define LED PB5 // Attmega168, Uno D13, onbroad LED
#define DDRport DDRD // Attmega168/328, Uno / Na
#define LEDport PORTD
#define LED PD5 // Attmega168, PD5/OC0B, Uno, D5
#define ISR_vector TIMER0_OVF_vect  //diffenrnt ISR vector name from Tiny13
#endif

#define count_1000ms OSC/1024/256 // count for timer overflow of 1 second
#define count_500ms OSC/1024/256/2 // count for timer overflow of 0.5 second


#include <avr/io.h>

// Variable to store the time count (uint8_t = byte)
uint8_t timeCount = 0;

int main (void)
{
  //DDRB |= _BV(PB2); //Set PB2 as output, ignore the rest
  DDRport |= _BV(LED); //Set LED pin as output, ignore the rest

  TCCR0B = _BV(CS02) | _BV(CS00); // clock frequency / 1024 
  OCR0B = 0x00;  // Output compare
  TCNT0 = 0; // Set counter 0 to zero
  TIMSK0 = _BV(TOIE0); // Enable overflow interrupt
  sei(); //Enable global interrupts
  
  while(1); // Infinite loop    
}



//ISR(TIM0_OVF_vect) //Attiny 13/25/45/85, Timer 0 overflow vector - this runs every time timer0 overflows
//ISR(TIMER0_OVF_vect) //Atmega8/88/168/328, Timer 0 overflow vector - this runs every time timer0 overflows
ISR(ISR_vector) //Timer 0 overflow vector - this runs every time timer0 overflows
{
  timeCount++;
  //if(timeCount == 37) //Timer overflown for the 37th time
  if(timeCount == count_1000ms) //Timer overflown for the 37th time
  //if(timeCount == count_500ms) //Timer overflown for the 37th time
  {
    //PORTB ^= _BV(PB2); //toggle PB2
    LEDport ^= _BV(LED); //toggle LED pin
    timeCount = 0;
  }
}
