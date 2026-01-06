/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6416A.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6416A	gpio( i2c );

int main(void)
{
	printf( "***** Hello, PCAL6416A! *****\r\n" );
	printf( "  Press buttons to turn ON the LEDs and show port read value in serial terminal. \r\n" );

	gpio.begin( PCAL6416A::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0xFF,	// Configure port1 as INPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0 is OUT, port1 is IN
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port1
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port1

	while ( true )
	{
		int input1	= gpio.input( 1 );	//  Read port1 input
		gpio.output( 0, input1 );		//  Output to port0

		printf( "0x%02X\r\n", input1 );

		wait( 0.1 );
	}
}
