//Requirements------------------------------
var socket,
	firstconnect = true,
	size,
	listTable,
	name;
//---------------------------------------------------

function connect(){
	if(firstconnect){
		socket = io.connect(null);
		
		socket.on('txCurrentList', function(playlist){
		
			size = playlist.length;
			
			listTable = "<table id='currentlist'><tr><th>Current Playlist</th></tr>";	
			for(var i=0; i<size; i++){
				listTable += '<tr><td><div onClick="removeSong('+i+')">';
				listTable += playlist[i];
				listTable += '</div></td></tr>';		
			}			
			listTable+="</table>";
			$('#currentlist').replaceWith(listTable)
		
		});
		
		socket.on('directoryList', function(mp3list){
			size = mp3list.length;
			
			listTable = "<table id='currentlist'><tr><th>Current MP3 Files in Directory</th></tr>";	
			for(var i=0; i<size; i++){
				listTable += '<tr><td><div onClick="addSong('+i+')">';
				listTable += mp3list[i];
				listTable += '</div></td></tr>';		
			}			
			listTable+="</table>";
			$('#currentlist').replaceWith(listTable)
		});
		
		socket.on('currentTrack', function(currentTrack){
			$('#currentTrack').replaceWith("<h2 id='currentTrack' align='right'>"+currentTrack+"</h2>")
		});
		
		
		firstconnect = false;
	}else{
		socket.socket.reconnect();
	}
}


//Button 1, to play a song (testing)
function play(){
	socket.emit('playCurrentList');
}

function getPlayList(){
	socket.emit('getCurrentList');	
}

function getDirectory(){
	socket.emit('getDirectory');
}

function loadDrive(){
	socket.emit('loadDrive');
}

function skipForward(){
	socket.emit('skipForward');
}

function skipBack(){
	socket.emit('skipBack');
}

function turnUp(){
	socket.emit('turnUp');
}

function turnDown(){
	socket.emit('turnDown');
}

function stopPlaylist(){
	socket.emit('stopPlaylist');
}

function removeSong(songNum){
	socket.emit('removeSong', songNum);
}

function addSong(songNum){
	socket.emit('addSong', songNum);
}

function giveName(){
	socket.emit('giveName', name);
}

connect();



