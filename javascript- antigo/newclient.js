//----------------------
var id = 0;
var isDraw = false;
var Img = 0;
var iea = [];
var hasInt = false;
var eea = [];
var hasExt = false;
var curCoord = [];
//----------------------

//----------------------
var socket = io.connect('192.168.0.100:3001'); 

socket.on('Welcome', function (data) {
	id = data;
	console.log('Welcome, dude!'); 
	//socket.emit('my other event', { my: 'data' }); 
	socket.send('Awww, yis!');
}); 

socket.on('Canvas', function(data){
	eea = data;
	hasExt = true;
	socket.emit("ack", id);
});

//----------------------

//----------------------

var canvas = document.getElementById('canv');
var frame = canvas.getContext('2d');

canvas.addEventListener('mousedown', function(evt) {
	isDraw = true;
}, true);

canvas.addEventListener('mouseup', function(evt) {
	isDraw = false;
	iea.push(0);
	socket.emit('stroke', [id, iea]);
}, true);

canvas.addEventListener('mousemove', function(evt) {
	var mouse = getMouse(canvas, evt);
	if (isDraw){
		drawBrush(mouse.x,mouse.y);
		iea.push([mouse.x,mouse.y]);
		hasEls = true;
	}
}, true);

function getMouse(canvas, evt) {
	var rect = canvas.getBoundingClientRect();
    return {
    	x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

function drawElements(els, internal){
	//console.log("Aiai");
	frame.beginPath();
	if (curCoord == []){
		var temp = els.shift(); 
		frame.moveTo(temp[0],temp[1]);
	}
	else frame.moveTo(curCoord[0],curCoord[1]);
	var coord = els.shift();
	if (coord != 0) frame.lineTo(coord[0],coord[1]);
	curCoord = coord;
	if (els == []) {
		if(internal) hasInt = false;
		else hasExt = false;
	}
	frame.stroke();
}

function update(){
	//console.log("update:");
	canvas.width = canvas.width; //to clear the canvas
	if (Img != 0) frame.putImageData(Img,0, 0);
	if (hasExt) drawElements(eea, false);
	if (hasInt) drawElements(iea, true);
	Img = frame.getImageData(0,0,canvas.width, canvas.height);
}

setInterval(update, 1000/60);
//----------------------