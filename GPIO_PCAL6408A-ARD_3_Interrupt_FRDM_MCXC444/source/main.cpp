/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6408A.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6408A	gpio( i2c );
InterruptIn	int_pin( D10 );

bool		int_flag		= false;

void callback( void ) {
	int_flag = true;
}

int main(void)
{
	printf( "***** Hello, PCAL6408A! *****\r\n" );
	printf( "  Watch the LEDs are blinking\r\n" );
	printf( "  And press buttons to generate interrupts\r\n" );

	gpio.begin( PCAL6408A::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	constexpr uint8_t	io_config_and_pull_up	= 0xF0;

	gpio.write_port(CONFIG, io_config_and_pull_up);			//  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT
	gpio.write_port(PULL_UD_EN, io_config_and_pull_up);		//  Pull-up/down in port0 bit7~4 are enabled
	gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);	//  Pull-up/down in port0 bit7~4 are as pull-up

	gpio.write_port( INT_MASK, (uint8_t)~io_config_and_pull_up );	//  Clear mask bits for input

	int_pin.fall( callback );

	while ( true )
	{
		static uint8_t	pat[]	= { 0xFE, 0xFD, 0xFB, 0xF7, 0xFB, 0xFD, };	//  LED blinking pattern
		static int		count	= 0;
		uint8_t			input0;

		if ( int_flag )
		{
			int_flag = false;

			uint8_t	status[ gpio.n_ports ];

			gpio.read_port( INT_STATUS, status );

			printf( "[INT] status:" );

			for ( int i = 0; i < gpio.n_ports; i++ )
				GPIO_base::print_bin( status[i] );

			input0 = gpio.input( 0 );

			printf( ",  input 0: " );
			GPIO_base::print_bin( input0 );
			printf( "\r\n" );
		}
		else
		{
			input0 = gpio.input( 0 );
		}

		gpio.output( 0, (input0 >> 4) & pat[ count++ % sizeof( pat ) ] );
		wait( 0.080 );
	}
}
