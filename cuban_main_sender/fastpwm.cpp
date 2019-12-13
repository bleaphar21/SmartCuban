///*
// * fastpwm.cpp  - Fast PWM implementation for approximating 8kHz audio
// * Author: ndemarinis
// */
//
//
//#include "fastpwm.h"
//
//// Enable fast PWM mode on Timer2
//// We do this by manipulating the configuration registers for this
//// timer block.  More information on the operation of Timer 2
//// can be found in the Atmega328p datasheet, starting on p. 141
//// NOTE:  This function uses the Arduino bitSet() and bitClear()
//// macros to manipulate register bits, which is one of many ways
//// to set hardware registers.
//void fastpwm_init(void)
//{
//  pinMode(PIN_FASTPWM, OUTPUT);
//
//	// Configure Timer2 clock source to use internal 32kHz oscillator
//	// This should be the default already.  (Sec 18.11.8, p. 158)
////	bitClear(ASSR, EXCLK);
////	bitClear(ASSR, AS2);
//
//	// Clear existing Timer2 configuration
//	TCCR2A = 0;
//	TCCR2B = 0;
//
//	// Enable Fast PWM mode (set WGM = 0b011) (Table 18-8, p. 155)
//	// Note that we only need to set the 1 bits, since the register
//	// was cleared above.
//	bitSet(TCCR2A, WGM21);
//	bitSet(TCCR2A, WGM20);
//
//	// Enable non-inverting PWM output on OC2A
//	// This connects the PWM output to IC pin PB3,
//	// which is pin 11 on the Arduino Uno headers
//	// (set COM2A = 0b10) (Table 18-3, p. 153)
//	bitSet(TCCR2A, COM2A1);
//
//	// Set timer clock prescaler = 1  (set CS = 0b001) (Table 18-9, p. 156)
//	bitSet(TCCR2B, CS20);
//
//	// The period of the PWM waveform is controlled by register OCR2A,
//	// which contains the number of timer ticks for which the output
//	// should be high.
//	// Initially, disable PWM output by clearing OCR2A
//	OCR2A = 0;
//}
//
//
//void fastpwm_start(void)
//{
//	// Set timer clock prescaler = 1  (set CS = 0b001) (Table 18-9, p. 156)
//	bitClear(TCCR2B, CS22);
//	bitClear(TCCR2B, CS21);
//	bitSet(TCCR2B,   CS20);
//}
//
//void fastpwm_stop(void)
//{
//	// Stop timer by disabling clock source (set CS = 0b000) (Table 18-9, p. 156)
//	// Set timer clock prescaler = 1  (set CS = 0b001) (Table 18-9, p. 156)
//	bitClear(TCCR2B, CS22);
//	bitClear(TCCR2B, CS21);
//	bitClear(TCCR2B, CS20);
//}
//
//void fastpwm_play_sample(uint8_t data)
//{
//	OCR2A = data;
//}
