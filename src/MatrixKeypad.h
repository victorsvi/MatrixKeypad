/*
	MatrixKeypad - Simple to use c-like Arduino library to interface matrix keypads.
	Copyright (C) 2021 Victor Henrique Salvi

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
	USA
*/
/** 
 * @file MatrixKeypad.h
 * @version 1.1.0
 * @author Victor Henrique Salvi
 * 
 * Simple to use c-like Arduino library to interface matrix keypads.
 *
 * Features
 *  - blocking or non-blocking read; 
 *  - supports any number of rows and columns; 
 *  - user defined key mapping; 
 *  - prevents reading the same event twice.
 * 
 * Limitations 
 *  - don't handle multiples keypress simultaneously; 
 *  - saves only the last key pressed.
 *  
 * Changelog
 * |Version|Date|Contributor|Description|
 * |---|---|---|---|
 * |1.1.0|2021/05/05|Victor Salvi|Added the MatrixKeypad_waitForKeyTimeout function|
 * |1.0.0|2021/05/05|Victor Salvi|Added the files to be compatible to the Arduino Library Manager (examples, properties file, keywords)|
 * |1.0.0|2021/05/05|Victor Salvi|Source code and usage documentation|
 * |1.0.0|2019/06/06|Victor Salvi|First Implementation|
 *
 */
#ifndef MATRIXKEYPAD_H
#define MATRIXKEYPAD_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdint.h>

/** 
 * structure that holds the physical parameters of the keypad, the pin mapping, the key mapping and the state variables
 */
typedef struct {
	uint8_t rown; /**< Number of rows. Must be greater than zero */
	uint8_t coln; /**< Number of columns. Must be greater than zero */
	uint8_t *rowPins; /**< Pin mapping for the rows. These pins are set as output. Is a unidimentional matrix with length = "rown" */
	uint8_t *colPins; /**< Pin mapping for the columns. These pins are set as inputs. Is a unidimentional matrix with length = "coln" */
	char *keyMap; /**< Key mapping for the keypad. Its a bidimentional matrix with "rown" rows and "coln" columns. When a keypress is detect at row R and column C, the returned key is the one at keyMap[R][C]. The key mapping is directly related to the pin mappings. Dont use '\0' as a mapped key  */
	char lastKey; /**< Holds the last key detected. Used to avoid the same keypress to be read multiple times */
	char buffer; /**< Holds the last key accepted. Is cleared after the keypress is requested. Its overwritten if a new key is pressed before the old one is requested */
} MatrixKeypad_t;

/** 
 * Creates a keypad object that represents the physical keypad and the pin mappings
 * 
 * A matrix keypad will have some pins or wires that you have to connect to digital inputs of the arduino.
 * You will need to sort out which pins are connected to the rows, which are connected to the colluns and their ordering. 
 * Then define a matrix and initialize it with the ordered row pin number. Define another matrix for the columns.
 * To create the key mapping, define a bidimentional array and initialize with the character to be returned when the key on it's place is pressed.
 * Note that the key mapping ordering is directly related to the pin mapping ordering.
 * The library don't make a copy of mappings to use less storage. The library references the mappings defined in the main sketch, so they can't be reporpused or edited.
 * 
 * As an example, consider a 4x3 keypad:
 * 
@code{.c}
uint8_t rown = 4; //4 rows
uint8_t coln = 3; //3 columns
uint8_t rowPins[rown] = {10, 9, 8, 7}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[coln] = {6, 5, 4}; //frist column is connect to pin 6, second to 5...
char keymap[rown][coln] = 
  {{'1','2','3'}, //key of the frist row frist column is '1', frist row second column column is '2'
   {'4','5','6'}, //key of the second row frist column is '4', second row second column column is '5'
   {'7','8','9'},
   {'*','0','#'}};
MatrixKeypad_t *keypad = MatrixKeypad_create((char*)keymap, rowPins, colPins, rown, coln); //keypad is the variable that you will need to pass to the other functions
@endcode 
 * 
 * @param keymap Key mapping for the keypad. Its a bidimentional matrix with "rown" rows and "coln" columns. You can define a variable as "char keymap[rown][coln]" and cast it as "(char*)keymap".
 * @param rowPins Pin mapping for the rows. Is a unidimentional matrix with length "rown".
 * @param colPins Pin mapping for the columns. Is a unidimentional matrix with length "coln".
 * @param rown Number of rows. Must be greater than zero.
 * @param coln Number of columns. Must be greater than zero.
 * @return A pointer to the structure representing the keypad.
 * @since 1.0.0
 */
