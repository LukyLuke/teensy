/* License:
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * A simple test for change a string into USB-Keyboard Scancodes:
 * http://www.win.tue.nl/~aeb/linux/kbd/scancodes-14.html
 * 
 * Official USB-Guide: Chapter 10
 * http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
 * 
 * For the Keyboard-Layouts see:
 * US: http://www.goodtyping.com/teclatUS.htm
 * CH: http://home.datacomm.ch/t.bigler/sskbdsg.htm
 * Others: https://www.terena.org/activities/multiling/ml-mua/test/kbd-all.html
 * 
 * @author Lukas Zurschmiede <l.zurschmiede@ranta.ch>
 * @version 0.1
 * @package teensy_keyboard
 * @license GPL-v3
 */

#include "config.h"
#include "keyboard_payload.h"

#ifdef CONSOLE_DEBUG
#include <stdio.h>
#endif

/**
 * The string to send by the Keyboard.
 * @see parse_command_lines(char *str) for documentation
 */
char *str = "K: ALT F2\n\
W 500\n\
S konsole\n\
E\n\
W 500\n\
S echo '#!/bin/sh'>hacked.sh\n\
E\n\
S echo 'while :; do echo \"Hacked or so :))\";done'>>hacked.sh\n\
E\n\
S sh hacked.sh &\n\
E\0";

/**
 * Keys to send as Keystrokes
 */
int press_key = 0, press_modifier = 0;

/**
 * Send a single character, defined by an array of chars, as Keystrokes
 * 
 * @param *str Pointer to the CharArray to send
 */
void parse_char(char *str);

/**
 * Parse the line and check for the following commands:
 * -> K MODIFIER KEY
 * -> S STRING TO SEND
 * -> W 100
 * -> X
 * -> E
 * -> T
 * -> U
 * -> D
 * -> L
 * -> R
 * 
 * The "K" is used to simulate a KeyStroke with a modifier key
 *         Modifiers are A(al), C(trl), W(in), S(hift), N(one)
 * The "S" is used to write a string
 * The "W" is used to wait the given amount of milliseconds before the next line is processed
 * The "X" sends an ESC keystroke
 * The "E" sends a RETURN keystroke
 * The "T" sends a TAB keystroke
 * The "U" sends a UP keystroke
 * The "D" sends a DOWN keystroke
 * The "L" sends a LEF keystroke
 * The "R" sends a RIGHT keystroke
 * 
 * All spaces are removed between the command sequence and the first character
 * For better readability you can use an optional doublepoint after the command char
 * 
 * @param *str Pointer to the CharArray to send
 */
void parse_command_lines(char *str);


/**
 * The main method
 */
int main(void) {
	int i;
	
#ifndef CONSOLE_DEBUG
	// Set for 16 MHz clock, configure the LED and turn it off
	CPU_PRESCALE(0);
	LED_CONFIG;
	LED_OFF;

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init(); 
	while (!usb_configured());

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	// we just blink a little around in this time...
	for (i = 0; i < 5; i++) {
		LED_ON;
		_delay_ms(100);
		LED_OFF;
		_delay_ms(100);
	}
#endif
	
	// Parse the above defined command
	parse_command_lines(str);
	return 0;
}

/**
 * Implementatio of parse_line (char *str)
 */
