# The MeuhCube project

The MeuhCube is an Open Source 7x7x7 LED cube project based on Arduino Mega 2560. It aims to provide any needed resources to create your own cube, including the electronic schematics, a laser cutted case and the firmware.

A MeuhCube can basically display embedded animations with the Arduino sketch or be driven over USB by any smart device like a computer, a Raspberry Pi or a smartphone.

The Arduino sketch also exposes a serial API over USB that can be implemented by some remote programs.

## What can I find in this repository?

There is some folders categorizing the resources:

  * **hardware:** Contains a bill of materials you'll need to buy, the electronic schematics, a laser cutted case and some 3D printable tools that will help you in you task
  * **software:** Contains the Arduino firmware and a young simulator, all you need to start!

You'll find more informations in the readme file of each folder.

## Remote clients over USB

You can drive a cube over USB with one of the clients in the **software** folder:

  * a Max/MSP sketch
  * a Proccesing library and simulator
  * a Python Module in Progress
  
You can also use this clients under developpement : 
  * [PLFM, a Python library for MeuhCube](https://gitlab.com/influencepc/Python-library-for-MeuhCube)

## External resources

We also post an additionnal documentation of this project in our local Fablab website :

  * 2016-01-15: [[fr] Meuh cube: Cube lumineux 7\*7\*7 Led V1](https://fablab.ledome.info/#!/projects/cube-lumineux-7-7-7-led)
  * 2016-09-01: [[fr] Meuhcube / cube à leds 7\*7\*7 v2](https://fablab.ledome.info/#!/projects/cube-lumineux-7-7-7-led-v2)
