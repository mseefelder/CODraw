//Code to manage our canvas.
/*
	For now it draws many circles while the LMB is pressed.
	BUG: when the mouse goes off the canvas, isDraw should 
turn false.
*/

var canvas = document.getElementById('canv');
var frame = canvas.getContext('2d');
var isDraw = false;
var bSize = 1;

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
}, true);

canvas.addEventListener('mousemove', function(evt) {
	var mouse = getMouse(canvas, evt);
	if (isDraw) drawBrush(mouse.x,mouse.y);
}, true);

function getMouse(canvas, evt) {
	var rect = canvas.getBoundingClientRect();
    return {
    	x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

//setInterval(mainloop, 1000/60);
