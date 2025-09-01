/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL9722.h"

SPI			spi( D11, D12, D13, D10 );
PCAL9722	gpio( spi );
InterruptIn	int_pin( D7 );

bool		int_flag		= false;

void callback( void ) {
	int_flag = true;
}

int main(void)
{
	printf( "***** Hello, PCAL9722! *****\r\n" );
	printf( "  Watch the LEDs are blinking\r\n" );
	printf( "  And press buttons to generate interrupts\r\n" );

	gpio.begin( PCAL9722::ARDUINO_SHIELD );

	uint8_t io_config_and_pull_up[] = {
		0x00,	// Configure port0 as OUTPUT
		0x00,	// Configure port1 as OUTPUT
		0x3F,	// Configure port2 bit 5~0 as INTPUT
	};

	gpio.config( io_config_and_pull_up );					//  Port0, 1 and port2 are configured as output
	gpio.write_port( PULL_UD_EN,  io_config_and_pull_up );	//  Pull-up/down enabled for port2 bit 5~0
	gpio.write_port( PULL_UD_SEL, io_config_and_pull_up );	//  Pull-up selected for port2 bit 5~0

	gpio.write_port( INT_MASK, (uint8_t)(~0x3F), 2 );		//  Interrupt mask cleared pn port2 bit 5~0

	int_pin.fall( callback );

	while ( true )
	{
		static uint8_t	pat[] = {  //  LED blinking pattern
			0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF,
			0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD
		};		static int	count = 0;
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

		gpio.output( 1, input2 );
		gpio.output( 0, pat[ count++ % sizeof( pat ) ] );
		wait( 0.080 );
	}
}
