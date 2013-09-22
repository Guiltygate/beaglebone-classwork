#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "gpio-utils.h"
#include "i2c-dev.h"
/* Written by Eric Ames for ECE497.
Calling instructions: the first argument is taken as the board size, while the
	next four arguments dictate from which gpio pins the on/off input is read.

General: ./etchsketch <size> <tempAddr1> <x_axis_gpio> <y_axis_gpio> <toggle_gpio>

I personally use ./etchsketch 20 72 ##51 15 60##

THERE IS NO ERROR HANDLING. If you pass in bad arguments, it will Segfault.

NOTE: This program would not be possible without the use of gpio-utils.c and its header, both of which are NOT MY OWN WORK, but from https://www.ridgerun.com/developer/wiki/index.php/
*/


//Draws the initialboard with 1 'x'
int initialdraw(int size, char* fieldArray){
	int i=0; int l=0;
	for(i; i<size; i++){
		for(l; l<size; l++){
			*fieldArray = ' ';
			printf("%c", *fieldArray);
			fieldArray++;
		}
		l=0;
		printf("\n");
	}
return 0;
}

//Everytime a button is pressed, redraw is called to update the board.
int redraw(int size, char fieldArray[size][size], int x, int y){
	int i=0; int l=0;
	printf("Redrawing with %d and %d.\n", x, y);
	fieldArray[y][x] = 'x';

	for(i; i<size; i++){
		for(l; l<size; l++){
			printf("%c", fieldArray[i][l]);
		}
		l=0;
		printf("\n");
	}	

return 0;
}

//Main argument
int main(int argc, char* argv[]){
	printf("Starting!\n");
	//Initial setup of variables	
	int size = atoi(argv[1]);
	char fieldArray[size][size];
	int x = 0;
	int y = 0;

	//Draw initial board
	printf("Begin initial draw...");
	initialdraw(size, &fieldArray[0][0]);
	printf("done.\n");

	//Grab gpio values from input arguments
	unsigned int tempAddr = atoi(argv[2]);
	unsigned int B = atoi(argv[3]);
	unsigned int C = atoi(argv[4]);
	unsigned int D = atoi(argv[5]);

	//export select gpios
	gpio_export(D);	
	gpio_set_dir(D, "in");


	//These variables are used to check whether the toggle value has changed.
	//This way, it doesn't matter whether the toggle is high or low- only
	//the change matters.
	int x_pos, y_pos, sel;

//CHANGE FOR AIN USE, NOT GPIOs
//Focus on exploring AIN use for joystick, don't worry about temp yet.
//Also, forgot, need to draw to pin grid?
/*	1) Convert toggles to AIN
	2) Draw to LED grid
	3) Setup TMP to clear
*/
	while(1){
		//Left
		gpio_get_value(B, &value1A);
		if (value1A != value1B && x > 0){
			--x;
			redraw(size, fieldArray, x, y);
			value1B = value1A;
		}
		//Right
		gpio_get_value(C, &value2A);
		if (value2A != value2B && x < size){
			++x;
			redraw(size, fieldArray, x, y);
			value2B = value2A;
		}
		//Up
		gpio_get_value(D, &value3A);
		if (value3A != value3B && y > 0){
			--y;
			redraw(size, fieldArray, x, y);
			value3B = value3A;
		}
		//Down
		gpio_get_value(D, &value4A);
		if (value4A != value4B && y < size){
			++y;
			redraw(size, fieldArray, x, y);
			value4B = value4A;
		}
		//Clear method 1
		gpio_get_value(D, &sel);
		if (sel == 1){
			initialdraw(size, &fieldArray[0][0]);
		}
		//Clear method 2
/*		gpio_get_value(E, &value5A);
		if (value5A != value5B){
			initialdraw(size, &fieldArray[0][0]);
			value5B = value5A;
		}
	*/
	sleep(0.5);
	}




return 0;
}
