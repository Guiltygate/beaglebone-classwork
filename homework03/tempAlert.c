#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "gpio-utils.h"
#include <errno.h>
#include "i2c-dev.h"
#include <fcntl.h>
/* Written by Eric Ames for ECE497.
Calling instructions: 

General: ./tempAlert <HexAddress1> <HexAddress2> <HexTempHi> <HexTempLow> <AlertGPIO> <DecimalAddress1> <DecimalAddress2>

I personally use ./tempAlert 48 49 20 1c 14 72 73

Apologies to the user for having you enter both the hex and decimal addresses-
I'm in a bit of a time crunch with 3 projects due, and the commands weren't playing
nice.

THERE IS NO ERROR HANDLING. If you don't pass in correct arguments, it will Segfault.

i2c bus 1 and daddress 0 is assumed- I didn't want to take any more arguments from the user.

NOTE: This program would not be possible without the use of gpio-utils.c and its header, both of which are NOT MY OWN WORK, but from https://www.ridgerun.com/developer/wiki/index.php/
*/

int main(int argc, char* argv[]){
	
	char *end;
	int res, size, file, address, i2cbus;
	int daddress;
	char filename[20];

	/* handle (optional) flags first */
	if(argc < 5) {
		printf("Please give i2c addresses and temperature settings\n");
		exit(1);
	}

	i2cbus   = 1;
	address  = atoi(argv[1]);
	daddress = 0;
	size = I2C_SMBUS_BYTE;

	int address2 = atoi(argv[2]);
	int TempHi = atoi(argv[3]);
	int TempLow = atoi(argv[4]);
	int gpio = atoi(argv[5]);
	int low = 1;

	//Compose
	char script[30];
	strcpy(script, "./tempAlertSetup.sh ");
	strcat(script, argv[6]);
	strcat(script, " ");
	strcat(script, argv[7]);
	strcat(script, " ");
	strcat(script, argv[3]);
	strcat(script, " ");
	strcat(script, argv[4]);
	strcat(script, " ");
	strcat(script, argv[5]);
	fflush(stdout);

	system(script);

	sprintf(filename, "/dev/i2c-%d", i2cbus);
	file = open(filename, O_RDWR);
	if (file<0) {
		if (errno == ENOENT) {
			fprintf(stderr, "Error: Could not open file "
				"/dev/i2c-%d: %s\n", i2cbus, strerror(ENOENT));
		} else {
			fprintf(stderr, "Error: Could not open file "
				"`%s': %s\n", filename, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) {
		fprintf(stderr,
			"Error: Could not set address to 0x%02x: %s\n",
			address, strerror(errno));
		return -errno;
	}
	//Infinite loop for reading Temp
	while(1){
		res = i2c_smbus_read_byte_data(file, daddress);
		gpio_get_value(gpio, &low);
		if (res < 0) {
			fprintf(stderr, "Error: Read failed, res=%d\n", res);
			exit(2);
		}
		if (low == 0){
			printf("Above TempHi Threshold!\n");

			int fConvert1 = (((res*9)/5)+32);

			printf("Hex: 0x%02x, C:%d, F:%d\n", res, res, fConvert1);
			printf("Hex: 0x%02x, C:%d, F:%d\n", res, res, fConvert1);
			printf("\n");
		}
	}
	close(file);
	
	exit(0);
}
