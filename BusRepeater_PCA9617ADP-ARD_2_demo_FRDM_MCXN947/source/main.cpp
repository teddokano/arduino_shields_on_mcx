/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

/** PCA9617ADP-ARD demo sample
 *
 *  Simple sample to operate two LDOs to try signal voltage convination
 *  All possible convination is demonstrated with 0.8V, 1.8V, 2.5V, 3.3V, 4.96V at LDO1 and
 *  2.5V, 3.0V, 3.3V, 4.96V at LDO2 except condition of "LDO1=4.96 and LDO2=2.5V".
 *
 *  To perform sample I2C transfer, it uses M24C02 class library which is defined in "MUX_SW_NXP_Arduino" library.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCA9617A:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/ic-bus-repeaters-hubs-extenders/level-translating-fm-plus-ic-bus-repeater:PCA9617A
 */

#include	"r01lib.h"
#include	"ldo/PCA9617ADP_ARD_LDO.h"
#include	"misc/eeprom/M24C02.h"

I2C					i2c( I2C_SDA, I2C_SCL );
M24C02				eeprom( i2c );
Pca9617adp_Ard_LDO1	ldo1;
Pca9617adp_Ard_LDO2	ldo2;

#define READ_BUFFER_SIZE 128

void	try_eeprom_read( void );

int main( void )
{
	printf("***** Hello, PCA9617A! *****\r\n");
	i2c.scan();

	i2c.err_callback( NULL );	//	suppressing error message from I2C because M24C02 uses NACK as a protocol

	char test_str[] = "EEPROM is responding - Hello, PCA9617A Arduino shield evaluation board test program";

	printf( "test string: %s\r\n", test_str );
	printf( "test string length: %u\r\n", sizeof( test_str ) );

	eeprom.write( 0, (uint8_t *)test_str, sizeof( test_str ) );

	while ( true )
	{
		for (int v1 = 0; v1 < Pca9617adp_Ard_LDO::v1_variation; v1++)
		{
			ldo1 = v1;

			for (int v2 = 0; v2 < Pca9617adp_Ard_LDO::v2_variation; v2++)
			{
				ldo2 = v2;

				if ( (3 == v1) && (0 == v2) )
					 continue;

				printf( "Voltage settings: V1 = %4.2fV, V1 = %4.2fV\r\n", ldo1.voltage(), ldo2.voltage() );
				try_eeprom_read();

				wait( 1.0 );
			 }
		}		
	}
}

void try_eeprom_read( void )
{
	char	read_str[ READ_BUFFER_SIZE ];

	eeprom.read( 0, (uint8_t *)read_str, READ_BUFFER_SIZE );

	printf( "%s\r\n", read_str );
}
