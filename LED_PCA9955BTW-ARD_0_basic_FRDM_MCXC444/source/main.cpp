/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"led/PCA9955B.h"

constexpr	int	cycle	= 100;

I2C			i2c( I2C_SDA, I2C_SCL );
uint8_t 	i2c_address = (0xBC >> 1);

DigitalOut	oe(  D8, 0 );	//	OE pin LOW
DigitalOut	rst( D9, 1 );	//	RESET pin HIGH

int main( void )
{
	printf("***** Hello, PCA9955B! *****\r\n\r\n");

	printf( "register addresses\r\n" );
	printf( "  PCA9955B::LEDOUT0 = 0x%02X\r\n", PCA9955B::LEDOUT0 );
	printf( "  PCA9955B::PWMALL  = 0x%02X\r\n", PCA9955B::PWMALL  );
	printf( "  PCA9955B::IREFALL = 0x%02X\r\n", PCA9955B::IREFALL );
	printf( "  PCA9955B::PWM0    = 0x%02X\r\n", PCA9955B::PWM0    );

	I2C_device::scan( i2c );
	
	uint8_t	init[]	= { 0xAA, 0xAA, 0xAA, 0xAA };

	i2c.reg_write( i2c_address, 0x80 | PCA9955B::LEDOUT0, init, sizeof( init ) );
	i2c.reg_write( i2c_address, PCA9955B::PWMALL,  0x00 );
	i2c.reg_write( i2c_address, PCA9955B::IREFALL, 0xFF );

	while ( true )
	{
		for ( int ch = 0; ch < 16; ch++ )
		{
			for ( int i = 0; i < 256; i++ )
			{
				i2c.reg_write( i2c_address, PCA9955B::PWM0 + ch, i );
				wait( 0.005 );
			}
			i2c.reg_write( i2c_address, PCA9955B::PWM0 + ch, 0x00 );
		}
	}
}
