///*
// * fastpwm.h  - Fast PWM implementation for approximating 8kHz audio
// * Author: ndemarinis
// */
//
//#ifndef FASTPWM_H_
//#define FASTPWM_H_
//
//#include <Arduino.h>
//
//#define PIN_FASTPWM 11
//
//// Enable fast PWM mode on Timer2
//// Configures output on OC2A, which is pin PB3 on the
//// Atmega328p, which is pin 11 on the Arduino Uno
//void fastpwm_init(void);
//
//// Start PWM generator
//// This is useful to re-start playing samples after
//// the PWM generator has been disabled with fastpwm_stop
//void fastpwm_start(void);
//
//// Stop PWM generator
//// Useful when finished playing a sound
//void fastpwm_stop(void);
//
//
//// Play a single sample on fast PWM pin
//// While playing sound, this function must be called at
//// every sample period interval
//void fastpwm_play_sample(uint8_t data);
//
//
//#endif /* FASTPWM_H_ */
