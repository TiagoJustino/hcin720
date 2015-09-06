var WEATHER_URL = 'http://api.openweathermap.org/data/2.5/weather?q=Rochester,NY';
var X_START = 5;
var Y_START = 100;
var X_STEP = 10;
var Y_STEP = X_STEP;
var X_LENGTH = 100;
var Y_LENGTH = X_LENGTH;

// got from http://www.proglogic.com/code/javascript/calculator/tempconverter.php
var kelvinToFahrenheit = function(kelvin) {
  return ((kelvin - 273.15)*9/5)+32;
}

var drawLine = function(start, length) {
  console.log('drawing line from', start, 'to', start + length);
  var path = new Path();
  path.strokeColor = 'black';
  path.moveTo(start);
  path.lineTo(start + length);
}


var drawPoint = function(point) {
  var shape = new Shape.Circle(point, 3);
  shape.strokeColor = 'black';
  shape.fillColor = 'black';
}

// got from http://stackoverflow.com/questions/847185/convert-a-unix-timestamp-to-time-in-javascript
var formateDate = function(timestamp) {
  var date = new Date(timestamp*1000);
  var year = date.getFullYear();
  var month = ('0' + date.getMonth()).substr(-2);
  var day = ('0' + date.getDate()).substr(-2);
  var hours = date.getHours();
  var minutes = ('0' + date.getMinutes()).substr(-2);
  var seconds = ('0' + date.getSeconds()).substr(-2);

  var formattedTime = year + '-' + month + '-' + day + ' ' + hours + ':' + minutes + ':' + seconds;

  return formattedTime;
}

var drawGraphAxis = function() {
  drawLine(new Point(0, Y_START), new Size(X_LENGTH + X_STEP, 0));
  drawLine(new Point(X_STEP, 0), new Size(0, Y_LENGTH + Y_STEP));
}

drawGraphAxis();

$.getJSON( WEATHER_URL, function( data ) {
  var n = 1;
  var temp = kelvinToFahrenheit(data.main.temp);
  console.log('temp =', temp);
  console.log('time =', formateDate(data.dt));
  drawPoint(new Point(X_START + n * X_STEP, Y_START - temp));
});
