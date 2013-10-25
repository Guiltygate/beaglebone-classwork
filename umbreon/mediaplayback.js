//Requirements------------------------------
var socket,
	firstconnect = true,
	size,
	listTable = "<tr><th>Current Playlist</th></tr>";
//---------------------------------------------------

function connect(){
	if(firstconnect){
		socket = io.connect(null);
		
		socket.on('txCurrentList', function(playList){
		
			size = playList.length;
	
			for(var i=0; i<size; i++){
				listTable += '<tr><td>';
				listTable += playList[i];
				listTable += '</td></tr>';		
			}

			$('#playList').append(listTable)
		
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



connect();



