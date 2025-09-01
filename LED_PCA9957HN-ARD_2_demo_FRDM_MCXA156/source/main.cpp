/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"led/PCA9957.h"
#include	"led/LED.h"
#include	<math.h>

#ifndef		M_PI
#define		M_PI	3.14159265358979323
#endif

constexpr	double	cycle	= 100;

SPI		spi( D11, D12, D13, D10 );	//	MOSI, MISO, SCLK, CS
PCA9957	ledd( spi );

LED		leds[]	= {
		LED( ledd,  0 ), LED( ledd,  1 ), LED( ledd,  2 ), LED( ledd,  3 ),
		LED( ledd,  4 ), LED( ledd,  5 ), LED( ledd,  6 ), LED( ledd,  7 ),
		LED( ledd,  8 ), LED( ledd,  9 ), LED( ledd, 10 ), LED( ledd, 11 ),
		LED( ledd, 12 ), LED( ledd, 13 ), LED( ledd, 14 ), LED( ledd, 15 ),
		LED( ledd, 16 ), LED( ledd, 17 ), LED( ledd, 18 ), LED( ledd, 19 ),
		LED( ledd, 20 ), LED( ledd, 21 ), LED( ledd, 22 ), LED( ledd, 23 )
};

int main( void )
{
	printf("***** Hello, PCA9957! *****\r\n");

	ledd.begin( 1.0, PCA9957::ARDUINO_SHIELD );

	double	sin0, sin1, sin2, white = 0, cycle_pi;
	cycle_pi	= M_PI / cycle;

	int count	= 0;

	while ( true )
	{
		if ( !(count++ % 3) )
			white	= 1.0;

		for ( int i = 0; i < cycle; i++ )
		{
			sin0	= sin( (i + 0 * cycle / 3) * cycle_pi );
			sin1	= sin( (i + 1 * cycle / 3) * cycle_pi );
			sin2	= sin( (i + 2 * cycle / 3) * cycle_pi );
			leds[  0 ]	= leds[  4 ]	= leds[  9 ]	= sin0 * sin0;
			leds[  1 ]	= leds[  6 ]	= leds[  7 ]	= sin1 * sin1;
			leds[  2 ]	= leds[  3 ]	= leds[  8 ]	= sin2 * sin2;

			leds[  9 ]	= leds[ 10 ]	= leds[ 11 ]	= leds[ 12 ]	= white;
			leds[  13 ]	= leds[ 14 ]	= leds[ 15 ]					= white;

			white	*= 0.9;

			wait( 1.0 / cycle );
		}
	}
}
