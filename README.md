<img src="img/voltaje.png" width=48 height=48 align="right">

# Vcc #
[![Library: Vcc](https://img.shields.io/badge/Library-Vcc-red?style=for-the-badge&logo=Arduino)](README.md)
[![Version: v0.1.5](https://img.shields.io/badge/Version-v0.1.5-blue?style=for-the-badge&logo=v)]()

Arduino library for measuring VCC supply voltage without external components.

## Original info ##

Arduino library to read VCC supply level without external components
This version also can be run on Logicgreen LG8F328P microcontroller.

<b>Warning!</b> Be aware running this program on ATmega, becase switching reference voltage to VCC will drive AREF pin with VCC. So if an external referece is connected to AREF pin it will be shorted to VCC.

Unfortunately it doesn't work on Logicgreen LGT8F328D. This because if ADMUX selecting reference voltage as VCC instead of internal reference then internal reference will be shut down.

## Installation ##
This library need EMA.h library (https://github.com/RafaelReyesCarmona/EMA) and ADC.h library (https://github.com/RafaelReyesCarmona/EMA). 

## Changelog

### 0.1.5
    * Fixed duplicity on defines sentences.

### 0.1.4
    * Added function 'Read_Volts_fast'.

## License ##

This file is part of Vcc Library.

Vcc Library is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Vcc Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Vcc Library.  If not, see <https://www.gnu.org/licenses/>.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

## Authors ##
[Ivo Pullens](https://github.com/Yveaux/Arduino_Vcc)
This version Copyright © 2022 Francisco Rafael Reyes Carmona.
Contact me: rafael.reyes.carmona@gmail.com

## Credits ##

Electric meter icon at the beginning is from [Flaticon.es](https://www.flaticon.es) designed by [orvipixel](https://www.flaticon.es/autores/orvipixel)
and licensed by [free license](img/license.pdf).
