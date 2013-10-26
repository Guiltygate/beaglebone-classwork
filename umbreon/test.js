var speaker = require('speaker'),
	fs = require('fs'),
	lame = require('lame'),
	async = require('async');

var audioOptions = {channels: 2, bitDepth: 16, sampleRate:44100},
	decoder = lame.Decoder();

var song = 'wwFile.mp3';
console.log(song);

var speakerOut = new speaker(audioOptions);
var inputStream = fs.createReadStream(song);
inputStream.pipe(decoder).pipe(speakerOut);
