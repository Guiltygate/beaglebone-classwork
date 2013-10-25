#!/usr/bin/node
"use strict";

var port = 8080, // Port to listen on
    http = require('http'),
    path = require('path'),
    url = require('url'),
    fs = require('fs'),
    child_process = require('child_process'),
    server,
    connectCount = 0,	// Number of connections to server
    errCount = 0;	// Counts the AIN errors.

	var lame = require('lame'),
		Speaker = require('speaker'),
		async = require('async');
 
//Decoder
var audioOptions = {channels: 2, bitDepth: 16, sampleRate: 44100};
var decoder = lame.Decoder();


//Read in preset directory for processing of .mp3's
var mountpath = '/mnt/server_media/';
var files = fs.readdirSync(mountpath + '.'),
	songs = [],
	songlist = [];

console.log(files);

files.forEach(function(file){
	if(path.extname(file) === '.mp3'){songs.push(mountpath + file); songlist.push(file);}
});	

console.log(songs);


function send404(res) {
    res.writeHead(404);
    res.write('404');
    res.end();
}


server = http.createServer(function (req, res) {
// server code
    var path = url.parse(req.url).pathname;
    console.log("path: " + path);
    if (path === '/') {
        path = '/boneServer.html';
    }

    fs.readFile(__dirname + path, function (err, data) {
        if (err) {return send404(res); }
//            console.log("path2: " + path);
        res.write(data, 'utf8');
        res.end();
    });
});


server.listen(port);
console.log("Listening on " + port);
/**
var speaker1 = new Speaker(audioOptions);
var speaker2 = new Speaker(audioOptions);

var stream1 = fs.createReadStream(songs[3]); //loads vampires
var stream2 = fs.createReadStream(songs[0]); //loads shoes

stream1.pipe(decoder).pipe(speaker1); //streams vampires
**/
//socket.io events
var io = require('socket.io').listen(server);
io.set('log level', 2);

//on 'finish' and 'flush' receive "Error: write after end." comes from events.js:69

// on a 'connection' event
io.sockets.on('connection', function (socket) {

    console.log("Connection " + socket.id + " accepted.");


    socket.on('playCurrentList', function(){
		console.log("Playing...");
		async.eachSeries(songs, function(song, done){
			var speaker = new Speaker(audioOptions);
			var inputStream = fs.createReadStream(song);
			var decoderNew = lame.Decoder();
			inputStream.pipe(decoderNew).pipe(speaker);
			
			speaker.on('close', function(){
				console.log("Finished a file!");
				done();
			});
		});
	});
	
	socket.on('getCurrentList', function(){console.log("Received playlist call!"); socket.emit('txCurrentList', songlist);});

    connectCount++;
    console.log("connectCount = " + connectCount);
});

