/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"pin_control.h"
#include	"temp_sensor/P3T1755.h"

I3C		i3c( D18, D19 );	//	SDA, SCL

const uint8_t	dynamic_address_list[]	= { 0x1A, 0x2B, 0x3C };

P3T1755	sensor[]	= {
						P3T1755( i3c ),
						P3T1755( i3c ),
						P3T1755( i3c )
				  };

LM75B	lm75b( i3c, 0x4F );

#define	WAIT_SEC	0.96

DigitalOut	r(    RED   );
DigitalOut	g(    GREEN );
DigitalOut	b(    BLUE  );
DigitalOut	trig( MB_INT);	//	IBI detection trigger output for oscilloscope

void	DAA_set_dynamic_ddress_from_static_ddress( uint8_t static_address, uint8_t dynamic_address );
int		DAA( const uint8_t *address_list, uint8_t list_length );
bool	check_i2c_device( LM75B ts );
void	info( LM75B ts );

int main( void )
{
	init_pin_control();
	i3c.set_IBI_callback( ibi_trigger_output );

	PRINTF("\r\nP3T1755 (Temperature sensor) I3C operation sample: getting temperature data and IBI\r\n");

	int	ndev	= DAA( dynamic_address_list, sizeof( dynamic_address_list ) );

	float	ref_temp	= 0;

	for ( int i = 0; i < ndev; i++)
	{
		sensor[ i ].address_overwrite( dynamic_address_list[ i ] );

		ref_temp	= sensor[ i ];
		float low	= ref_temp + 1.0;
		float high	= ref_temp + 2.0;

		sensor[ i ].thresholds( low, high );

		sensor[ i ].bit_op8( P3T1755::Conf, ~0x02, 0x02 );	//ALART pin configured to INT mode
		sensor[ i ].ccc_set( CCC::DIRECT_ENEC, 0x01 );		// Enable IBI

		info( sensor[ i ] );
	}

	bool	i2c_device	= check_i2c_device( lm75b );
	float	temp		= 0;
	uint8_t	ibi_addr;

	while ( true )
	{
		if ( (ibi_addr	= i3c.check_IBI()) )
			PRINTF( "\r\n*** IBI : Got IBI from target_address: 0x%02X", ibi_addr );

		PRINTF( "\r\n" );

		for ( int i = 0; i < ndev; i++)
			PRINTF( "  %8.4f˚C @%02X", (float)sensor[ i ], dynamic_address_list[ i ] );

		if ( i2c_device )
		{
			i3c.mode( I3C::I2C_MODE );
			PRINTF( "  %7.3f˚C @4F(I2C)", (float)lm75b );
			i3c.mode( I3C::I3C_MODE );
		}

		led_set_color( temp, ref_temp );
		wait( WAIT_SEC );
	}
}

void DAA_set_dynamic_ddress_from_static_ddress( uint8_t static_address, uint8_t dynamic_address )
{
	i3c.ccc_broadcast( CCC::BROADCAST_RSTDAA, NULL, 0 ); // Reset DAA
	i3c.ccc_set( CCC::DIRECT_SETDASA, static_address, dynamic_address << 1 ); // Set Dynamic Address from Static Address
}

int DAA( const uint8_t *address_list, uint8_t list_length )
{
	i3c_device_info_t	*list_p;
	int					n_devices;

	i3c.ccc_broadcast( CCC::BROADCAST_RSTDAA, NULL, 0 ); // Reset DAA
	n_devices	= i3c.DAA( address_list, list_length, &list_p );

	printf( "\r\n=== DAA result: total %d target(s) found\r\n", n_devices );
	for ( int i = 0; i < n_devices; i++ )
	{
		printf( "Target %d\r\n", i );
		printf( "  dynamicAddr    = 0x%02X\r\n",  list_p[ i ].dynamicAddr    );
		printf( "  staticAddr     = 0x%02X\r\n",  list_p[ i ].staticAddr     );
		printf( "  dcr            = 0x%02X\r\n",  list_p[ i ].dcr            );
		printf( "  bcr            = 0x%02X\r\n",  list_p[ i ].bcr            );
		printf( "  vendorID       = 0x%04X\r\n",  list_p[ i ].vendorID       );
		printf( "  partNumber     = 0x%08lX\r\n", list_p[ i ].partNumber     );
		printf( "  maxReadLength  = 0x%04X\r\n",  list_p[ i ].maxReadLength  );
		printf( "  maxWriteLength = 0x%04X\r\n",  list_p[ i ].maxWriteLength );
		printf( "  hdrMode        = 0x%02X\r\n",  list_p[ i ].hdrMode        );
		printf( "\r\n" );
	}

	return n_devices;
}

bool check_i2c_device(  LM75B ts )
{
	uint8_t	dummy	= 0;

	i3c.mode( I3C::I2C_MODE );
	int	r	= i3c.write( ts.address(), &dummy, 1 );
	i3c.mode( I3C::I3C_MODE );

	return !r;
}

void info( LM75B ts )
{
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
	uint8_t		pid[ I3C::PID_LENGTH ];
	uint8_t		bcr, dcr;

	uint8_t		a	= ts.address();
	uint16_t	t	= ts.read_r16( P3T1755::Temp   );
	uint8_t		c	= ts.read_r8(  P3T1755::Conf   );
	uint16_t	l	= ts.read_r16( P3T1755::T_LOW  );
	uint16_t	h	= ts.read_r16( P3T1755::T_HIGH );

	ts.ccc_get( CCC::DIRECT_GETPID, pid, sizeof( pid ) );
	ts.ccc_get( CCC::DIRECT_GETBCR, &bcr, 1 );
	ts.ccc_get( CCC::DIRECT_GETDCR, &dcr, 1 );

	PRINTF( "\r\nRegister dump - I3C target address:7’h%02X (0x%02X)\r\n", a, a << 1 );
	PRINTF( "  - Temp   (0x0): 0x%04X (%8.4f˚C)\r\n", t, (float)t / 256.0 );
	PRINTF( "  - Conf   (0x1): 0x  %02X\r\n", c );
	PRINTF( "  - T_LOW  (0x2): 0x%04X (%8.4f˚C)\r\n", l, (float)l / 256.0 );
	PRINTF( "  - T_HIGH (0x3): 0x%04X (%8.4f˚C)\r\n", h, (float)h / 256.0 );

	PRINTF( "  * PID    (CCC:Provisioned ID)                 : 0x" );
	for ( int i = 0; i < I3C::PID_LENGTH; i++ ) PRINTF( " %02X", pid[ i ] );	PRINTF( "\r\n" );
	PRINTF( "  * BCR    (CCC:Bus Characteristics Register)   : 0x%02X\r\n", bcr );
	PRINTF( "  * DCR    (CCC:Device Characteristics Register): 0x%02X (= %s)\r\n", dcr, (0x63 == dcr) ? "Temperature sensor" : "Unknown" );

	PRINTF( "\r\n" );
}


