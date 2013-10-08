#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <signal.h>
#include "address_defines.h"
#include "gpio.h"

#define GPIO1_START_ADDR 0x4804C000
#define GPIO1_END_ADDR   0x4804e000
#define GPIO1_SIZE (GPIO1_END_ADDR - GPIO1_START_ADDR)

#define GPIO_SETDATAOUT 0x194
#define GPIO_CLEARDATAOUT 0x190
#define USR3 (1<<24)

int main(int argc, int argv[]){

//Setup variables
int valueA, valueB;
volatile void *gpio_addr;
volatile unsigned int *gpio_setdataout_addr;
volatile unsigned int *gpio_cleardataout_addr;

//Setup gpio's via bash script
system("./setup.sh");

//Open file, setup initial addresses
int fd = open("/dev/mem", O_RDWR);
gpio_addr = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_START_ADDR);

//Calculate base addresses for read/write
gpio_setdataout_addr   = gpio_addr + GPIO_SETDATAOUT;
gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

//read/write to register based on toggles
while(1){
	gpio_get_value(51, &valueA);
	gpio_get_value(48, &valueB);
	if(valueA == 1){
		*gpio_setdataout_addr = USR3;
	}else if(valueB == 0){
		*gpio_cleardataout_addr = USR3;
	}
}


return 0;
}
