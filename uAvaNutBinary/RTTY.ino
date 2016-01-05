/*
	PicoHorus Telemetry Payload - RTTY Functions
	Mark Jessop <vk5qi@rfhead.net>
	2015-12-30

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


// RTTY Functions - from RJHARRISON's AVR Code
void rtty_txstring (char * string)
{
 
	/* Simple function to sent a char at a time to 
	** rtty_txbyte function. 
	** NB Each char is one byte (8 Bits)
	*/
	
	// Preamble
	rtty_txbyte('\0');
	rtty_txbyte('\0');
	rtty_txbyte('\0');
	rtty_txbyte('\0');
	char c;

	c = *string++;
	while ( c != '\0')
	{
		wdt_reset();
		rtty_txbyte (c);
		c = *string++;
	}

}
 
void rtty_txbyte (char c)
{
	/* Simple function to sent each bit of a char to 
	** rtty_txbit function. 
	** NB The bits are sent Least Significant Bit first
	**
	** All chars should be preceded with a 0 and 
	** proceded with a 1. 0 = Start bit; 1 = Stop bit
	**
	** ASCII_BIT = 7 or 8 for ASCII-7 / ASCII-8
	*/
	int i;
	rtty_txbit (0); // Start bit
	// Send bits for for char LSB first	
	for (i=0;i<7;i++)
	{
		if (c & 1) rtty_txbit(1); 
			else rtty_txbit(0);	
		c = c >> 1;
	}
	rtty_txbit (1); // Stop bit
    rtty_txbit (1); // Stop bit
}
  
void rtty_txbit (int bit)
{
		if (bit)
		{
		  // high
                  analogWrite(RADIO_TXD, 100+(RTTY_SHIFT/7)); // High
		}
		else
		{
		  // low
                  analogWrite(RADIO_TXD, 100);
		}

 		_delay_ms(9.91525);	// Note: This value was determined by trial and error, 
 							// using a CRO to acheive 100 baud, with about 150ppm error.
 							// Yes. I should probably be using interrupts instead. Sue me.
}


// Raw FSK Transmit Functions.
// Note that these are sent MSB first, which is opposite to RTTY.
void fsk_txarray(char * string, int len){

	// Preamble for a bit to let the modem sync.
	fsk_txbyte(0x55);
	fsk_txbyte(0x55);
	fsk_txbyte(0x55);

	// Now transmit the array
	for(int j = 0; j<len; j++){
		wdt_reset();
		fsk_txbyte(string[j]);
	}

}

void fsk_txbyte(char c){
	int i;
	// Send bits for for char LSB first	
	for (i=0;i<8;i++)
	{
		if (c & 0x80) rtty_txbit(1); 
			else rtty_txbit(0);	
		c = c << 1;
	}
}