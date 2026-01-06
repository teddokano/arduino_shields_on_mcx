/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6534.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6534	gpio( i2c, 0x22 );

int main(void)
{
	printf( "***** Hello, PCAL6534! *****\r\n" );
	printf( "  Press buttons to turn ON the LEDs and show port read value in serial terminal. \r\n" );

	gpio.begin( PCAL6534::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0x00,	// Configure port2 as OUTPUT
		0xE0,	// Configure port3 bit 7~5 as INPUT
		0x03,	// Configure port4 bit 1 and 0 as INPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0, 1, 2 and port3 bit 4~0 are configured as output
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port3 bit 7~5 and port4 bit 1 and 0
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port3 bit 7~5 and port4 bit 1 and 0

	while ( true )
	{
		int input3	= gpio.input( 3 );	//  Read port3 input
		int input4	= gpio.input( 4 );	//  Read port4 input
		int output2	= ( input3 & 0xFC ) | input4;
		gpio.output( 2, output2 );	//  Output to port2

		printf( "0x%02X\r\n", output2 );

		wait( 0.1 );
	}
}
