/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"temp_sensor/P3T1085.h"

I2C		i2c( D18, D19 );	//	SDA, SCL
P3T1085	sensor( i2c, 0x48 );

int main( void )
{
	printf( "***** Hello, P3T1085! *****\r\n" );
	i2c.scan();

	float	temp;

	while ( true )
	{
		temp	= sensor;
		printf( "Temperature: %8.4f˚C\r\n", temp );

		wait( 1 );
	}
}
