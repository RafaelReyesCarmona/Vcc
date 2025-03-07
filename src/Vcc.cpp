/*
Vcc.cpp - Arduino library for measuring VCC supply voltage without external components.
v0.1.5

Created by Ivo Pullens, Emmission, 2014
This version - Copyright © 2022 Francisco Rafael Reyes Carmona.
All rights reserved.

rafael.reyes.carmona@gmail.com


  This file is part of Vcc Library.

  Vcc Library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Vcc Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Vcc Library.  If not, see <https://www.gnu.org/licenses/>.

*/

/*
  Vcc - A supply voltage measuring library for Arduino

  Created by Ivo Pullens, Emmission, 2014
  
  Inspired by:
  http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Vcc.h"
#include <EMA.h>
#include <ADC.h>

Vcc::Vcc( const float correction )
  : m_correction(correction)
{
}

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define ADMUX_VCCWRT1V1 (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
  #define _IVREF 1.1
  #define _IVREF_FAST 1100L
  #define _ADCMAXRES 1024.0
  #define _ADCMAXRES_FAST 1024L
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define ADMUX_VCCWRT1V1 (_BV(MUX5) | _BV(MUX0))
  #define _IVREF 1.1
  #define _IVREF_FAST 1100L
  #define _ADCMAXRES 1024.0
  #define _ADCMAXRES_FAST 1024L
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define ADMUX_VCCWRT1V1 (_BV(MUX3) | _BV(MUX2))
  #define _IVREF 1.1
  #define _IVREF_FAST 1100L
  #define _ADCMAXRES 1024.0
  #define _ADCMAXRES_FAST 1024L
#elif defined(__LGT8FX8P__)
  #define ADMUX_VCCWRT1V1 (_BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX0))
  #define _IVREF 1.024
  #define _IVREF_FAST 1024L
  #define _ADCMAXRES 4096.0
  #define _ADCMAXRES_FAST 4096L
#elif defined(__LGT8FX8E__)
  #define ADMUX_VCCWRT1V1 (_BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
  #define _IVREF 1.25
  #define _IVREF_FAST 1250L
  #define _ADCMAXRES 4096.0
  #define _ADCMAXRES_FAST 4096L
#elif defined(__AVR_ATmega328P__)
  #define ADMUX_VCCWRT1V1 (_BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
  #define _IVREF 1.1
  #define _IVREF_FAST 1100L
  #define _ADCMAXRES 1024.0
  #define _ADCMAXRES_FAST 1024L
#else
  #error Platform not supported.
#endif  

void select_vcc(void) {
  analogReference(DEFAULT);    // Set AD reference to VCC
#if defined(__LGT8FX8P__)
  ADCSRD |= _BV(BGEN);         // IVSEL enable
#endif
  // Read 1.1V/1.024V/1.25V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  if (ADMUX != ADMUX_VCCWRT1V1)
  {
    ADMUX = ADMUX_VCCWRT1V1;
    // Wait for Vref to settle. Bandgap reference start-up time: max 70us
    delayMicroseconds(350); 
  }
}

float Vcc::Read_Volts(void)
{
  select_vcc();
  static EMA<2> EMA_filter;
  uint16_t pVal_filtered;
  uint16_t pVal = adcGet_();

  pVal_filtered = EMA_filter(pVal);

  // Calculate Vcc (in V)
  float vcc = m_correction * _IVREF * _ADCMAXRES / pVal_filtered;

  return vcc;
} // end Read_Volts


uint16_t Vcc::Read_Volts_fast(void)
{
  select_vcc();
  static EMA<2> EMA_filter;
  uint16_t pVal_filtered;
  uint16_t pVal = adcGet_();

  pVal_filtered = EMA_filter(pVal);

  // Calculate Vcc (in mV)
  unsigned long vcc = _IVREF_FAST * _ADCMAXRES_FAST / pVal_filtered;
  //Serial.println(vcc);
  return (uint16_t)vcc;
} // end Read_Volts_fast

float Vcc::Read_Perc(const float range_min, const float range_max, const boolean clip)
{
  // Read Vcc and convert to percentage
  float perc = 100.0 * (Read_Volts()-range_min) / (range_max-range_min);
  // Clip to [0..100]% range, when requested.
  if (clip)
    perc = constrain(perc, 0.0, 100.0);

  return perc;
}