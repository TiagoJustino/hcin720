var GITHUB_URL = 'https://api.github.com/events';
var receivedEvents = {};
var usersData = [];
var eventsData = [];
var collectedData = [];
var accumulatedData = {};
var now;
var startTime;

// modified from http://stackoverflow.com/questions/1584370/how-to-merge-two-arrays-in-javascript-and-de-duplicate-items
var arrayUnique = function (array) {
  var i;
  var j;
  var a = array.concat();
  for (i=0; i<a.length; ++i) {
    for (j=i+1; j<a.length; ++j) {
      if (a[i].id == a[j].id) {
        a.splice(j--, 1);
      }
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

var populateUsersList = function(data) {
  var compiledObj = {};
  var compiledData = [];
  for(var i = 0; i < data.length; i++) {
    var login = data[i].actor.login;
    var type = data[i].type.substr(0, data[i].type.indexOf('Event'))
    if(compiledObj[login]) {
      compiledObj[login].n += 1;
      compiledObj[login].lastEventType = type;
    } else {
      compiledObj[login] = {
        n: 1,
        lastEventType: type,
        login: login,
        avatar: data[i].actor.avatar_url
      };
    }
  }
  for(var o in compiledObj) {
    compiledData.push(compiledObj[o]);
  }
  usersData = compiledData.sort(function(a, b) {
    return a.n - b.n;
  });
  $('#userslist').empty();
  for(var i = 1; i <= 5; i++) {
    var user = usersData[usersData.length - i];
    if(user) {
      var str = '<li><img style="max-height: 100px; max-width: 100px" src="{0}"> {1} has {2} events, with the last being a {3}.</li>';
      var formatted = formatString(str, user.avatar, user.login, user.n, user.lastEventType)
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
  eventsData = compiledData.sort(function(a, b) {
    return a.n - b.n;
  });
  $('#eventslist').empty();
  for(var i = 1; i <= eventsData.length; i++) {
    var event = eventsData[eventsData.length - i];
    if(event) {
      var str = '<li>{0} -> {1}</li>';
      var formatted = formatString(str, event.type, event.n);
      $('#eventslist').append(formatted);
    }
  }
}

var requestData = function() {
  $.getJSON( GITHUB_URL, onFetchedData);
}

var updateAccumulatedData = function() {
  for(i = 0; i < usersData.length; i++) {
    if(!accumulatedData[usersData[i].login]) {
      accumulatedData[usersData[i].login] = [];
    }
    accumulatedData[usersData[i].login].push({
      time: now,
      value: usersData[i].n
    })
  }

  for(i = 0; i < eventsData.length; i++) {
    if(!accumulatedData[eventsData[i].type]) {
      accumulatedData[eventsData[i].type] = [];
    }
    accumulatedData[eventsData[i].type].push({
      time: now,
      value: eventsData[i].n
    })
  }
}

var plotData = function(label, data) {
  console.log(label, data);
}

var updateGraph = function() {
  updateAccumulatedData();
  if (usersData) {
    for(var i = 1; i <= 2; i++) {
      var user = usersData[usersData.length - i];
      if(user) {
        plotData(user.login, accumulatedData[user.login]);
      }
    }
  }

  if (eventsData) {
    for(var i = 1; i <= 2; i++) {
      var event = eventsData[eventsData.length - i];
      if(event) {
        plotData(event.type, accumulatedData[event.type]);
      }
    }
  }
}

var onFetchedData = function( data ) {
  for(var i = 0; i < data.length; i++) {
    if(receivedEvents[data[i].id]) {
      data.splice(i, 1);
      i--;
    } else {
      receivedEvents[data[i].id] = true;
      data[i].now = now;
    }
  }
  if(data.length > 0) {
    now = getSeconds();
    collectedData = collectedData.concat(data);
    populateUsersList(collectedData);
    populateEventsList(collectedData);
    updateGraph();
  }
  setTimeout(requestData, 1000);
}

var getSeconds = function() {
  return new Date().getTime() / 1000;
}

startTime = getSeconds();
requestData();
