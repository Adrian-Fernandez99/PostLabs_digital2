/*
 * Display.c
 *
 * Created: 1/21/2026 11:17:08 PM
 *  Author: adria
 */ 

#include <avr/io.h>

#include "Display.h"

const uint8_t TABLA7SEG[16] =
{
	0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x7F, 0x4E, 0x7E, 0x4F, 0x47
};
//	0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    A,    B,    C,    D,    E,    F

uint8_t display(uint8_t dis_no, uint8_t digito){
	dis_no = TABLA7SEG[digito];
	return dis_no;
}