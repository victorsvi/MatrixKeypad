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
 * @file MatrixKeypad.c
 * @version 1.1.0
 * @author Victor Henrique Salvi
 * 
 * Simple to use c-like Arduino library to interface matrix keypads.
 *
 */
#include "MatrixKeypad.h"
#include "Arduino.h"
#include <stdlib.h>

MatrixKeypad_t *MatrixKeypad_create (char *keymap, uint8_t *rowPins, uint8_t *colPins, uint8_t rown, uint8_t coln){
	
	MatrixKeypad_t *keypad;
	uint8_t i;

	keypad = malloc(sizeof(MatrixKeypad_t));
	if(keypad == NULL) {
		return NULL;
	}
	
	keypad->rown = rown;
	keypad->coln = coln;
	keypad->rowPins = rowPins;
	keypad->colPins = colPins;
	keypad->keyMap = keymap;
	keypad->lastKey = '\0';
	keypad->buffer = '\0';
	
	/* How the hardware works
	 * 
	 * The keypad is a matrix which each row and column is a wire. All wires are disconnected from each other.
	 * When a button is pressed the corresponding row and column wires are shorted.
	 * To detect that, the rows are set as outputs and held high and the columns are set as inputs with pullup resistors.
	 * To scan a row, the row is set to low. If a key is pressed, the corresponding column will read as low. 
	 */
	for(i = 0; i < keypad->rown; i++){
		pinMode(keypad->rowPins[i], OUTPUT);
		digitalWrite(keypad->rowPins[i], HIGH);
	}
	for(i = 0; i < keypad->coln; i++){
		pinMode(keypad->colPins[i], INPUT_PULLUP);
	}
    
	return keypad;
}

void MatrixKeypad_scan (MatrixKeypad_t *keypad){
	
	uint8_t row, col;
	char key = '\0'; /* the "not detected" key */
	
	if(keypad != NULL) {
		
		/* How the hardware works
		 * 
		 * The keypad is a matrix which each row and column is a wire. All wires are disconnected from each other.
		 * When a button is pressed the corresponding row and column wires are shorted.
		 * To detect that, the rows are set as outputs and held high and the columns are set as inputs with pullup resistors.
		 * To scan a row, the row is set to low. If a key is pressed, the corresponding column will read as low. 
		 * On the other side, if none key is pressed, the corresponding column will read as high.
		 * 
		 * To scan the keypad, each row is set to low and each column is read. If it reads a column as high, the corresponding key is pressed.
		 */
		for(row = 0; row < keypad->rown; row++){
			
			digitalWrite(keypad->rowPins[row], LOW);
			for(col = 0; col < keypad->coln; col++){
				if(digitalRead(keypad->colPins[col]) == LOW) {
					key = keypad->keyMap[row * keypad->coln + col]; /* imagine as keyMap[row][col] */
				}
			}			
			digitalWrite(keypad->rowPins[row], HIGH);
		}
		
		if(keypad->lastKey != key) {	/* saves the key in the buffer only if the last key was released */
			keypad->lastKey = key;		/* because the buffer is flushed after a reading */
			if(key != '\0') {			/* don't overwrite the buffer when the key is released. Important when the scan interval is higher than the time of the keypress */
				keypad->buffer = key;
			}
		}
	}

}

uint8_t MatrixKeypad_hasKey (MatrixKeypad_t *keypad){
	
	if(keypad == NULL) {
		return 0;
	}
	
	if(keypad->buffer != '\0'){
		return 1;
	}
	
	return 0;
}

char MatrixKeypad_getKey (MatrixKeypad_t *keypad){
	
	char key;
	
	if(keypad == NULL) {
		return '\0';
	}
	
	key = keypad->buffer;
	keypad->buffer = '\0'; /* a key press event can be read only one time.
						    * the buffer is cleared after its read to avoid reading
							* the same key press event many times
							*/
	
	return key;
}

char MatrixKeypad_waitForKey (MatrixKeypad_t *keypad){
	
	char key;
	
	if(keypad == NULL) {
		return '\0';
	}
	
	/* scans the keypad until a key is pressed */
	while(!MatrixKeypad_hasKey(keypad)) {	
		MatrixKeypad_scan(keypad);
	}
	key = MatrixKeypad_getKey(keypad);
	
	return key;
}

char MatrixKeypad_waitForKeyTimeout (MatrixKeypad_t *keypad, uint16_t timeout){
	
	char key;
	uint16_t startTime = millis();
	
	if(keypad == NULL) {
		return '\0';
	}
	
	/* scans the keypad until a key is pressed or a timeout occurs */
	while(!MatrixKeypad_hasKey(keypad) && ((millis() - startTime) <= timeout)) {	
		MatrixKeypad_scan(keypad);
	}
	key = MatrixKeypad_getKey(keypad);
	
	return key;
}

void MatrixKeypad_flush (MatrixKeypad_t *keypad){
 
	if(keypad != NULL) {

		keypad->buffer = '\0';
	} 
}
