# hcin720 - Assignment 2

This assignment illustrates how to use photon with external components for
reading data (sensors) and generating output for the world (actuators). It also
demonstrate how to communicate with photon both via serial connection and
cloud.

## Material

It follows the list of components used for this assignment:

* [Particle Photon](https://store.particle.io/?product=particle-photon)
* [Photoresistor](https://www.sparkfun.com/products/9088)
* [Accelerometer mma8452q](https://www.sparkfun.com/products/12756)
* [Piezo Speaker](https://www.sparkfun.com/products/7950)
* 2 330 ohm resistors
* 10k ohm resistor

For this implementation I also used a [Photon Battery
Shield](https://www.sparkfun.com/products/13626) with a [1200mAh
Li-Ion](https://www.adafruit.com/products/258), which is not required for
reproducing this assignment.

### Sensor Reading

#### Photoresistor

The photoresistor has the characteristic of changing its resistance depending
on the amount of light it's receiving. We can estimate the amount of light
hitting the photoresistor by connecting one of its end to the 3v3 and then
reading the voltage coming out on the other end. In this case the second end of
the photoresistor is connected to photon's analog pin A0, and the reading is
made using the method `analogRead()`. The technique used is called [voltage
divider](https://learn.sparkfun.com/tutorials/voltage-dividers). More details
on the connections are given later in this README.

#### Accelerometer

The communication between the photon and the accelerometer is done using I2C
protocol. The SDA and SCL pins of the accelerometer are connected to D0 and D1
respectively. The reading was done using the
[SparkFun's](https://github.com/sparkfun)
[library](https://github.com/sparkfun/MMA8452_Accelerometer/tree/master/Libraries/Arduino/src)
(which needed to be edited to work with photon). Again, more details on the
connection later in this README.

## Web Visualization

Only two libraries were used in this project:

* [jQuery]( https://jquery.com/)
* [Paper.js](http://paperjs.org/)

All visualizations were done (almost) from scratch using only these two
libraries. Two visualizations were created.

The basic view presents in a graph the data received from the photoresistor and
from the accelerometer (the acceleration on the axis x, y and z). The data can
be received either from the cloud or from serial connection.

The second view further explore the capabilities of the project and show the
received data in a indirect way. A photon image is loaded to the canvas in the
central position. Depending on the photon physical orientation it's position on
the canvas changes. The canvas background is painted depending on the amount of
light the photoresistor is receiving (darker background for less light).
Finally the accelerometer is also capable of detecting tap. This is visualized
on the web by switching the photon image to a [Nyan cat](http://www.nyan.cat/).
Upon receiving entering the nyan cat mode, the photon also starts playing the
nyan cat song and shouting rainbow sequence (The status RGB LED iterate over
different colors).

## Connections

![Photon component connections](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia2/connections.jpg)
