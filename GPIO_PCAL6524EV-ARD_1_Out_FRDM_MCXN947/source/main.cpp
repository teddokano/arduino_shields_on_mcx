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
	printf( "Watch the LEDs counting in binary\r\n" );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0xF0,	// Configure port2 bit 7~4 as INPUT
	};

	gpio.config( io_config_and_pull_up );	//  Port0, 1 and port2 bit 3~0 are configured as output

	while ( true )
	{
		static int count = 0;
		gpio.output( 2, count++ );	//  Output to port2

		wait( 0.1 );
	}
}
