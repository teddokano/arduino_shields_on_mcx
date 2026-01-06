/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6524.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6524	gpio( i2c, 0x22 );

int main(void)
{
	printf( "***** Hello, PCAL6524! *****\r\n" );
	printf( "  Press buttons to turn ON the LEDs and show port read value in serial terminal. \r\n" );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0xF0,	// Configure port2 bit 7~4 as INPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0, 1 and port2 bit 4~0 are configured as output
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port2 bit 7~4
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port2 bit 7~4

	while ( true )
	{
		int input2	= gpio.input( 2 );	//  Read port2 input
		gpio.output( 2, input2 >> 4 );	//  Output to port2

		printf( "0x%02X\r\n", input2 );

		wait( 0.1 );
	}
}
