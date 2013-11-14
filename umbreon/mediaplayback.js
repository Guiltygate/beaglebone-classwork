//Requirements------------------------------
var socket,
	firstconnect = true,
	size,
	listTable;
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

function removeSong(songNum){
	socket.emit('removeSong', songNum);
}

function addSong(songNum){
	socket.emit('addSong', songNum);
}

connect();



