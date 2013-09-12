#!/bin/sh

#15/48, 17/87, 23/49, 27/115 are outputs
# 16/51, 18/4, 24/15, 28/113 are inputs

if [ $# -eq 0 ]
then
	echo "No gpio pins supplied; exiting."
	exit
fi	

A=$1
B=$2
C=$3
D=$4



cleanup(){
echo $A > /sys/class/gpio/unexport
echo $B > /sys/class/gpio/unexport
echo $C > /sys/class/gpio/unexport
echo $D > /sys/class/gpio/unexport
exit
}

#gpio setup
echo $A > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio${A}/direction

echo $B > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio${B}/direction

echo $C > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio${C}/direction

echo $D > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio${D}/direction


trap cleanup SIGINT

VALUE_1A=`cat /sys/class/gpio/gpio${A}/value`
VALUE_1B=`cat /sys/class/leds/beaglebone:green:usr0/brightness`

VALUE_2A=`cat /sys/class/gpio/gpio${B}/value`
VALUE_2B=`cat /sys/class/leds/beaglebone:green:usr1/brightness`

VALUE_3A=`cat /sys/class/gpio/gpio${C}/value`
VALUE_3B=`cat /sys/class/leds/beaglebone:green:usr2/brightness`

VALUE_4A=`cat /sys/class/gpio/gpio${D}/value`
VALUE_4B=`cat /sys/class/leds/beaglebone:green:usr3/brightness`

echo $VALUE_1A
echo $VALUE_1B
echo $VALUE_2A
echo $VALUE_2B
echo $VALUE_3A
echo $VALUE_3B
echo $VALUE_4A
echo $VALUE_4B

#read toggles
while [ "1" = "1" ];
do
	if [ "$VALUE_1A" != `cat /sys/class/gpio/gpio${A}/value` ];
	then
		if [ "$VALUE_1B" != "1" ];
		then
			echo 1 > /sys/class/leds/beaglebone:green:usr0/brightness
			VALUE_1B=1
		else
			echo 0 > /sys/class/leds/beaglebone:green:usr0/brightness
			VALUE_1B=0
		fi
		VALUE_1A=`cat /sys/class/gpio/gpio${A}/value`
	fi

	if [ "$VALUE_2A" != `cat /sys/class/gpio/gpio${B}/value` ];
	then
		if [ "$VALUE_2B" != "1" ];
		then
			echo 1 > /sys/class/leds/beaglebone:green:usr1/brightness
			VALUE_2B=1
		else
			echo 0 > /sys/class/leds/beaglebone:green:usr1/brightness
			VALUE_2B=0
		fi
		VALUE_2A=`cat /sys/class/gpio/gpio${B}/value`
	fi

	if [ "$VALUE_3A" != `cat /sys/class/gpio/gpio${C}/value` ];
	then
		if [ "$VALUE_3B" != "1" ];
		then
			echo 1 > /sys/class/leds/beaglebone:green:usr2/brightness
			VALUE_3B=1
		else
			echo 0 > /sys/class/leds/beaglebone:green:usr2/brightness
			VALUE_3B=0
		fi
		VALUE_3A=`cat /sys/class/gpio/gpio${C}/value`
	fi	

	if [ "$VALUE_4A" != `cat /sys/class/gpio/gpio${D}/value` ];
	then
		if [ "$VALUE_4B" != "1" ];
		then
			echo 1 > /sys/class/leds/beaglebone:green:usr3/brightness
			VALUE_4B=1
		else
			echo 0 > /sys/class/leds/beaglebone:green:usr3/brightness
			VALUE_4B=0
		fi
		VALUE_4A=`cat /sys/class/gpio/gpio${D}/value`
	fi




done
