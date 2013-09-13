#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "gpio-utils.h"
/* Written by Eric Ames for ECE497.
Calling instructions: the first argument is taken as the board size, while the
	next four arguments dictate from which gpio pins the on/off input is read.

General: ./etchsketch <size> <left_tog> <right_tog> <up_tog> <dwn_tog> <clr_tog>

I personally used ./etchsketch 20 48 49 51 15

THERE IS NO ERROR HANDLING. If you don't pass in arguments, it will Segfault.

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
	unsigned int A = atoi(argv[2]);
	unsigned int B = atoi(argv[3]);
	unsigned int C = atoi(argv[4]);
	unsigned int D = atoi(argv[5]);
	unsigned int E = atoi(argv[6]);

	//export gpios
	gpio_export(A);
	gpio_set_dir(A, "in");
	gpio_export(B);
	gpio_set_dir(B, "in");
	gpio_export(C);
	gpio_set_dir(C, "in");
	gpio_export(D);	
	gpio_set_dir(D, "in");
	gpio_export(E);	
	gpio_set_dir(E, "in");


	//These variables are used to check whether the toggle value has changed.
	//This way, it doesn't matter whether the toggle is high or low- only
	//the change matters.
	int value1A;
	int value1B;
	gpio_get_value(A, &value1B);
	int value2A;
	int value2B;
	gpio_get_value(B, &value2B);
	int value3A;
	int value3B;
	gpio_get_value(C, &value3B);
	int value4A;
	int value4B;
	gpio_get_value(D, &value4B);
	int value5A;
	int value5B;
	gpio_get_value(E, &value5B);

	while(1){
		//Left
		gpio_get_value(A, &value1A);
		if (value1A != value1B && x > 0){
			--x;
			redraw(size, fieldArray, x, y);
			value1B = value1A;
		}
		//Right
		gpio_get_value(B, &value2A);
		if (value2A != value2B && x < size){
			++x;
			redraw(size, fieldArray, x, y);
			value2B = value2A;
		}
		//Up
		gpio_get_value(C, &value3A);
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
		//Clear
		gpio_get_value(E, &value5A);
		if (value5A != value5B){
			initialdraw(size, &fieldArray[0][0]);
			value5B = value5A;
		}
	
	sleep(0.5);
	}




return 0;
}
