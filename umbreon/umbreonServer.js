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
	child_process = require('child_process'),
	halt = false,
	firstplay = true;
	i=0,
	volume = 30,
	name = 'bugger',
	playing = false;
	
	
var mountpath = '/mnt/server_media/';

var dirfiles = [],
	playlist = [],
	songlist = [],
	setlist = [],
	mp3files,
	vidoes = [];	

//============Audio dependencies======================
var lame = require('lame'),
	Speaker = require('speaker'),
	async = require('async');
 
//Decoder
var audioOptions = {channels: 2, bitDepth: 16, sampleRate: 44100};
var decoder = lame.Decoder();
var speaker = new Speaker(audioOptions);
//======================================================

//Read in all directory files, and put only .mp3s into mp3list
function readDirectory(){
	mp3list = [];
	dirfiles = fs.readdirSync(mountpath + '.');

	dirfiles.forEach(function(file){
		var ex = path.extname(file);
		if(ex === '.mp3'){
			mp3list.push(file);
		}
	});	
}

//set the song list that's actually played
function setPlaylist(){
	playlist = [],
	setlist.forEach(function(song){
		playlist.push(mountpath + song);
	});
}

//Only called once, to set initial setlist. All further changes come from add/removeSong
function setList(){
	setlist = [];
	mp3list.forEach(function(song){
		setlist.push(song);
	});
}

//Stop current stream from playing
function stop(){
	halt = true;
	currentStream.end();
	speaker.end();
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
	 request.files.file.name = name;
	 response.end("Upload Complete.");
});

app.get("/download", function(req, res) {
	 res.download(playlist[i]);
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
		setPlaylist();
		if(playlist.length === 0 || playing){
    		return;
    	}
    		playing = true;
		
		console.log("Beginning playback...");
		halt = false;
		i=-1;
		
		async.eachSeries(playlist, function(song, done){
			if(!halt){
				i++;
				speaker = new Speaker(audioOptions);
				currentStream = fs.createReadStream(song).pipe(new lame.Decoder).pipe(speaker);
				socket.emit('currentTrack', path.basename(playlist[i]));
				speaker.on('close', function(){
					done();
				});
			}
		});
		playing = false;
	});
	
	socket.on('stopPlaylist', function(){
		console.log("Stopping playback...");
		stop();
	});
	
	socket.on('getCurrentList', function(){
		console.log("Received playlist call!");
		socket.emit('txCurrentList', setlist);
	});
	
	socket.on('getDirectory', function(){
		readDirectory();
		
		socket.emit('directoryList', mp3list);
		
	});
	
	socket.on('removeSong', function(songNum){
		console.log(songNum);
		setlist.splice(songNum, 1);
		socket.emit('txCurrentList', setlist);
	});
	
	socket.on('addSong', function(songNum){
		setlist.push(mp3list[songNum]);
		socket.emit('directoryList', mp3list);
	});
	
	socket.on('skipForward', function(){
		currentStream.end();
		speaker.end();
		console.log("Yeees?...");
	});
	
	socket.on('skipBack', function(){
		if(i > 0){
			i--;
		}
	});
	
	socket.on('turnUp', function(){
		volume+=5;
		console.log(volume);
		child_process.exec('amixer -D default cset numid=5 '+volume+'%');
	});
	
	socket.on('turnDown', function(){
		volume-=5;
		console.log(volume);
		child_process.exec('amixer -D default cset numid=5 '+volume+'%');
	});
	
	socket.on('loadDrive', function(){
	console.log("Loading new drive...");
		child_process.exec('mount -t vfat -o rw /dev/sda1 /mnt/server_media');
		mp3list = [],
		setlist = [];
	});
	
	socket.on('giveName', function(newName){
		name = newName;
	});

    connectCount++;
    console.log("connectCount = " + connectCount);
});
//========================================================================


//========================Songlist Processing===============================

readDirectory();
setList();
setPlaylist();
var currentStream;




//==========================================================================
	
