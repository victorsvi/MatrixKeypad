/**
 * Matrix Keypad
 * 
 * This example shows how to use the library to perform a blocking read of a generic keypad.
 *  
 * @version 1.1.0
 * @author Victor Henrique Salvi
 */
#include "MatrixKeypad.h"
#include <stdint.h>

const uint8_t rown = 4; //4 rows
const uint8_t coln = 3; //3 columns
uint8_t rowPins[rown] = {10, 9, 8, 7}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[coln] = {6, 5, 4}; //frist column is connect to pin 6, second to 5...
char keymap[rown][coln] = 
  {{'1','2','3'}, //key of the frist row frist column is '1', frist row second column column is '2'
   {'4','5','6'}, //key of the second row frist column is '4', second row second column column is '5'
   {'7','8','9'},
   {'*','0','#'}};
MatrixKeypad_t *keypad; //keypad is the variable that you will need to pass to the other functions

char key;

void setup() {

	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);

	keypad = MatrixKeypad_create((char*)keymap /* don't forget to do this cast */, rowPins, colPins, rown, coln); //creates the keypad object

}

void loop() {

	key = MatrixKeypad_waitForKey(keypad); //blocks the program until the user presses a key
	Serial.print(key); //prints the pressed key to the serial output

	blink (); //blinks a led everytime that's called to show that the keypad scanning is blocking the program
	
	delay(20); //do something
}

/* inverts the led state */
void blink () {
	static int led_state = LOW;
	if(led_state == HIGH){
		led_state = LOW;
	}
	else {
		led_state = HIGH;
	}
	digitalWrite(LED_BUILTIN, led_state);
}