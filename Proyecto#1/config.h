/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "britt_barillas"
#define IO_KEY       "aio_vUwn41r1ViBdzixwGeEqZ0XwHplG"

#define WIFI_SSID "Brittany Barillas"
#define WIFI_PASS "12345678"

// comment out the following lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
