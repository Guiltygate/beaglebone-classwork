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


//socket.io events
var io = require('socket.io').listen(server);
io.set('log level', 2);

//on 'finish' and 'flush' receive "Error: write after end." comes from events.js:69

// on a 'connection' event
io.sockets.on('connection', function (socket) {

    console.log("Connection " + socket.id + " accepted.");

	//Big thanks to TooTallNate of the nodejs community, your modules are pretty badass.
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


//THE FOLLOWING CODE IS NOT MY OWN
//It is used to handle uploaded files
//Link here: http://stackoverflow.com/questions/16534892/multipart-file-uploads-using-nodejs

var express = require("express"),
	app = express();
	
// tell express to use the bodyParser middleware and set upload directory
app.use(express.bodyParser({ keepExtensions: true, uploadDir: "/mnt/server_media" }));
app.engine('jade', require('jade').__express);

app.post("/upload", function (request, response) {
	// request.files will contain the uploaded file(s),
	// keyed by the input name (in this case, "file")
	
	// show the uploaded file name   
	console.log("file name", request.files.file.name);
	console.log("file path", request.files.file.path);
	
	response.end("upload complete"); 
});
	
// render file upload form 
app.get("/", function (request, response) {
	response.render("boneServer.html");
});
	
app.listen(8080);	
	
	
	

