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
	printf( "  Press buttons to turn ON the LEDs and show port read value in serial terminal. \r\n" );

	gpio.begin( PCAL9722::ARDUINO_SHIELD );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0x3F,	// Configure port2 bit 5~0 as INTPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0, 1 and port2 are configured as output
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port2 bit 5~0
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port2 bit 5~0

	while ( true )
	{
		int input2	= gpio.input( 2 );	//  Read port2 input
		gpio.output( 0, input2 );	//  Output to port2

		printf( "0x%02X\r\n", input2 );

		wait( 0.1 );
	}
}
