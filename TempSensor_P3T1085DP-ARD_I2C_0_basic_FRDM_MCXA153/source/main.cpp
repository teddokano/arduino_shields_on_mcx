/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"

I2C		i2c( D18, D19 );	//	SDA, SCL

constexpr uint8_t	static_address	= 0x48;
uint8_t				w_data[]	= { 0 };
uint8_t				r_data[ 2 ];

int main( void )
{
	printf( "***** Hello, P3T1085! *****\r\n" );
	i2c.scan();

	while ( true )
	{
		i2c.write( static_address, w_data, sizeof( w_data ), I3C::NO_STOP );
		i2c.read(  static_address, r_data, sizeof( r_data ) );

		printf( "%f\r\n", (((int)r_data[ 0 ]) << 8 | r_data[ 1 ]) / 256.0 );
		wait( 1 );
	}
}
