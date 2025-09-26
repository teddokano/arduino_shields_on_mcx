/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL9722.h"

SPI			spi( ARD_MOSI, ARD_MISO, ARD_SCK, ARD_CS );	//	MOSI, MISO, SCLK, CS
PCAL9722	gpio( spi );

int main(void)
{
	printf( "***** Hello, PCAL9722! *****\r\n" );
	printf( "Watch the LEDs counting in binary\r\n" );

	gpio.begin( PCAL9722::ARDUINO_SHIELD );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0x3F,	// Configure port2 bit 5~0 as INTPUT
	};

	gpio.config( io_config_and_pull_up );	//  Port0, 1 and port2 bit 7 and 6 are configured as output

	while ( true )
	{
		static int count = 0;
		gpio.output( 0, count++ );	//  Output to port2

		wait( 0.1 );
	}
}
