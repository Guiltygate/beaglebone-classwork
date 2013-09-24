#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "gpio-utils.h"
#include <errno.h>
#include "i2c-dev.h"
#include "i2cbusses.h"

#define BICOLOR		// undef if using a single color display

/* Written by Eric Ames for ECE497. (Much of the i2c work was adapted from others.)
Calling instructions: the first argument is taken as the board size, the second an optional decimal register (for later use, can be ignored now). 3rd and 4th are both the AIN # to call for the joystick. Final argument is the gpio pin to be read for the joystick toggle.

General: ./etchsketch <tempAddr1> <x_axis_gpio> <y_axis_gpio> <toggle_gpio> <btn_1> <btn_2>

I personally use ./etchsketch 72 6 4 15 112 115

THERE IS NO ERROR HANDLING. If you pass in bad arguments, bad things will happen.

NOTE: This program would not be possible without the use of gpio-utils.c and its header, both of which are NOT MY OWN WORK, but from https://www.ridgerun.com/developer/wiki/index.php/
*/

static __u16 initial1_bmp[]=
	{0b0000000011111111, 0b0000000011111111, 0b0000000011111111,
	 0b0000000011111111, 0b0000000011111111, 0b0000000011111111,
	 0b0000000011111111, 0b0000000011111111};

static __u16 initial2_bmp[]=
	{0b1111111100000000, 0b1111111100000000, 0b1111111100000000,
	 0b1111111100000000, 0b1111111100000000, 0b1111111100000000,
	 0b1111111100000000, 0b1111111100000000};

static __u16 initial3_bmp[]=
	{0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
	 0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
	 0b0000000000000000, 0b0000000000000000};

static __u16 gif0[]=
	{0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
static __u16 gif1[]=
	{0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
static __u16 gif2[]=
	{0xf0ff, 0x01, 0x01, 0x01, 0x0, 0x0, 0x0, 0x0};
static __u16 gif3[]=
	{0xffff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
static __u16 gif4[]=
	{0xffff, 0x0101, 0x0101, 0x0101, 0x01, 0x01, 0x01, 0x1f};
static __u16 gif5[]=
	{0xffff, 0x0101, 0x0101, 0x0101, 0x0101, 0x0101, 0x0101, 0xff};
static __u16 gif6[]=
	{0xffff, 0x0101, 0x0181, 0x0181, 0x0181, 0x0181, 0x0181, 0x01ff};
static __u16 gif7[]=
	{0xffff, 0x0101, 0x81f9, 0x8181, 0x8181, 0x8181, 0x8181, 0xffff};
static __u16 gif8[]=
	{0xffff, 0x0101, 0xf9fd, 0x8185, 0x8185, 0x8185, 0x8181, 0xffff};
static __u16 gif9[]=
	{0xffff, 0x0101, 0xfdfd, 0x8585, 0x8585, 0x859d, 0x8181, 0xffff};
static __u16 gif10[]=
	{0xffff, 0x0101, 0xfdfd, 0x8585, 0x8585, 0x9dbd, 0x8181, 0xffff};
static __u16_gif11[]=
	{0xffff, 0x0101, 0xfdfd, 0x8585, 0x85a5, 0x9dbd, 0x8181, 0xffff};

static int check_funcs(int file) {
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
		fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
		return -1;
	}
	return 0;
}

// Writes block of data to the display
static int write_block(int file, __u16 *data) {
	int res;
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)data);
	return res;
}



//Draws the initialboard with 1 'x'
static int initialdraw(int size, char* fieldArray, int file){
	int i=0; int l=0;
	for(i; i<size; i++){
		for(l; l<size; l++){
			*fieldArray = '0';
			fieldArray++;
		}
		l=0;
	}
	write_block(file, initial1_bmp);
	sleep(1);
	write_block(file, initial2_bmp);
	sleep(1);
	write_block(file, initial3_bmp);
return 0;
}

//Everytime a button is pressed, redraw is called to update the board.
static int redraw(int size, char fieldArray[size][size], int x, int y, int file){
	int i=0; int l=0;
	char * ptr;
	long parsed;
	__u16 write_array[8];
	char str[8];

	fieldArray[y][x] = '1';

	for(i; i<size; i++){
		for(l; l<size; l++){
			str[l] = fieldArray[i][l];
		}
	parsed = strtol(str, & ptr, 2);
	sprintf(str, "%ld", parsed);
	write_array[i] = strtol(str, & ptr, 10);
	l=0;
	}
	write_block(file, write_array);

return 0;
}

