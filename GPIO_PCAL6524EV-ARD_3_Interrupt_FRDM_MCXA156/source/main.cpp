/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6524.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCAL6524	gpio( i2c, 0x22 );
InterruptIn	int_pin( D10 );

bool		int_flag		= false;

void callback( void ) {
	int_flag = true;
}

int main(void)
{
	printf( "***** Hello, PCAL6524! *****\r\n" );
	printf( "  Watch the LEDs are blinking\r\n" );
	printf( "  And press buttons to generate interrupts\r\n" );
	I2C_device::scan( i2c );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0xF0,	// Configure port2 bit 7~4 as INPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0, 1 and port2 bit 3~0 are configured as output
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port2 bit 7~4
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port2 bit 7~4

	gpio.write_port( INT_MASK, (uint8_t)(~0xF0), 2 );		//  Interrupt mask cleared on port2 bit 7~4

	int_pin.fall( callback );

	while ( true )
	{
		static uint8_t pat[] = {	//  LED blinking pattern
			0xFE, 0xFB, 0xFD, 0xF7, 0xFD, 0xFB,
		};
		static int	count = 0;
		uint8_t		input2;

		if ( int_flag )
		{
			int_flag = false;

			uint8_t	status[ gpio.n_ports ];

			gpio.read_port( INT_STATUS, status );

			printf( "[INT] status 0~%d:", gpio.n_ports - 1 );

			for ( int i = 0; i < gpio.n_ports; i++ )
				GPIO_base::print_bin( status[i] );

			input2 = gpio.input( 2 );

			printf( ",  input 2: " );
			GPIO_base::print_bin( input2 );
			printf( "\r\n" );
		}
		else
		{
			input2 = gpio.input( 2 );
		}

		gpio.output( 2, (input2 >> 4) & pat[ count++ % sizeof( pat ) ] );
		wait( 0.080 );
	}
}
