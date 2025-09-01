/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL9722.h"
#include	<ctype.h>
#include	<string.h>

SPI			spi( D11, D12, D13, D10 );
PCAL9722	gpio( spi );
InterruptIn	int_pin( D7 );
Ticker		t;
bool		int_flag	= false;
uint8_t		buttons		= 0x3F;

void	message( const char *s, float dulation = 0.2, float blank = 0.1 );
void	disp7seg( char c );
int		button_display( uint8_t v );
void	button_callback( void );
void	auto_message( int t );
void	ticker_callback( void );

int main( void )
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
	gpio.input( 2 );										//	Clear interrupt

	int_pin.fall( button_callback );
	t.attach( ticker_callback, 0.05 );

//	message( "...Hello PCAL9722  Hello PCAL9722" );
//	message( "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

	while ( true )
	{
		if ( int_flag )
		{
			int_flag = false;

			uint8_t	status[ gpio.n_ports ];

			gpio.read_port( INT_STATUS, status );

			printf( "[INT] status 0~2:" );
			for ( int i = 0; i < gpio.n_ports; i++ )
				GPIO_base::print_bin( status[ i ] );

			buttons	= gpio.input( 2 );

			printf( ",  input 2: " );
			GPIO_base::print_bin( buttons );
			printf( "\r\n" );
		}
		wait( 0 );
	}
}

void message( const char *s, float dulation, float blank )
{
	char c;

	while ( (c	= *s++) )
	{
		disp7seg( c );
		wait( dulation );
		disp7seg( 0xFF );
		wait( blank );
	}
}

void disp7seg( char c )
{
	static uint8_t	patterns[]	= {
			0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,	//	0~7
			0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, //	8~F
			0xC2, 0x89, 0xCF, 0xF1, 0x8D, 0xC7, 0xAA, 0xAB,	//	G~N
			0xA3, 0x8C, 0x98, 0xAF, 0x92, 0x87, 0xE3, 0xC1,	//	O~V
			0xE2, 0x9B, 0x91, 0xB6							//	W~Z
	};

	if ( isdigit( c ) )
		gpio.output( 1, patterns[ c - '0' ] );
	else if ( isalpha( c ) )
		gpio.output( 1, patterns[ tolower( c ) - 'a' + 10 ] );
	else if ( '.' == c )
		gpio.output( 1, 0x7F );
	else
		gpio.output( 1, 0xFF );
}

int button_display( uint8_t v )
{
	for ( int i = 5; i >= 0; i-- )
	{
		if ( (v >> i) & 0x1 )
		{
			disp7seg( i + '0' );
			return i;
		}
	}

	return 0;
}

void button_callback( void )
{
	int_flag = true;
}

void auto_message( int t )
{
	const char		s[]	= " Hello PCAL9722 SPI GPIO Expander";
//	const char		s[]	= "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	constexpr int	len	= sizeof( s );

	if ( t < 20 )
		return;

	t	= t - 20;
	int	p	= (t >> 3) % len;

	if ( t & 0x7 )
		disp7seg( *(s + p) );
	else
		disp7seg( ' ' );
}

void ticker_callback( void )
{
	static uint8_t	pat[] = {  //  LED blinking pattern
		0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF,
		0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD
	};
	static int	count 		= 0;
	static int	event_time	= 0;

	if ( 0x3F & ~buttons )
	{
		button_display( 0x3F & ~buttons );
		event_time	= count;
	}
	else
	{
		auto_message( count - event_time );
	}

	gpio.output( 0, (buttons | 0xC0) & pat[ count++ % sizeof( pat ) ] );
}
