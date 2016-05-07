#ifndef CONSOLE_DEBUG
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include "usb_keyboard.h"

#ifndef KEYBOARD_EXAMPLE
#define KEYBOARD_EXAMPLE

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

// Teensy 2.0: LED is active high
#define LED_CONFIG  (DDRD |= (1<<6))
#define LED_ON      (PORTD |= (1<<6))
#define LED_OFF     (PORTD &= ~(1<<6))

#endif