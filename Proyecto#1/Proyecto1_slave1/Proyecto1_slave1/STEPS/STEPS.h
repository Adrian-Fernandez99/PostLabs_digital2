/*
 * STEPS.h
 *
 * Created: 2/18/2026 2:45:32 AM
 *  Author: adria
 */ 


#ifndef STEPS_H_
#define STEPS_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void step_init(void);
void mov_step(uint8_t direccion);


#endif /* STEPS_H_ */