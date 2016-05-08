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
 * For debuging compile it like this:
 * shell> gcc -DCONSOLE_DEBUG keyboard_payload.c -o main
 * shell> ./main
 */

// Define the Keyboard-Layout you want use
// See https://www.terena.org/activities/multiling/ml-mua/test/kbd-all.html
// See http://home.datacomm.ch/t.bigler/sskbdsg.htm
// For now: MAPPING_CH, MAPPING_DE, MAPPING_US (for all others)
//#define MAPPING_US
#define MAPPING_CH
//#define MAPPING_DE

// Uncomment the next line for debugging on a console and not using it on a teensy
//#define CONSOLE_DEBUG

