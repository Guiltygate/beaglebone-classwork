sudo mount -t vfat -o rw /dev/sda1 /mnt/server_media

amixer -D default cset numid=5 30%

node umbreonServer.js
