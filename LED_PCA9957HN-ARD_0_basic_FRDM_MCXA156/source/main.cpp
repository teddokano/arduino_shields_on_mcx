/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"led/PCA9957.h"

constexpr int		cycle	= 100;

SPI			spi( D11, D12, D13, D10 );	//	MOSI, MISO, SCLK, CS

DigitalOut	oe(  D9, 0 );	//	OE pin LOW
DigitalOut	rst( D8, 1 );	//	RESET pin HIGH

int main( void )
{
	printf("***** Hello, PCA9957! *****\r\n\r\n");

	printf( "register addresses\r\n" );
	printf( "  PCA9957::LEDOUT0 = 0x%02X\r\n", PCA9957::LEDOUT0 );
	printf( "  PCA9957::PWMALL  = 0x%02X\r\n", PCA9957::PWMALL  );
	printf( "  PCA9957::IREFALL = 0x%02X\r\n", PCA9957::IREFALL );
	printf( "  PCA9957::PWM0    = 0x%02X\r\n", PCA9957::PWM0    );

	uint8_t	w_data[]	= {
			(PCA9957::LEDOUT0 + 0) << 1, 0xAA,
			(PCA9957::LEDOUT0 + 1) << 1, 0xAA,
			(PCA9957::LEDOUT0 + 2) << 1, 0xAA,
			(PCA9957::LEDOUT0 + 3) << 1, 0xAA,
			(PCA9957::LEDOUT0 + 4) << 1, 0xAA,
			(PCA9957::LEDOUT0 + 5) << 1, 0xAA
	};
	uint8_t	m2_data[]	= { PCA9957::MODE2   << 1, 0x18 };	//	clear error and no auto switch-off
	uint8_t	iref_data[]	= { PCA9957::IREFALL << 1, 0xFF };	//	output current maximum
	uint8_t	pwm_data[ 2 ];
	uint8_t	dummy[ sizeof( w_data ) ];

	spi.write( m2_data,   dummy, sizeof( iref_data ) );
	spi.write( w_data,    dummy, sizeof(    w_data ) );
	spi.write( iref_data, dummy, sizeof( iref_data ) );

	while ( true )
	{
		for ( int channel = 0; channel < 24; channel++ )
		{
			pwm_data[ 0 ]	= (PCA9957::PWM0 + channel) << 1;

			for ( int pwm = 0; pwm < 256; pwm++ )
			{
				pwm_data[ 1 ]	= pwm;
				spi.write( pwm_data, dummy, sizeof( pwm_data ) );
				wait( 0.01 );
			}

			pwm_data[ 1 ]	= 0x00;
			spi.write( pwm_data, dummy, sizeof( pwm_data ) );
		}
	}
}
