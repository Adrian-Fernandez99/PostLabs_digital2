// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
// 
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
float temperatura = 0;
float humedad = 0;
int distancia = 0;

#define IO_LOOP_DELAY 10000  
// 10 segundos (seguro para cuenta free)
unsigned long lastUpdate = 0;

#define RX2 16
#define TX2 17

char buffer[15];
int bufferIndex = 0;
bool receivingValue = false;
char currentType = '\0';

// set up the 'counter' feed
//AdafruitIO_Feed *counter = io.feed("counter");

AdafruitIO_Feed *feedTemperatura = io.feed("temperatura");
AdafruitIO_Feed *feedHumedad = io.feed("humedad");
AdafruitIO_Feed *feedDistancia = io.feed("distancia");
AdafruitIO_Feed *feedMotor = io.feed("motordcventilador");
AdafruitIO_Feed *feedStepper = io.feed("motorstepper");


void setup() {

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);  // UART con Nano

  while(!Serial);

  Serial.print("Connecting to Adafruit IO");

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  io.connect();

  // Handler para el motor
  feedMotor->onMessage(handleMotor);
  feedStepper->onMessage(handleStepper);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  digitalWrite(2, HIGH);
  Serial.println();
  Serial.println(io.statusText());
}

/************* LOOP *************/

void loop() {

  io.run();

  // Leer datos del Nano
  read_Data();

  // Enviar a Adafruit cada 10 segundos
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {

    feedTemperatura->save(temperatura);
    feedHumedad->save(humedad);
    feedDistancia->save(distancia);

    Serial.println("Datos enviados a Adafruit:");
    Serial.print("Temp: ");
    Serial.println(temperatura);
    Serial.print("Humedad: ");
    Serial.println(humedad);
    Serial.print("Distancia: ");
    Serial.println(distancia);

    lastUpdate = millis();
  }
}

/************* HANDLER MOTOR *************/
void handleStepper(AdafruitIO_Data *data) {

  char valor = data->value()[0];

  if(valor == 'S') {
    Serial2.write('S');   // Stepper ON
    Serial.println("Stepper ON");
  }
  else if(valor == 's') {
    Serial2.write('s');   // Stepper OFF
    Serial.println("Stepper OFF");
  }
}

void handleMotor(AdafruitIO_Data *data) {

  char valor = data->value()[0];

  if(valor == 'M') {
    Serial2.write('M');   // Manual ON
    Serial.println("Motor ON (Manual)");
  }
  else if(valor == 'm') {
    Serial2.write('m');   // Manual OFF
    Serial.println("Motor OFF (Manual)");
  }
  else if(valor == 'A') {
    Serial2.write('A');   // Volver a automático
    Serial.println("Motor AUTO");
  }
}



/************* LECTURA UART *************/

void read_Data() {

  while (Serial2.available()) {

    char receivedChar = Serial2.read();

    if (receivedChar == 't' || receivedChar == 'h' || receivedChar == 'd') {
      receivingValue = true;
      bufferIndex = 0;
      currentType = receivedChar;
    }

    else if (receivingValue) {

      if (receivedChar == ' ') {

        buffer[bufferIndex] = '\0';

        if (currentType == 't') {
          temperatura = atof(buffer);
        }
        else if (currentType == 'h') {
          humedad = atof(buffer);
        }
        else if (currentType == 'd') {
          distancia = atoi(buffer);
        }

        receivingValue = false;
      }

      else if (bufferIndex < sizeof(buffer) - 1) {
        buffer[bufferIndex++] = receivedChar;
      }
    }
  }
}