void parse_command_lines(char *str) {
	char *send = str, chr;
	int cmd = *(send++), len = 1, sent = 0, timeout = 0;
	
	// Skip the ":" after the command
	if (*send == ':') {
		send++;
		len++;
	}
	
	// Skip all whitespaces if there are some
	if (*send == ' ') {
		while (*send == ' ') {
			send++;
			len++;
		}
	}
	
#ifdef CONSOLE_DEBUG
	int debug_char;
#endif
	
	// Check for the command
	switch (cmd) {
		// Single key with a modifier
		case 'K':
		case 'k':
#ifdef CONSOLE_DEBUG
			debug_char = *send;
			printf("> Keystroke with modifier: %s", &debug_char);
#endif
			while (1) {
				chr = *(send++);
				if (chr == '\0') break;
				if (chr == '\r') continue;
				if (chr == '\n') break;
				if (chr == ' ' && !sent) {
					// Check for F1-F12 or parse a normal key
					if ( ((*send == 'f') || (*send == 'F'))
						&& ((*(send + 1) > 48) && (*(send + 1) < 58)) // Check ASCII-Code gt 0 and le 9
					) {
						send++;
						
						// Check for F10-12
						if (((*(send + 1) > 47) && (*(send + 1) < 51))) {
							send++;
							press_key = 19 + *send; // USB-Scankey for F10-F12 is 67-69
						} else {
							press_key = 9 + *send; // USB-Scankey for F1-F9 is 58-66
						}
						
					// Space
					} else if ( ((*send == 's') || (*send == 'S'))
						&& ((*(send + 1) == 'p') || (*(send + 1) == 'P'))
					) {
						send++;
						press_key = KEY_SPACE;
						
					// Any other key
					} else {
						parse_char(send++);
					}
					
					switch (str[len]) {
						case 'W':
						case 'w':
							press_modifier = KEY_GUI;
							break;
						case 'S':
						case 's':
							press_modifier = KEY_SHIFT;
							break;
						case 'C':
						case 'c':
							press_modifier = KEY_CTRL;
							break;
						case 'A':
						case 'a':
							press_modifier = KEY_ALT;
							break;
						default:
							press_modifier = KEY_NONE;
					}
#ifdef CONSOLE_DEBUG
					debug_char = *(send - 1);
					printf(" and key: %s", &debug_char);
					if ((*(send - 1) == 'f') || (*(send - 1) == 'F')) {
						debug_char = *send;
						printf("%s", &debug_char);
					}
					printf("\n");
#else
					usb_keyboard_press(press_key, press_modifier);
#endif
					sent = 1;
				}
			}
			break;
			
		// Send a String and press ENTER at the end
		case 'S':
		case 's':
			while (1) {
				chr = *(send++);
				if (chr == '\0') break;
				if (chr == '\r') continue;
				if (chr == '\n') break;
				parse_char(&chr);
#ifdef CONSOLE_DEBUG
				cmd = 0; // If I not do this, printf shows 'S' after the parsed character... Don't know yet why
				printf("  Char: %s, ASCII: %d, USB: %d, Modifier: %d\n", &chr, chr, press_key, press_modifier);
#else
				usb_keyboard_press(press_key, press_modifier);
#endif
			}
			break;
			
		// Wait for the given amount of milliseconds
		case 'W':
		case 'w':
			timeout = 0;
			while (1) {
				chr = *(send++);
				if (chr == '\0') break;
				if (chr == '\r') continue;
				if (chr == '\n') break;
				if (chr > 47 && chr < 58) {
					timeout *= 10;
					timeout += (chr - 48);
				}
			}
#ifdef CONSOLE_DEBUG
			printf("> Waiting for %d Milliseconds\n", timeout);
#else
			// The _delay_ms function needs a compile-time constant, so we count up in 10ms steps until we reach timeout
			while (timeout > 10) {
				timeout -= 10;
				_delay_ms(10);
			}
#endif
			break;
			
		case 'X':
		case 'x':
			// Read until the end of the line and press the ESCAPE key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending ESCAPE\n");
#else
			usb_keyboard_press(KEY_ESC, KEY_NONE);
#endif
			break;
			
		case 'E':
		case 'e':
			// Read until the end of the line and press the RETURN key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending ENTER\n");
#else
			usb_keyboard_press(KEY_ENTER, KEY_NONE);
#endif
			break;
			
		case 'T':
		case 't':
			// Read until the end of the line and press the TABULATOR key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending TAB\n");
#else
			usb_keyboard_press(KEY_TAB, KEY_NONE);
#endif
			break;
			
		case 'U':
		case 'u':
			// Read until the end of the line and press the TABULATOR key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending UP\n");
#else
			usb_keyboard_press(KEY_UP, KEY_NONE);
#endif
			break;
			
		case 'D':
		case 'd':
			// Read until the end of the line and press the TABULATOR key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending DOWN\n");
#else
			usb_keyboard_press(KEY_DOWN, KEY_NONE);
#endif
			break;
			
		case 'L':
		case 'l':
			// Read until the end of the line and press the TABULATOR key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending LEFT\n");
#else
			usb_keyboard_press(KEY_LEF, KEY_NONE);
#endif
			break;
			
		case 'R':
		case 'r':
			// Read until the end of the line and press the TABULATOR key
			chr = *send;
			while (1) {
				if (chr == '\0') break;
				if (chr == '\n') break;
				chr = *(send++);
			}
#ifdef CONSOLE_DEBUG
			printf("> sending RIGHT\n");
#else
			usb_keyboard_press(KEY_RIGHT, KEY_NONE);
#endif
			break;
	}
	
	// Recursive call until the string ends with a '0'
	if (*send != 0) {
		parse_command_lines(send);
	}
	#ifdef CONSOLE_DEBUG
	else {
		printf("> End\n");
	}
	#endif
}

/**
 * Implementation of parse_char(char *str)
 */
