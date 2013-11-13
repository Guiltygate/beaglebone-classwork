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
	connectCount = 0;
	
	
//============Audio dependencies======================
var lame = require('lame'),
	Speaker = require('speaker'),
	async = require('async');
 
//Decoder
var audioOptions = {channels: 2, bitDepth: 16, sampleRate: 44100};
var decoder = lame.Decoder();
//======================================================

	
server.listen(8080);


//======App formalities & upload protocol=======================================
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');
app.use(express.logger('dev'));

app.use(express.bodyParser({keepExtensions:true, uploadDir: 'uploads' }));
app.use(express.static(__dirname + '/'));
app.use(express.limit('5mb'));

app.get('/', function(req, res){
	res.render('index', {title:'Home'});
	});
	
app.get('/upload', function(req, res){
  res.send('<form method="post" enctype="multipart/form-data">'
    + '<p>Image: <input type="file" name="image" /></p>'
    + '<p><input type="submit" value="Upload" /></p>'
    + '</form>');
});

app.post('/', function(req, res, next){

  // connect-form adds the req.form object
  // we can (optionally) define onComplete, passing
  // the exception (if any) fields parsed, and files parsed
  req.form.complete(function(err, fields, files){
    if (err) {
      next(err);
    } else {
      console.log('\nuploaded %s to %s'
        ,  files.image.filename
        , files.image.path);
      res.redirect('back');
    }
  });
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
				console.log("Finished a file!");
				done();
			});
		});
	});
	
	socket.on('getCurrentList', function(){console.log("Received playlist call!"); socket.emit('txCurrentList', songlist);});

    connectCount++;
    console.log("connectCount = " + connectCount);
});
//========================================================================


//========================Songlist Processing===============================
var mountpath = '/mnt/server_media/';
var files = fs.readdirSync(mountpath + '.'),
	songs = [],
	songlist = [];


files.forEach(function(file){
	if(path.extname(file) === '.mp3'){songs.push(mountpath + file); songlist.push(file);}
});	

//==========================================================================
	
