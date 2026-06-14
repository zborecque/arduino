# Zborecque's Arduino LED projects

Just a bunch of my personal Arduino experiments used mainly to operate addressable LED stripes on my RC models fleet.

## Setup
* Board: `SparkFun Pro Micro` [more details](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all).
* **NOTE!** remember to select the right CPU voltage (in my case it's 5V), in case you mess up [see here](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/troubleshooting-and-faq#ts-reset).

## Projects

### FPV Combat damage fire

_under development_

A concept of having a [FPV Combat](https://fpv-combat.com) aircraft of (Raptus)[https://www.fpv-combat.com/download/raptus/] class being illuminated with addressable LEDs that would imitate fire effects increasing with taken damage. There might be also some additional effects available. Main PWM signal will be coming from the FPV Combat board (which is able to control one of the servos - to indicate taken damage by briefly distorting one of the aircraft steering planes - for example rudder). Additional effects might use another PWM channel, but also detecting the FPVC mixing of a single PWM channel might be used.

### Black_Mesa_Wing

Final and operating version used with `Atreides` FPV flying wing. This aircraft is not existing anymore, therefore the project and all variables are abandoned. The project consisted of some defined LED indexes (there were multiple LED strip parts glued across the hull of the model, and were illuminnating in variety of sequences), and was operated using the input PWM signal. It was possible to achieve several lighting modes, that were chosen with use of the TX switches and mixes. Some of the illuminaitons were simple colour-changing effects, but also there were some animations (like sliding turn signal) and avaiation lights mode (simulating actual aircraft lights and beacons, with landing lights)

### sketch_feb23a

Just a simple playground for initial version of illuminating an RC model with an WS2811 LED strip. LED light sequences are being changed with the use of the PWM input that is particularly assigned to do it.