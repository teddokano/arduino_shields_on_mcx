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

constexpr uint8_t	static_address	= 0x72;
constexpr uint8_t	dynamic_address	= 0x08;

I3C		i3c( D18, D19 );	//	SDA, SCL

P3T1035		sensor( i3c, static_address );

void	DAA_set_dynamic_ddress_from_static_ddress( uint8_t static_address, uint8_t dynamic_address );

int main( void )
{
	printf("P3T1035/P3T2030 (Temperature sensor) I3C operation sample: getting temperature data\r\n");

	DAA_set_dynamic_ddress_from_static_ddress( static_address, dynamic_address );
	sensor.address_overwrite( dynamic_address );

	float	temp;

	while ( true )
	{
		temp	= sensor;
		printf( "Temperature: %8.4f˚C\r\n", temp );

		wait( 1 );
	}
}

void DAA_set_dynamic_ddress_from_static_ddress( uint8_t static_address, uint8_t dynamic_address )
{
	i3c.ccc_broadcast( CCC::BROADCAST_RSTDAA, NULL, 0 ); // Reset DAA
	i3c.ccc_set( CCC::DIRECT_SETDASA, static_address, dynamic_address << 1 ); // Set Dynamic Address from Static Address
}
