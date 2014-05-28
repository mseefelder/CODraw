//Code to manage our canvas.
/*
	For now it draws many circles while the LMB is pressed.
	BUG: when the mouse goes off the canvas, isDraw should 
turn false.

	Planned workflow:
	For every update:
		clear canvas
		draw local image
		draw external shapes
		update local image
		draw internal shapes
*/

//socket initialization:
var socket = io.connect('http://localhost:3001'); 

socket.on('Welcome', function () { 
	console.log('Welcome, dude!'); 
	//socket.emit('my other event', { my: 'data' }); 
	socket.send('Awww, yis!');
}); 

var canvas = document.getElementById('canv');
var frame = canvas.getContext('2d');
var isDraw = false;
var bSize = 1;
var localImage = 0;
var elArray = [];
var curCoord = [];
var hasEls = false;

function drawSquare(mx,my){
	frame.fillRect(mx,my,80,80);
}

function drawBrush(mx, my){
	frame.beginPath();
	frame.arc(mx, my, bSize, 0, 2*Math.PI, true);
	frame.fill();
}

canvas.addEventListener('mousedown', function(evt) {
	isDraw = true;
}, true);

canvas.addEventListener('mouseup', function(evt) {
	isDraw = false;
	elArray.push(0);
}, true);

canvas.addEventListener('mousemove', function(evt) {
	var mouse = getMouse(canvas, evt);
	if (isDraw){
		drawBrush(mouse.x,mouse.y);
		elArray.push([mouse.x,mouse.y]);
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

//tentativas

function getMouseF(canvas) {
	var rect = canvas.getBoundingClientRect();
    return {
    	x: frame.clientX - rect.left,
        y: frame.clientY - rect.top
    };
}

function drawElements(els){
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
	if (els == []) hasEls = false;
	frame.stroke();
}

function update(){
	//console.log("update:");
	//canvas.width = canvas.width; //to clear the canvas
	if (localImage) frame.putImageData(localImage,0, 0);
	if (hasEls) drawElements(elArray);
	localImage = frame.getImageData(0,0,canvas.width, canvas.height);
}

function mainloop(){
	var mouse = getMouseF(canvas);
	if (isDraw) drawBrush(mouse.x,mouse.y);
}

setInterval(update, 1);


/*EXTRAS:
Super good tutorial:
diveintohtml5.info/canvas.html
Image Data:
http://www.w3schools.com/tags/canvas_putimagedata.asp
*/