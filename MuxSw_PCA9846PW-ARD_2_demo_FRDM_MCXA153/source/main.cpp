/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

/** PCA9846 operation sample
 *
 *  About PCA9846:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/ic-multiplexers-switches/four-channel-ultra-low-voltage-fm-plus-ic-bus-switch-with-reset:PCA9846
 */

#include	"r01lib.h"
#include	"mux_sw/PCA9846.h"
#include	"misc/eeprom/M24C02.h"

I2C			i2c( I2C_SDA, I2C_SCL );
PCA9846		sw( i2c );
M24C02 		eeprom[]	= {
								M24C02( i2c, 0xA0 >> 1 ),
								M24C02( i2c, 0xA2 >> 1 ),
								M24C02( i2c, 0xA4 >> 1 ),
								M24C02( i2c, 0xA6 >> 1 )
							};

#define	READ_BUFFER_SIZE	128

void	try_eeprom_read( void );

int main( void )
{
	printf( "***** Hello, PCA9846! *****\r\n" );

	char test_str[]	= "EEPROM[*] is responding - Hello, PCA9846 Arduino shield evaluation board test program";

	printf( "test string: %s\r\n", test_str );
	printf( "test string length: %u\r\n", sizeof( test_str ) );

	sw.select( PCA9846::CH0 | PCA9846::CH1 | PCA9846::CH2 | PCA9846::CH3 );

	for ( int i = 0; i < sw.N_CH; i++ )
	{
		test_str[ 7 ]	= i + '0';
		eeprom[ i ].write( 0, (uint8_t*)test_str, sizeof( test_str ) );
	}

	while ( true )
	{
		printf( "\r\n*** PCA9846 setting: All channels ON ***\r\n" );
		sw.select( PCA9846::CH0 | PCA9846::CH1 | PCA9846::CH2 | PCA9846::CH3 );
		try_eeprom_read();

		wait( 1 );

		for ( int i = 0; i < sw.N_CH; i++ )
		{
			printf( "\r\n*** PCA9846 setting: only channel %d is ON ***\r\n", i );
			
			sw.select( 0x1 << i );
			try_eeprom_read();

			wait( 1 );
		}
	}
}

void try_eeprom_read( void )
{
	char		read_str[ READ_BUFFER_SIZE ];
	
	for ( int i = 0; i < sw.N_CH; i++ )
	{
		if ( eeprom[ i ].ping() )
		{
			eeprom[ i ].read( 0, (uint8_t*)read_str, READ_BUFFER_SIZE );
			printf( "%s\r\n", read_str );
		}
		else
		{
			printf( "EEPROM[%d] is not responding\r\n", i );
		}
	}
}
