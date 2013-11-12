//Requirements------------------------------
var socket,
	firstconnect = true;
//---------------------------------------------------

function connect(){
	if(firstconnect){
		socket = io.connect(null);
		firstconnect = false;
	}else{
		socket.socket.reconnect();
	}
}


//Button 1, to play a song (testing)
function play(){
	socket.emit('playcurrentlist');
}


connect();

