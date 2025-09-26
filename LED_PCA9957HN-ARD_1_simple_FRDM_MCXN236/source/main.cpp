/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"led/PCA9957.h"

constexpr	int	cycle	= 100;

SPI		spi( ARD_MOSI, ARD_MISO, ARD_SCK, ARD_CS );	//	MOSI, MISO, SCLK, CS
PCA9957	ledd( spi );

int main( void )
{
	printf("***** Hello, PCA9957! *****\r\n");

	ledd.begin( 1.0, PCA9957::ARDUINO_SHIELD );

	while ( true )
	{
		for ( int ch = 0; ch < 16; ch++ )
		{
			for ( int i = 1; i <= cycle; i++ )
			{
				ledd.pwm( ch, (float)i / (float)cycle );
				wait( 0.01 );
			}

			for ( int i = cycle; i >= 0; --i )
			{
				ledd.pwm( ch, (float)i / (float)cycle );
				wait( 0.01 );
			}
		}
	}
}

