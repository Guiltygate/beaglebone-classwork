#!/bin/bash
# Set up gpio 51 and 48 to write
cd /sys/class/gpio
echo 51 > export
echo 48 > export
echo in  > gpio51/direction
echo in > gpio48/direction
