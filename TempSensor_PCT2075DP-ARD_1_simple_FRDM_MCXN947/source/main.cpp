/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"temp_sensor/PCT2075.h"

I2C		i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCT2075	sensor( i2c );

int main( void )
{
	printf( "***** Hello, PCT2075! *****\r\n" );
	i2c.scan();

	float	temp;

	while ( true )
	{
		temp	= sensor;
		printf( "Temperature: %8.4f˚C\r\n", temp );

		wait( 1 );
	}
}