void parse_char(char *str) {
	int chr = *str;
	
	// Default values
	press_modifier = KEY_NONE;
	
	// Chars: ENTER, SPACE
	if (chr == 10) {
		press_key = 40;
	}
	else if (chr == 32) {
		press_key = 44;
	}
	
	// Chars: {|}~
	else if (chr > 122) {
		switch (chr) {
			case '{':
				press_key = SKEY_LEFT_CURLYBRACE;
				press_modifier = SMOD_LEFT_CURLYBRACE;
				break;
				
			case '}':
				press_key = SKEY_RIGHT_CURLYBRACE;
				press_modifier = SMOD_RIGHT_CURLYBRACE;
				break;
				
			case '|':
				press_key = SKEY_TILDE;
				press_modifier = SMOD_TILDE;
				break;
				
			case '~':
				press_key = SKEY_PIPE;
				press_modifier = SMOD_PIPE;
				break;
		}
	}
	
	// chars: a-z
	// the small 'a' is at position '4' for usb-keycodes
	else if (chr > 96) {
		press_key = chr - (97 - 4);
	}
	
	// chars: [\]^_`
	else if (chr > 90) {
		switch (chr) {
			case '[':
				press_key = SKEY_LEFT_SQUAREBRACE;
				press_modifier = SMOD_LEFT_SQUAREBRACE;
				break;
				
			case ']':
				press_key = SKEY_RIGHT_SQUAREBRACE;
				press_modifier = SMOD_RIGHT_SQUAREBRACE;
				break;
				
			case '\\':
				press_key = SKEY_BACKSLASH;
				press_modifier = SMOD_BACKSLASH;
				break;
				
			case '^':
				press_key = SKEY_ROOF;
				press_modifier = SMOD_ROOF;
				break;
				
			case '_':
				press_key = SKEY_UNDERLINE;
				press_modifier = SMOD_UNDERLINE;
				break;
				
			case '`':
				press_key = SKEY_BACKQUOTE;
				press_modifier = SMOD_BACKQUOTE;
				break;
		}
	}
	
	// chars: A-Z
	// the small 'a' is at position '4' for usb-keycodes
	else if (chr > 64) {
		press_key = chr - (65 - 4);
		press_modifier = KEY_SHIFT;
	}
	
	// chars: :;<=>?@
	else if (chr > 57) {
		switch (chr) {
			case ':':
				press_key = SKEY_DOUBLEPOINT;
				press_modifier = SMOD_DOUBLEPOINT;
				break;
				
			case ';':
				press_key = SKEY_SEMICOLON;
				press_modifier = SMOD_SEMICOLON;
				break;
				
			case '<':
				press_key = SKEY_SMALLERTHAN;
				press_modifier = SMOD_SMALLERTHAN;
				break;
				
			case '>':
				press_key = SKEY_GREATERTHAN;
				press_modifier = SMOD_GREATERTHAN;
				break;
				
			case '=':
				press_key = SKEY_EQUAL;
				press_modifier = SMOD_EQUAL;
				break;
				
			case '?':
				press_key = SKEY_QUESTION;
				press_modifier = SMOD_QUESTION;
				break;
				
			case '@':
				press_key = SKEY_AT;
				press_modifier = SMOD_AT;
				break;
		}
	}
	
	// chars: 0
	// the '0' is at position '39' for usb-keycodes
	else if (chr == 48) {
		press_key = 39;
	}
	
	// chars: 1-9
	// the '1' is at position '30' for usb-keycodes
	else if (chr > 48) {
		press_key = chr - (49 - 30);
	}
	
	// chars: !"#$%&'()*+'-./
	else if (chr > 32) {
		switch (chr) {
			case '!':
				press_key = SKEY_EXCLAMATION;
				press_modifier = SMOD_EXCLAMATION;
				break;
				
			case '"':
				press_key = SKEY_QUOTE;
				press_modifier = SMOD_QUOTE;
				break;
				
			case '#':
				press_key = SKEY_HASH;
				press_modifier = SMOD_HASH;
				break;
				
			case '$':
				press_key = SKEY_DOLLAR;
				press_modifier = SMOD_DOLLAR;
				break;
				
			case '%':
				press_key = SKEY_PERCENT;
				press_modifier = SMOD_PERCENT;
				break;
				
			case '&':
				press_key = SKEY_AND;
				press_modifier = SMOD_AND;
				break;
				
			case '\'':
				press_key = SKEY_SINGLEQUOTE;
				press_modifier = SMOD_SINGLEQUOTE;
				break;
				
			case '(':
				press_key = SKEY_LEFT_BRACE;
				press_modifier = SMOD_LEFT_BRACE;
				break;
				
			case ')':
				press_key = SKEY_RIGHT_BRACE;
				press_modifier = SMOD_RIGHT_BRACE;
				break;
				
			case '*':
				press_key = SKEY_ASTERISK;
				press_modifier = SMOD_ASTERISK;
				break;
				
			case '+':
				press_key = SKEY_PLUS;
				press_modifier = SMOD_PLUS;
				break;
				
			case '-':
				press_key = SKEY_MINUS;
				press_modifier = SMOD_MINUS;
				break;
				
			case '.':
				press_key = SKEY_PERIOD;
				press_modifier = SMOD_PERIOD;
				break;
				
			case '/':
				press_key = SKEY_SLASH;
				press_modifier = SMOD_SLASH;
				break;
		}
	}
	
	// Nothing what we know yet
	else {
		press_key = 0;
	}
	
	// Only send a KeyStrokes if the values are valid.
	// The range is from 4 up to 231
	if (press_key <= 3 || press_key > 233) {
		press_key = 0;
		press_modifier = 0;
	}
}
