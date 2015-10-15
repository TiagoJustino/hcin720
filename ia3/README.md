# hcin720 - Assignment 3

This assignment illustrates how to use a laser cutter a 3D printer enclosures
for a micro controller project. Moreover, it demonstrate how the final product
  can interact with everyday objects.

## Step 1 - Soldering

The first step towards a more polished product is getting rid of the
breadboard. The battery shield and the accelerometer were soldered together.
Also, female and male connectors were added to the battery shield pins and to
the sensors and actuators as necessary. Finally a on/off switch was added to
the battery:

![Soldered parts](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/01_parts.jpg)

## Step 2 - Cardboard enclosure

A cardboard enclosure was created using the laser cutter. The box was designed
using a 2D design software (Adobe Illustrator) and the cardboard was cut using
a [universal laser systems vls
6.60](http://www.engravingsys.com/flying-optics-laser/vls-platform-laser/vls-6-60-platform/).

![Lasercut pieces](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/02_lasercut_pieces.jpg)

![Lasercut box showing the slots for battery and photon](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/03_lasercut_box_with_slots.jpg)

![Assembled lasercut box](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/04_assembled_lasercut_box.jpg)

![Assembled lasercut box, closed, with photon](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/05_close_lasercut_box.jpg)

The result can be seen in these two videos: [Lasercut
box](https://www.youtube.com/watch?v=6FM2jhhoWrU) and [Lasercut box
demo](https://www.youtube.com/watch?v=5v9mZn7W5Ps).

## Step 3 - 3D printed enclosure

Following the cardboard enclosure, a new enclosure was created for 3d printing,
using [Tinkercad](tinkercad.com). The same ideas was kept from the previous
enclosure, where the battery is separated from the photon. Two cuts were made
to the upper face so the sensors and actuators can be exposed. A sliding led
was is used for closing the box completely:

![3D printed box](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/08_3d_printed_box.jpg)

## Step 4 - Attaching a servo for creating a self stabilizing platform.

Improving the previous project, a servo motor was attached for creating a self
stabilizing platform. The accelerometer is used for calculating the tilting
angle. The servo motor compensates the inclination. A new lid was designed for
attaching the motor as seen below.

![3D printed lid for attaching servo](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/09_lid_for_attaching_servo.jpg)

A platform was also designed to be attached to the motor. The video
[Accelerometer controlled servo](https://www.youtube.com/watch?v=7Pjy6zcO87I)
demonstrates how the servo motor can be used to compensate the tilting angle
detected by the accelerometer.

The pictur bellow shows the final assembly. The video [Final self stabilizing
platform demo](https://www.youtube.com/watch?v=VcfdFXkriTA) shows the final
product and exemplifies its applicability, keeping the cup stable. This idea
can be used in a boat for keeping cups and other objects stable.

![3D printed box assembled with servo](https://raw.githubusercontent.com/TiagoJustino/hcin720/master/ia3/11_final_assembly.jpg)

### Attribution

The idea for this project was based in two youtube videos:
[CoffeeMate](https://www.youtube.com/watch?v=EbMFuhaoxpo) and [Gyroscopic
self-leveling pool table](https://www.youtube.com/watch?v=N-aE5oszXyQ).
