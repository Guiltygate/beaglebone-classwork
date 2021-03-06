#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> // For usleep()
#include <signal.h> // Defines signal-handling functions (i.e. trap Ctrl-C)
#include "gpio.h"

#define PIN_MUX_PATH "/sys/kernel/debug/omap_mux/"
#define MAX_BUF 64

/*MAIN function written by Eric Ames and David Cooper;
	sub-functions taken from LightTracker.c

./luxCanis <toggle_button> <AINPin1> <AINPin2>

Both AIN pins are given a default value in case none is entered. The default
pins are AIN0 and AIN1
*/

//Globals
int controlGPIOs[5];
char ain[2];

//*****************BEGIN Functions taken from LightTracker******************
//Sets the pin muxes on the gpios
int mode_gpio_out(char *pinMux){
	int fd, len;
	char buf[MAX_BUF];
 
	len = snprintf(buf, sizeof(buf), PIN_MUX_PATH "%s", pinMux);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0){
		perror("mode/gpio");
		return fd;
	}
 
	write(fd, "7", 2); 
	close(fd);
	return 0;
}

//Reads analog input
int analogIn(char ainn)
{
	FILE *fp;
	char ainPath[MAX_BUF];
	char ainVal[MAX_BUF];
	
    snprintf(ainPath, sizeof ainPath, "/sys/devices/ocp.2/helper.14/AIN%c", ainn);

	if((fp = fopen(ainPath, "r")) == NULL){
	printf("Can't open this pin, %c\n", ainn);
	return 1;
	}

	fgets(ainVal, MAX_BUF, fp);

	fclose(fp);
	return atoi(ainVal);		
}

//Even though it's annoying to be locked into these 4 gpio pins, the fact
//that they'll always work without physical adjustment is a nice feature
void initIO(){
	//Mode0 names
	char gpio1[] = "gpmc_wait0";
	char gpio2[] = "gpmc_wpn";
	char gpio3[] = "gpmc_a0";
	char gpio4[] = "spi0_cs0";

	int i;

	//Set pin mux in gpio output mode for controlGPIOss
#ifdef SET_PIN_MUX
mode_gpio_out(gpio1);
	mode_gpio_out(gpio2);
	mode_gpio_out(gpio3);
	mode_gpio_out(gpio4);
#endif

	//Export gpios and set up output direction for controlGPIOss
	for (i = 0; i < 5; i++){
		gpio_export(controlGPIOs[i]);
		gpio_set_direction(controlGPIOs[i], 1);
	}
	//Toggle switch gpio
	printf("%d\n", controlGPIOs[4]);
	gpio_export(controlGPIOs[4]);
	gpio_set_direction(controlGPIOs[4], 0);
	
}

//Rotate clockwise
void rotateClock (int current){
	//Use a 4 state position to decide how to rotate
	switch(current){
	case 0:	gpio_set_value(controlGPIOs[0], 0);
			gpio_set_value(controlGPIOs[1], 1);
			gpio_set_value(controlGPIOs[2], 1);
			gpio_set_value(controlGPIOs[3], 0);
			break;

	case 1:	gpio_set_value(controlGPIOs[0], 1);
			gpio_set_value(controlGPIOs[1], 1);	
			gpio_set_value(controlGPIOs[2], 0);
			gpio_set_value(controlGPIOs[3], 0);
			break;

	case 2:	gpio_set_value(controlGPIOs[0], 1);
			gpio_set_value(controlGPIOs[1], 0);
			gpio_set_value(controlGPIOs[2], 0);
			gpio_set_value(controlGPIOs[3], 1);
			break;

	case 3:	gpio_set_value(controlGPIOs[0], 0);
			gpio_set_value(controlGPIOs[1], 0);
			gpio_set_value(controlGPIOs[2], 1);
			gpio_set_value(controlGPIOs[3], 1);
			break;
	}
	usleep(60000);

}

//*****************END Functions taken from LightTracker******************


//MAIN function.
int main(int argc, char *argv[]){
		system("echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots");
		
		controlGPIOs[0] = 30;
		controlGPIOs[1] = 31;
		controlGPIOs[2] = 48;
		controlGPIOs[3] = 51;
	
	int i, k, samples[20], min, toggle, minIndex, contine, counter, minFound, difference;
	minFound = 0;
	int rotation = 0;
	min = 99999;
	counter = 0;
	contine = 1;
	if(argc < 3){
		printf("Fewers than 2 arguments detected: using default values.\n");
		ain[0] = '0';
		ain[1] = '1';
		controlGPIOs[4] = 14;
	}else{
		ain[0] = argv[2][0];
		ain[1] = argv[3][0];
		controlGPIOs[4] = atoi(argv[1]);
	}

	//Initialize IO
	initIO();

while(contine){	
	gpio_get_value(controlGPIOs[4], &toggle);
	//if toggle pressed, rotate once then counter-rotate to light source
	if(toggle == 0){
	//Clockwise rotate for a cycle and record the value in different directions
		for(i=0; i<5; i++){
			for(k=0; k<4; k++){
				rotateClock(k);
				samples[counter] = analogIn(ain[0])+analogIn(ain[1]);
				if(min>samples[counter]){
				minIndex=counter;min=samples[counter];}
				counter++;
			}
		}
		
		printf("%d\n", minIndex);
		counter = 0;
		
		for(k=0; k<((20-minIndex)/4); k++){
			for(i=0; i<4; i++){
				rotateClock(3-i);
				counter++;
			}
		}
		for(i=0; i<((20-minIndex)%4); i++){
			rotateClock(3-i);
			counter++;
		}
		minFound = 1;
	}
	//Track light source

	if(minFound == 1){
		difference = analogIn(ain[0])-(analogIn(ain[1])-500);
		printf("%d\n", difference);
		//Moving Left and Right
		if(rotation >= 3){
			rotation = 0;
		}
		if(difference > 400){
			rotateClock(rotation);
			rotation++;
			rotateClock(rotation);
			rotation++;
		}else if(difference < -400){
			rotateClock(3-rotation);
			rotation++;
			rotateClock(3-rotation);
			rotation++;
		}
		
		fflush(stdout);
		usleep(250000);
	}
}
return 0;
}


