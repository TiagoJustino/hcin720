$(document).ready(function() {
  var socket = io();
  socket.on('to browser', function(data) {
    $('#messages').append($('<li>').text(data));
  });

  var sendWave = function() {
    console.log("sending servo command");
    socket.emit('to serial', "servo");
    socket.emit('to serial', $( "#height" ).val() );
  }

  $( "#in" ).click(function() {
    console.log("sending forward command");
    socket.emit('to serial', "forward");
    socket.emit('to serial', $( "#amount" ).val() );
  });

  $( "#out" ).click(function() {
    console.log("sending backward command");
    socket.emit('to serial', "backward");
    socket.emit('to serial', $( "#amount" ).val() );
  });

  $( "#wave" ).click(function() {
    sendWave();
  });

  $('#height').keyup(function(e){
    if(e.keyCode == 13)
    {
        sendWave();
    }
});
});
