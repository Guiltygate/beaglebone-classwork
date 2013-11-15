<h1>Umbreon Media Controller</h1>

<p>
Simple nodejs express web server that allows devices on the server's network
to play mp3 files through the server device (my case case, a beaglebone
hdmi port).</p>

Invoking with

 ./startServer.sh
 
will automatically mount whatever usb device is present by running

 mount -t vfat -o rm /dev/sda1 /mnt/server_media

Invoking with 'node umbreonServer.js' will just start the server, without
mounting server_media.

<b>NOTE</b>: You will need to create the folder chain '/mnt/server_media' for proper
reading, uploading, and downloading of files. Every mp3 file processed by the server
comes from that location, so create that for proper drive mounting or change
the umbreonServer.js file.
