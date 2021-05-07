# MatrixKeypad

[![Arduino Compatible](https://github.com/victorsvi/MatrixKeypad/actions/workflows/Arduino%20Compatible.yml/badge.svg)](https://github.com/victorsvi/MatrixKeypad/actions?workflow=Arduino%20Compatible)

Simple to use Arduino library to interface matrix keypads.

### Features

- Blocking or non-blocking read; 
- Supports any number of rows and columns; 
- User defined key mapping;
- prevents reading the same event twice. 

### Limitations

- Don't handle multiples keypress simultaneously; 
- Saves only the last key pressed.

## How to use

### Instalation 

You can instal the library by:
1. Open the Library Manager inside Arduino IDE, find for "MatrixKeypad" and click install;
2. Go to _Releases_, download the .zip file, import the file inside Arduino IDE. 

### Pin and key mapping

A matrix keypad will have some pins or wires that you have to connect to digital inputs of the arduino.
Each of those pins is either connected to a column or a row. You will need to sort out which is which. To to that, you can use a multimeter in continuity mode and do a iterative method:
1. Place each probe to a distinct pin;
2. Start pressing the keypad's buttons;
3. If you detect continuity, you are probing the row and column pins of the key you are pressing;
4. If you gone trough all the keys and found no continuity, you are either probing two rows or two columns;
5. Keep doing that until you are sure which pin is connected to which row or column.

Then you can connect the keypad to the Arduino and define the two pin mappings, one for the rows and another for the columns. The pin mapping is a vector that holds the pin number indexed by the order of the row or column. You can use the top left of the keypad as a referential, so the first column to the left is the index 0, the second is the index 1...
 
The key mapping, is a bidimentional array that holds the key character that will be returned by the library indexed by row and column, using the same referential as the pin mappings.

Here is a example of a 4x3 keypad:
```
	 C0  C1  C2
	 --- --- ---
R0	| 1 | 2 | 3 |
	 --- --- ---
R1	| 4 | 5 | 6 |
	 --- --- ---
R2	| 7 | 8 | 9 |
	 --- --- ---
R3	| * | 0 | # |
	 --- --- ---
```
The pins connection are
|Keypad Pin|Arduino Pin|
|---|---|
|R0|10|
|R1|9|
|R2|8|
|R3|7|
|C0|6|
|C1|5|
|C2|4|

The resulting mappings are
```c
uint8_t rown = 4; //4 rows
uint8_t coln = 3; //3 columns
uint8_t rowPins[rown] = {10, 9, 8, 7}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[coln] = {6, 5, 4}; //frist column is connect to pin 6, second to 5...
char keymap[rown][coln] = 
  {{'1','2','3'}, //key of the frist row frist column is '1', frist row second column column is '2'
   {'4','5','6'}, //key of the second row frist column is '4', second row second column column is '5'
   {'7','8','9'},
   {'*','0','#'}};
```

You can find more information about this in the [documentation](../master/docs/api.md) or by [practical examples](../master/examples).

### Non-bloking read

To perform a non-blocking read, you will need to:
1. Define your pin and key mapping;
2. create a pointer variable of the type MatrixKeypad_t;
3. call MatrixKeypad_create in "setup()" to initialize the keypad object;
4. call Keypad_scan in "loop()" to scan the keypad for keypresses;
5. call MatrixKeypad_hasKey or MatrixKeypad_getKey to know if a key was pressed and retrieve the key.
You can check out this [example skecth](../master/examples/MatrixKeypadNonBlocking/MatrixKeypadNonBlocking.ino).

### Bloking read

To perform a blocking read, you will need to:
1. Define your pin and key mapping;
2. create a pointer variable of the type MatrixKeypad_t;
3. call MatrixKeypad_create in "setup()" to initialize the keypad object;
4. call MatrixKeypad_waitForKey when you want to wait for the user input.
You can check out this [example skecth](../master/examples/MatrixKeypadBlocking/MatrixKeypadBlocking.ino).

## Documentation

Read the documentation [here](../master/docs/api.md)
