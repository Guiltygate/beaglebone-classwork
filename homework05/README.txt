README for homework05 of ECE497.
This README is going to be much shorter, as not much original work was done.

1) Memory Map

2) gpio via mmap()
	1st part works as expected, very similar to the given exercise on eLinux.org
	for mmap(). It automatically uses gpio51 and gpio48, so the values cannot
	be altered by the user. Note that 51 goes to Vcc, 48 goes to ground.
	
	2nd part is a very minor alteration to gpioThru. Basically the only change is
	using the GPIO_1 address instead of GPIO_0, and changing GPIO3 to GPIO51.
	Again, works as expected.
