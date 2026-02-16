#include "I2C.h"

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//****************
// Funcion para inicializar I2C Maestro
//****************
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); //Pines de i2c como entradas
	
	switch(Prescaler){
		case 1:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		break;
		case 4:
			TWSR &= ~(1<<TWPS1);
			TWSR |= (1<<TWPS0);
		break;
		case 16:
			TWSR &= ~ (1<<TWPS0);
			TWSR |= (1<<TWPS1);
		break;
		case 64:
			TWSR |= (1<<TWPS1)|(1<<TWPS0);
		break;
		default:
			TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
			Prescaler = 1;
		break;
	}
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler); /*must be > 10 for stable operation */
	TWCR |= (1<<TWEN);
}
//****************
// Funcion de inicio de la comunicacion I2C
//****************
uint8_t I2C_Master_Start()
{
	TWCR = (1<< TWINT) | (1<<TWSTA) | (1<<TWEN); 
	while(!(TWCR & (1<< TWINT))); //Espera a que termine la flag TWINT
	return ((TWSR & 0XF8) == 0x08);
	
	/*estado = TWSR & 0XF8; //Verificar estado
	if ((estado != 0x08) || (estado != 0x10)) {
		return 1;
	}else{
		return estado;
	}*/
}
//****************
//Funcion de reinicio de la comunicacion I2C
//****************
uint8_t I2C_Master_RepeatedStart(){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	return ((TWSR & 0xF8) == 0x10);
}
//****************
//Funcion de parada de la comunicacion I2C
//****************
void I2C_Master_Stop(){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO); //Inicia el envio secuencia parada STOP
	while (TWCR & (1 << TWSTO)); //Esperamos a que el bit se limpie
}
//****************
//Funcion de transmision de datos del maestro al esclavo
//esta funcion devolvera un 0 si el esclavo a recibido
//el dato
//****************
uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato; //Cargar el dato
	TWCR = (1<<TWEN)|(1<<TWINT);
	
	while(!(TWCR&(1<<TWINT))); //Espera al flag TWINT
	estado = TWSR & 0xF8; //verificar estado
	//verificar si se transmitio una SLA + W con AFK, SLA + R con ACK, o un Dato con ACK
	if(estado == 0x18 || estado == 0x28)
	{
		return 1;
	}
	else
	{
		return estado;
	}
}

//********
//Funcion de recepcion de datos enviados´por el esclavo al maestro
//********
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if(ack){
		TWCR |= (1<<TWINT) | (1 << TWEN) | (1 << TWEA); //Lectura con ACK
	}
	else{
		TWCR = (1<<TWINT) | (1<<TWEN);
	}
	
	while(!(TWCR&(1<<TWINT))); //ESPERA AL FLAG TWINT
	
	estado = TWSR & 0xF8; //verificar estado
	//Verificar dato leido con ACK o sin ACK
	if(ack && estado != 0x50) return 0;
	if(!ack && estado != 0x58) return 0;
	
	*buffer = TWDR;
	return 1;	
}
//********
//Funcion para inicializar I2C Esclavo
//********
void I2C_Slave_Init(uint8_t adress){
	DDRC &= ~((1<<DDC4)|(1<<DDC5)); //PINES DE I2C COMO ENTRADAS
	TWAR = adress <<1; //se asigna la direccion que tendra
	//TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}