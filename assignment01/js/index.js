/***********************************************************************
 * Paper example adapted by TiagoJustino from                          *
 * http://paperjs.org/tutorials/getting-started/working-with-paper-js/ *
 ***********************************************************************/

var draw = function(start, offset) {
  var path = new Path();
  path.strokeColor = 'black';
  var startPoint = new Point(start);
  path.moveTo(startPoint);
  path.lineTo(startPoint + offset);
}

// draw a T
draw([  0,  50], [100, -50]);
draw([ 50,  25], [  0, 100]);

// draw a J
draw([150,  50], [100, -50]);
draw([200,  25], [  0, 100]);
draw([200, 125], [-50,   0]);
draw([150, 125], [  0, -25]);
