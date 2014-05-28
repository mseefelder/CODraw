/*
var io = require('socket.io').listen(3001); 
var express = require('express'); 
var app = express();

io.sockets.on('connection', function (socket) { 
  socket.emit('news', { hello: 'world' }); 

  socket.on('my other event', function (data) { 
    console.log(data); 
  });
});

app.get('/', function(req, res){
  res.sendfile("client.html");
 });

app.listen(8080);
*/

/*
var io = require('socket.io').listen(8000);

io.sockets.on('connection', function (socket) {
  socket.on('message', function () { });
  socket.on('disconnect', function () { });
});
*/

var io = require('socket.io').listen(3001); 
console.log("Bud");
var express = require('express'); 
var app = express();

var id = 0;
counter = 0;
var artists = [];
var canvas = [];

//------------------------------------------------

function artist(index){
  this.id = index;
  this.buffer = [];
  this.isempty = true;
};

//------------------------------------------------

io.sockets.on('connection', function (socket) {
  
  console.log('Server up!');
  socket.emit('Welcome', id);
  artists.push(new artist(id));
  id++;

  socket.on('message', function (message) {
    console.log(message);
  });

  socket.on('stroke', function(data){
    artists[data[0]].buffer.push(data[1]);
    socket.broadcast.emit(data[1]);  
  });
});

app.get('/', function(req, res){
  res.sendfile("index-novo.html");
  //res.sendfile("./socket.io/socket.io.js");
 });
/*
function update(){
  if (counter == id){ 
    socket.emit('Canvas', canvas);
    counter = 0;
  }
  if (!artists[id].isempty) canvas.push(artists[id].buffer.shift());
  counter ++;
}

setInterval(update, 1);
*/
app.listen(8080);