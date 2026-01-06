/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"temp_sensor/P3T1035.h"

/*
 * A header file:"temp_sensor/P3T2030.h" and class "P3T2030" are 
 * aliases of "temp_sensor/P3T1035.h" and "P3T1035".
 * One of those can be used for P3T1035 and P3T2030.
 */

I2C		i2c( D18, D19 );	//	SDA, SCL
P3T1035	sensor( i2c, 0x72 );

int main( void )
{
	printf( "***** Hello, P3T1035/P3T2030! *****\r\n" );

	//	wait reset recovery time of temp sensor (20ms) for if the system is started by power-on
	wait( 0.02 );
	
	i2c.scan();

	float	temp;

	while ( true )
	{
		temp	= sensor;
		printf( "Temperature: %8.4f˚C\r\n", temp );

		wait( 1 );
	}
}
