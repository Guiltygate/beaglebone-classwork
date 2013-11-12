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
	socket.emit('testIO');
}

//Button2, to play a predetermined song (testing purposes)


//Button 3, for downloading a file
function capture(){

	document.getElementById("test").innerHTML="New version.";


}

//Button 4, for testing random functions
function testingAround(){
	console.log("Met requirements!");
}

connect();

