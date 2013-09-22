#!/bin/sh

if [ $# -eq 0 ]
then
	echo "No gpio pins supplied; exiting."
	exit
fi	

AddrA=$1
AddrB=$2

echo Original Hex Temps are
temp1=`i2cget -y 1 0x${AddrA} 0`
temp2=`i2cget -y 1 0x${AddrB} 0`
echo $temp1
echo $temp2

echo Converted Temperatures Celsius
temp3=$(printf "%d\n" ${temp1})
temp4=$(printf "%d\n" ${temp2})
echo $temp3
echo $temp4

temp3=$(expr ${temp3} \* 9)
temp3=$(expr ${temp3} / 5)
temp3=$(expr ${temp3} + 32)

temp4=$(expr ${temp4} \* 9)
temp4=$(expr ${temp4} / 5)
temp4=$(expr ${temp4} + 32)

echo Converted Temperatures Farenheit ESTIMATE
echo $temp3
echo $temp4
