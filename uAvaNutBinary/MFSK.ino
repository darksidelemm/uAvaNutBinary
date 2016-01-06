/*
	PicoHorus Telemetry Payload - MFSK Functions
	Mark Jessop <vk5qi@rfhead.net>
	2016-01-06

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a full copy of the GNU General Public License, 
    see <http://www.gnu.org/licenses/>.
*/


#include <util/delay.h>

  
void mfsk_txsymbol (uint8_t symbol)
{
    switch(symbol){
        case 0:
            digitalWrite(3, HIGH);
            analogWrite(RADIO_TXD, 100);
            break;
        case 1:
            digitalWrite(3, HIGH);
            analogWrite(RADIO_TXD, 120);
            break;
        case 2:
            digitalWrite(3, LOW);
            analogWrite(RADIO_TXD, 140);
            break;
        case 3:
            digitalWrite(3, LOW);
            analogWrite(RADIO_TXD, 160);
            break;
        default:
            digitalWrite(3, HIGH);
            analogWrite(RADIO_TXD, 100);
            break;
    }

    _delay_ms(19.85); 
}


// Raw MFSK Transmit Functions.
// Note that these are sent MSB first, which is opposite to RTTY.
void mfsk_txarray(char * string, int len){

    // Preamble for a bit to let the modem sync.
    mfsk_txbyte(0x1B);
    mfsk_txbyte(0x1B);
    mfsk_txbyte(0x1B);
    mfsk_txbyte(0x1B);

    // Now transmit the array
    for(int j = 0; j<len; j++){
        wdt_reset();
        mfsk_txbyte(string[j]);
    }

}

void mfsk_txbyte(char c){
    int i;
    // Send bits for for char MSB first 
    for (i=0;i<4;i++)
    {
        uint8_t symbol = ((uint8_t)c & 0xC0) >> 6;
        mfsk_txsymbol(symbol);
        c = c << 2;
    }
}