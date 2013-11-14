//New express framework server module

/*Dependencies
*/
var express = require('express'),
	app = express(),
	server = require('http').createServer(app),
	io = require('socket.io').listen(server),
	path = require('path'),
	url = require('url'),
	fs = require('fs'),
	connectCount = 0,
	currentstream;
	
var mountpath = '/mnt/server_media/';

var files = [],
	songs = [],
	songlist = [],
	vidoes = [];	

//============Audio dependencies======================
var lame = require('lame'),
	Speaker = require('speaker'),
	async = require('async');
 
//Decoder
var audioOptions = {channels: 2, bitDepth: 16, sampleRate: 44100};
var decoder = lame.Decoder();
//======================================================

function setPlaylist(){
	songs = [];
	files = fs.readdirSync(mountpath + '.');

	files.forEach(function(file){
		if(path.extname(file) === '.mp3'){
			songs.push(mountpath + file);
		}
	});	
}

function viewPlaylist(){
	songlist = [];
	songs.forEach(function(song){
		songlist.push(song);
	});
}

	
server.listen(8080);


//======App formalities & upload protocol=======================================
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');
app.use(express.logger('dev'));

app.use(express.static(__dirname + '/'));

app.use(express.bodyParser({ keepExtensions: true, uploadDir: "/mnt/server_media" }));

app.post("/upload", function (request, response) {
	 console.log("file path: ", request.files.file.path);
	 response.end("upload complete");
});


app.get('/', function(req, res){
	res.render('index', {title:'Home'});
	});

//==========================================================================


	
//==============Socket.io events==========================================	
io.set('log level', 2);

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
				console.log("Finished a song!");
				done();
			});
		});
	});
	
	socket.on('getCurrentList', function(){
		console.log("Received playlist call!");
		songlist = [];
		viewPlaylist();
		socket.emit('txCurrentList', songlist);
	});

    connectCount++;
    console.log("connectCount = " + connectCount);
});
//========================================================================


//========================Songlist Processing===============================

setPlaylist();

//==========================================================================
	
