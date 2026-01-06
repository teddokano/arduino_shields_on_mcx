/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"led/PCA9955B.h"

constexpr	int	cycle	= 100;

I2C			i2c( I2C_SDA, I2C_SCL );
PCA9955B	ledd( i2c );

int main( void )
{
	printf("***** Hello, PCA9955B! *****\r\n");

	ledd.begin( 1.0, PCA9955B::ARDUINO_SHIELD );

	while ( true )
	{
		for ( int ch = 0; ch < 16; ch++ )
		{
			for ( int i = 1; i <= cycle; i++ )
			{
				ledd.pwm( ch, (float)i / (float)cycle );
				wait( 0.01 );
			}

			for ( int i = cycle; i >= 0; --i )
			{
				ledd.pwm( ch, (float)i / (float)cycle );
				wait( 0.01 );
			}
		}
	}
}
