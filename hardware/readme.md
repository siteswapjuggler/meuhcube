# The hardware folder

This project has been built around the Arduino Mega 2560 and its 70 I/O pins (54 digitals, 16 analogs). This allowed to build a cube composed of 7x7x7 LED the simplest way possible:

  * a grid of 7x7 LED need 49 digital outputs
  * 7 layers of grids need 7 more digital outputs

The cube consume a total of 56 GPIO only to pilot the 343 LED.

## Bill of materials

Here is the parts you'll need to buy:

Farnell ref. | Name | Specs  | Case | Price | Qty
--- | --- | --- | --- | --- | ---
1759415 | C3 | X5R / 6.3V / 22 uF | 0805 | 0,432 € | 1
2496834 | C1 |  X7R / 6.3V / 0.1 uF | 0603 | 0,01 € | 1
2376912 | regulateur | ADP23001 | TSOT | 1,25 € | 1
1467689 | comparateur | TS391ILT | SOT-23-5 | 0,37 € | 1
2333955 | inductance | 4.7 uH | | 0,74 € | 1
1843686 | D1 | | | 0,55 € | 1
1170937 | R1 | 31.6k | 0603 | 0,00 € | 1
1170889 | R2 | 10.2k | 0603 | 0,01 € | 1
\-| R3 | | 603 | 7
1471048 | canal N | | | 0,15 € | 49
2317568 | transistor | | | 0,33 € | 7
2320833 | C2 |  X5R / 25V / 10 uF | 0805 | 0,33 € | 1
1593494 | connecteur | 20 pin / femelle | | 1,80 € | 1
2356147 | connecteur | 36 pin / male | | 2,10 € | 1
2356160 | connecteur | 8 pin / male | | 0,45 € | 5
1841229 | connecteur | 10 pin / male | | 0,54 € | 1
1800872 | pieds | 3 mm / noir | | 0,15 € | 4

+ PCB 21,13 €

Total = 41,94 € 