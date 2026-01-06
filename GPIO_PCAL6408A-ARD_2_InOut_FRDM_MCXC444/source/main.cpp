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
	printf( "  Press buttons to turn ON the LEDs and show port read value in serial terminal. \r\n" );

	gpio.begin( PCAL6408A::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	constexpr uint8_t	io_config_and_pull_up = 0xF0;

	gpio.write_port( CONFIG,      io_config_and_pull_up );	//  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down in port0 bit7~4 are enabled
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up/down in port0 bit7~4 are as pull-up

	while ( true )
	{
		int input0	= gpio.input( 0 );	//  Read port0 input
		gpio.output( 0, input0 >> 4 );	//  Output to port0

		printf( "0x%02X\r\n", input0 );

		wait( 0.1 );
	}
}
