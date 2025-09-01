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
	printf( "Watch the LEDs counting in binary\r\n" );

	gpio.begin( PCAL6416A::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0xFF,	// Configure port1 as INPUT
	};

	gpio.config( io_config_and_pull_up );	//  Port0 is OUT, poer1 is IN
	
	while ( true )
	{
		static int count = 0;
		gpio.output( 0, count++ );	//  Output to port0

		wait( 0.1 );
	}
}