MatrixKeypad_t *MatrixKeypad_create (char *keymap, uint8_t *rowPins, uint8_t *colPins, uint8_t rown, uint8_t coln);

/** 
 * Scans the keypad to check if a key is currently pressed.
 * The time interval between scans will affect the responsiveness of the keypad. 
 * This function must be called inside the "loop()" function to scan the keypad periodically if you are using the NON-BLOCKING reading mode.
 * A interval too long will make the keybord miss press events. However, a interval too short will consume unnecessary cpu time. A interval between 20ms and 100ms.
 * You can put a lower limit on the scan interval saving the time of the last scan. Example to limit to at least 100ms using the variable lastScan (long):
 * 
@code{.c}
if((millis() - lastScan) >= 100) {
   MatrixKeypad_scan(keypad);
   lastScan = millis();
}
@endcode 
 * 
 * @param keypad The keypad object returned by MatrixKeypad_create.
 * @since 1.0.0
 */
void MatrixKeypad_scan (MatrixKeypad_t *keypad);

/** 
 * Checks if a keypress was detected.
 * 
 * @param keypad The keypad object returned by MatrixKeypad_create.
 * @return 1 if a key was pressed or 0 if none was pressed.
 * @since 1.0.0
 */
uint8_t MatrixKeypad_hasKey (MatrixKeypad_t *keypad);

/** 
 * Returns the last key pressed.
 * This function is NON-BLOCKING. It won't scan the keyboard for new events or wait for a event.
 * You must use the MatrixKeypad_scan function to scan the keypad periodically.
 * 
 * @param keypad The keypad object returned by MatrixKeypad_create.
 * @return The pressed key character from the key mapping or '\0' (null character) if none key was pressed.
 * @since 1.0.0
 */
char MatrixKeypad_getKey (MatrixKeypad_t *keypad);

/** 
 * Waits until a key is pressed and returns it.
 * If there is a unread event in the buffer, that event is returned instead.
 * This function is BLOCKING. The program will freeze until a key press is detected.
 * 
 * @param keypad The keypad object returned by MatrixKeypad_create.
 * @return The pressed key character from the key mapping.
 * @since 1.0.0
 */
char MatrixKeypad_waitForKey (MatrixKeypad_t *keypad);

/** 
 * Waits until a key is pressed and returns it.
 * If there is a unread event in the buffer, that event is returned instead.
 * This function is BLOCKING. The program will freeze until a key press is detected or it timeouts.
 * 
 * @param keypad The keypad object returned by MatrixKeypad_create.
 * @param timeout Maximum time in milliseconds to wait for a event.
 * @return The pressed key character from the key mapping or '\0' if a timeout occurs.
 * @since 1.1.0
 */
char MatrixKeypad_waitForKeyTimeout (MatrixKeypad_t *keypad, uint16_t timeout);

/** 
 * Cleans the unread keys buffer.
 * You can use this function to flush the queued keypresses that weren't read by MatrixKeypad_getKey.
 * 
 * @param keypad The keypad object returned by MatrixKeypad_create.
 * @since 1.0.0
 */
void MatrixKeypad_flush (MatrixKeypad_t *keypad);

#ifdef __cplusplus
	}
#endif

#endif
