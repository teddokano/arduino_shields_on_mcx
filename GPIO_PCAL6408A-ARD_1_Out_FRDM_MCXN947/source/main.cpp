/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6408A.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6408A	gpio( i2c );

int main(void)
{
	printf( "***** Hello, PCAL6408A! *****\r\n" );
	printf( "Watch the LEDs counting in binary\r\n" );

	gpio.begin( PCAL6408A::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	gpio.write_port( CONFIG, 0xF0 );	//  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT

	while ( true )
	{
		static int	count	= 0;
		gpio.output( 0, count++ );	//  Output to port0

		wait( 0.1 );
	}
}
