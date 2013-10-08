README for homework05 of ECE497.
This README is going to be much shorter, as not much original work was done.

1) Memory Map

	0 |---------|FFFF_FFFF - Reserved block (1GB)
	2 |		|
	3 |		|E
	4 |		|
	5 |		|D
	6 |		|
	7 |		|C
	8 |		|
	9 |---------|BFFF_FFFF - EMIF0 SDRAM (8/16 bit External Memory) (1GB)
	10|		|
	11|		|A
	12|		|
	13|		|9
	14|		|
	15|		|8
	16|		|
	17|---------|7FFF_FFFF - Reserved block (512MB)
	18|		|
	19|		|6
	20|		|
	21|---------|5FFF_FFFF - More Mixed, mostly reserved space
	22|		|
	23|---------|4FFF_FFFF - Mixed, USB registers, Reserved Space, Other
	24|		|
	25|---------|3FFF_FFFF - Reserved block (512MB)
	26|		|
	27|		|
	28|		|
	29|---------|1FFF_FFFF - GPMC (External memory) End (512MB)
	30|		|
	  |		|
	  |		|
	31|---------|0000_0000
	
	
	GPIO_0 - 44E0_7000 Start / 44E0_9000 End
	GPIO_1 - 4804_c000 Start / 4804_DFFF End
	GPIO_2 - 481A_C000 Start / 481A_DFFF End
	GPIO_3 - 481A_E000 Start / 481A_FFFF End

2) gpio via mmap()
	1st part works as expected, very similar to the given exercise on eLinux.org
	for mmap(). It automatically uses gpio51 and gpio48, so the values cannot
	be altered by the user. Note that 51 goes to Vcc, 48 goes to ground.
	
	2nd part is a very minor alteration to gpioThru. Basically the only change is
	using the GPIO_1 address instead of GPIO_0, and changing GPIO3 to GPIO51.
	Again, works as expected.
