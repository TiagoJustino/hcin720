var GITHUB_URL = 'https://api.github.com/events'
var X_START = 5;
var Y_START = 500;
var X_STEP = 10;
var Y_STEP = X_STEP;
var X_LENGTH = 500;
var Y_LENGTH = X_LENGTH;

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

// modified from http://stackoverflow.com/questions/1584370/how-to-merge-two-arrays-in-javascript-and-de-duplicate-items
var arrayUnique = function(array) {
  var a = array.concat();
  for(var i=0; i<a.length; ++i) {
    for(var j=i+1; j<a.length; ++j) {
      if(a[i].id == a[j].id)
        a.splice(j--, 1);
    }
  }
  return a;
};

var formatString = function() {
  var args = arguments;
  var base = args[0];
  return base.replace(/{(\d+)}/g, function(match, number) { 
    number = parseInt(number);
    return typeof args[number + 1] != 'undefined'
      ? args[number + 1]
      : match
      ;
  });
};

drawGraphAxis();

var collectedData = [];

var populateUsersList = function(data) {
  var compiledObj = {};
  var compiledData = [];
  for(var i = 0; i < data.length; i++) {
    var login = data[i].actor.login;
    var type = data[i].type.substr(0, data[i].type.indexOf('Event'))
    if(compiledObj[login]) {
      compiledObj[login].events += 1;
      compiledObj[login].lastEventType = type;
    } else {
      compiledObj[login] = {
        events: 1,
        lastEventType: type,
        login: login,
        avatar: data[i].actor.avatar_url
      };
    }
  }
  for(var o in compiledObj) {
    compiledData.push(compiledObj[o]);
  }
  var sortedData = compiledData.sort(function(a, b) {
    return a.events - b.events;
  });
  $('#userslist').empty();
  for(var i = 1; i <= 5; i++) {
    var user = sortedData[sortedData.length - i];
    if(user) {
      var str = '<li><img style="max-height: 100px; max-width: 100px" src="{0}"> {1} has {2} events, with the last being a {3}.</li>';
      var formatted = formatString(str, user.avatar, user.login, user.events, user.lastEventType)
      $('#userslist').append(formatted);
    }
  }
}

var populateEventsList = function(data) {
  var compiledObj = {};
  var compiledData = [];
  for(var i = 0; i < data.length; i++) {
    var type = data[i].type.substr(0, data[i].type.indexOf('Event'));
    if(compiledObj[type]) {
      compiledObj[type].n += 1;
    } else {
      compiledObj[type] = {type: type, n: 1};
    }
  }
  for(var o in compiledObj) {
    compiledData.push(compiledObj[o]);
  }
  var sortedData = compiledData.sort(function(a, b) {
    return a.n - b.n;
  });
  $('#eventslist').empty();
  for(var i = 1; i <= sortedData.length; i++) {
    var event = sortedData[sortedData.length - i];
    if(event) {
      var str = '<li>{0} -> {1}</li>';
      var formatted = formatString(str, event.type, event.n);
      $('#userslist').append(formatted);
    }
  }
}

var requestData = function() {
  $.getJSON( GITHUB_URL, onFetchedData);
}

var onFetchedData = function( data ) {
  collectedData = arrayUnique(collectedData.concat(data));
  populateUsersList(collectedData);
  populateEventsList(collectedData);
  setTimeout(requestData, 1000);
}

requestData();
