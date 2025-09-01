/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6416A.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6416A	gpio( i2c );
InterruptIn	int_pin( D10 );

bool		int_flag		= false;

void callback( void ) {
	int_flag = true;
}

int main(void)
{
	printf( "***** Hello, PCAL6416A! *****\r\n" );
	printf( "  Watch the LEDs are blinking\r\n" );
	printf( "  And press buttons to generate interrupts\r\n" );

	gpio.begin( PCAL6416A::ARDUINO_SHIELD );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0xFF,	// Configure port1 as OUTPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0 is OUT, port1 is IN
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port1
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port1

	gpio.write_port( INT_MASK, (uint8_t)(~0xFF), 1 );		//  Interrupt mask cleared on port1

	int_pin.fall( callback );

	while ( true )
	{
		static uint8_t pat[] = {	//  LED blinking pattern
			0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 
			0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD,
		};
		static int	count = 0;
		uint8_t		input1;

		if ( int_flag )
		{
			int_flag = false;

			uint8_t	status[ gpio.n_ports ];

			gpio.read_port( INT_STATUS, status );

			printf( "[INT] status 0~%d:", gpio.n_ports - 1 );

			for ( int i = 0; i < gpio.n_ports; i++ )
				GPIO_base::print_bin( status[i] );

			input1 = gpio.input( 1 );

			printf( ",  input 1: " );
			GPIO_base::print_bin( input1 );
			printf( "\r\n" );
		}
		else
		{
			input1 = gpio.input( 1 );
		}

		gpio.output( 0, input1 & pat[ count++ % sizeof( pat ) ] );
		wait( 0.062 );
	}
}
