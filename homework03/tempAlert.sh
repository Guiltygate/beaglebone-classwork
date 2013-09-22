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

cleanup(){
echo $gpio > /sys/class/gpio/unexport
exit
}	

echo $gpio > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio${gpio}/direction

trap cleanup SIGINT

i2cset -y 1 0x${AddrA} 2 0x${TempB}
i2cset -y 1 0x${AddrA} 3 0x${TempA}

i2cset -y 1 0x${AddrB} 2 0x${TempB}
i2cset -y 1 0x${AddrB} 3 0x${TempA}

while [ "1" = "1" ];
do
	if [ "0" = `cat /sys/class/gpio/gpio${gpio}/value` ];
	then
		echo Temperature
		#Initial Temperatures
		temp1=`i2cget -y 1 0x${AddrA} 0`
		temp2=`i2cget -y 1 0x${AddrB} 0`
		#Translate to decimal Celsius
		temp1=$(printf "%d\n" ${temp1})
		temp2=$(printf "%d\n" ${temp2})
		#Convert to Farenheit
		temp1=$(expr ${temp1} \* 9)
		temp1=$(expr ${temp1} / 5)
		temp1=$(expr ${temp1} + 32)
		temp2=$(expr ${temp2} \* 9)
		temp2=$(expr ${temp2} / 5)
		temp2=$(expr ${temp2} + 32)
		#print
		echo $temp1
		echo $temp2
	fi


done
