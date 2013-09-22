#!/bin/sh
#Form for calling: ./tempAlert Address1 Address2 TemperatureHigh TemperatureLow AlertGPIO

#Use GPIO51

if [ $# -eq 0 ]
then
	echo "No addresses or temperatures supplied; exiting."
	exit
fi

AddrA=$1
AddrB=$2
TempA=$3
TempB=$4
gpio=$5

echo $gpio > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio${gpio}/direction

i2cset -y 1 0x${AddrA} 2 0x${TempB}
i2cset -y 1 0x${AddrA} 3 0x${TempA}

i2cset -y 1 0x${AddrB} 2 0x${TempB}
i2cset -y 1 0x${AddrB} 3 0x${TempA}
