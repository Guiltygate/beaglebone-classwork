
Author: Eric Ames. Written for ECE497, Homework01.
NOTE: testScriptR.sh and etchsketch.c are my own work, but gpio-util.c and gpio-util.h ARE NOT. Please see https://www.ridgerun.com/developer/wiki/index.php/Gpio-int-test.c for the original source material.

TOGGLE LED
testScriptR.sh is a simple script controlling the four onboard LEDs via toggle switchs. You must specify the four gpios to use as arguments, otherwise the program will close.

General Form: ./testScriptR.sh <toggle_1> <toggle_2> <toggle_3> <toggle_4>

ETCHSKETCH
etchsketch is defined in the file, but it basically takes five toggle switches as input and moves a cursor the desired direction (the fifth is to clear the board). Note that 1 button press creates to x's- that's not a mistake. I wanted a thicker line, and it worked well with the code
fortunately. 

General Form: ./etchsketch <board_size> <left_toggle> <right_toggle> <up_toggle> <down_toggle> <clear_board>

THERE IS CURRENTLY NO ERROR HANDLING FOR ETCHSKETCH. If you fail to pass in arguments, it WILL SegFault. If I have time, I'd like to fix that.
