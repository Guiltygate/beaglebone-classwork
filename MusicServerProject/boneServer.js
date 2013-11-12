#!/usr/bin/node
"use strict";

var port = 8080, // Port to listen on
    http = require('http'),
    url = require('url'),
    fs = require('fs'),
    child_process = require('child_process'),
    server,
    connectCount = 0,	// Number of connections to server
    errCount = 0;	// Counts the AIN errors.

	var lame = require('lame'),
		Speaker = require('speaker');
 
//Encoder and Decoder
var encoder = lame.Encoder({channels: 2, bitDepth: 16, sampleRate: 44100});
encoder.on("data", function(data) {sendData(data);});

var decoder = lame.Decoder();
decoder.on('format', function(format) {decoder.pipe(encoder);});

		


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

// socket.io, I choose you
var io = require('socket.io').listen(server);
io.set('log level', 2);

// on a 'connection' event
io.sockets.on('connection', function (socket) {

    console.log("Connection " + socket.id + " accepted.");
//    console.log("socket: " + socket);

    // now that we have our connected 'socket' object, we can 
    // define its event handlers

    socket.on('testIO', function(){
		console.log("Worked!");
	});

    connectCount++;
    console.log("connectCount = " + connectCount);
});