//Main argument
int main(int argc, char* argv[]){

	//Initial setup of variables	
	int size = 8;
	char fieldArray[size][size];
	int x = 0; int y = 0;


//i2c Setup-------------------------------------------------------

	int res, i2cbus, address, file;
	char filename[20];
	int force = 0;

	i2cbus = lookup_i2c_bus("1");
	printf("i2cbus = %d\n", i2cbus);

	address = parse_i2c_address("0x70");
	printf("address = 0x%2x\n", address);

	file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
	if (file < 0
	 || check_funcs(file)
	 || set_slave_addr(file, address, force))
		//exit(1);

	// Check the return value on these if there is trouble
	i2c_smbus_write_byte(file, 0x21); // Start oscillator (p10)
	i2c_smbus_write_byte(file, 0x81); // Disp on, blink off (p11)
	i2c_smbus_write_byte(file, 0xe7); // Full brightness (page 15)


//i2c Setup-------------------------------------------------------

	//Draw initial board
	initialdraw(size, &fieldArray[0][0], file);

	//Grab gpio values from input arguments
	unsigned int tempAddr = atoi(argv[1]);
	unsigned int ainA = atoi(argv[2]);
	unsigned int ainB = atoi(argv[3]);
	unsigned int toggle = atoi(argv[4]);
	unsigned int btn1 = atoi(argv[5]);
	unsigned int btn2 = atoi(argv[6]);

	//export select gpios
	gpio_export(toggle);	
	gpio_set_dir(toggle, "in");
	gpio_export(btn1);	
	gpio_set_dir(btn1, "in");
	gpio_export(btn2);	
	gpio_set_dir(btn2, "in");


	//These variables are used to check for position and clearing the board.
	int x_pos, y_pos;
	int sel = 1;
	int value1, value2;

//CHANGE FOR AIN USE, NOT GPIOs
/*	1) Convert toggles to AIN - DONE
	2) Draw to LED grid - CURRENT
	3) Setup TMP to clear
*/
	int i = 0;
	while(1){
		gpio_get_value(btn1, &value1);
		gpio_get_value(btn2, &value2);
		//Left & Right
/*		ain_get_value(ainA, &x_pos);
		ain_get_value(ainA, &x_pos);
		if (x_pos>1700 && x > 0){
			--x;
			redraw(size, fieldArray, x, y, file);
		}
		else if (x_pos<100 && x < size-1){
			++x;
			redraw(size, fieldArray, x, y, file);
		}
		//Up & Down
		ain_get_value(ainB, &y_pos);
		ain_get_value(ainB, &y_pos);
		if (y_pos>1600 && y > 0){
			--y;
			redraw(size, fieldArray, x, y, file);
		}
		else if (y_pos<200 && y < size-1){
			++y;
			redraw(size, fieldArray, x, y, file);
		}
*/
		if (value1 == 1 && value2 == 0 && x > 0){
			--x;
			redraw(size, fieldArray, x, y, file);
		}
		if (value1 == 0 && value2 == 1 && x < size-1){
			++x;
			redraw(size, fieldArray, x, y, file);
		}
		if (value1 == 1 && value2 == 1 && y < size-1){
			++y;
			redraw(size, fieldArray, x, y, file);
		}
		if (value1 == 1 && sel == 0 && y > 0){
			--y;
			redraw(size, fieldArray, x, y, file);
			sel == 1;
		}
		if (value1 == 1 && value2 == 1 && sel == 0){
			initialdraw(size, &fieldArray[0][0], file);
			write_block(file, gif0);sleep(1);write_block(file, gif1);sleep(1);
			write_block(file, gif2);sleep(1);write_block(file, gif3);sleep(1);
			write_block(file, gif4);sleep(1);write_block(file, gif5);sleep(1);
			write_block(file, gif6);sleep(1);write_block(file, gif7);sleep(1);
			write_block(file, gif8);sleep(1);write_block(file, gif9);sleep(1);
			write_block(file, gif10);sleep(1);
		}


		//Clear method 1 //CHANGE
		gpio_get_value(toggle, &sel);
		if (sel == 0 && value1 == 0 && value2 == 0){
			initialdraw(size, &fieldArray[0][0], file);
			sel == 1;
		}
	printf("Running...\n");
	//printf("X pos: %d\n", x_pos);
	//printf("Y pos: %d\n", y_pos);

	sleep(1);

	}



close(file);
return 0;
}
