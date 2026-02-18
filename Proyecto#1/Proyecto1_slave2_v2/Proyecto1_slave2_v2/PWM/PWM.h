/*
 * PWM.h
 *
 * Created: 2/17/2026 11:00:56 PM
 *  Author: adria
 */ 


#ifndef PWM_H_
#define PWM_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

void PWM_init();
uint16_t map_servo(uint16_t ADC_var);

#endif /* PWM_H_ */