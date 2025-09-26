/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"lcd/PCA8561.h"

I2C			i2c( I2C_SDA, I2C_SCL );
PCA8561		lcdd( i2c );

void	test_putchar( void );
void	test_puts( void );
void	test_puts_with_delay( void );
void	test_COM_SEG_access( void );
void	test_register_access( void );

int main(void)
{
	PRINTF("\r***** Hello, PCA8561! *****\r\n");

	I2C_device::scan( i2c, 124 ); //  Scan stop at 124

	while ( true )
	{
		test_putchar();
		test_puts();
		test_puts_with_delay();
		test_COM_SEG_access();
		test_register_access();
	}
}

void test_putchar( void )
{
	for ( int i = 0; i < 10; i++ )
	{
		lcdd.putchar(i + '0');
		wait( 0.2 );
	}

	lcdd.clear();
}

void test_puts( void )
{
	const char* test_str[4] = {
		"A\n",
		"BB\n",
		"CCC\n",
		"DDDD\n",
	};

	for ( int i = 0; i < 4; i++ )
	{
		lcdd.puts( test_str[ i ] );
		wait( 0.5 );
	}
}

void test_puts_with_delay( void )
{
	lcdd.puts( "    ABDCEFGHIJKLMNOPQRSTUVWXYZ 0123456789    ", 100 );
}

void test_COM_SEG_access( void )
{
	for ( int b = 1; b < 5; b++ )
	{
		bool on_off = b % 2;

		for (int c = 0; c < PCA8561::N_COM; c++)
		{
			for (int s = 0; s < PCA8561::N_SEG; s++)
			{
				lcdd.com_seg( c, s, on_off );
				wait( 0.01 );
			}
		}
	}
}

void test_register_access( void )
{
	const int	size	= 12;
	uint8_t		test_bf[ 2 ][ size ];

	for ( int i = 0; i < 2; i++ )
		for ( int j = 0; j < size; j++ )
			test_bf[ i ][ j ]	= i ? 0xFF : 0x00;

	for ( int i = 0; i < 6; i++ ) {
		lcdd.reg_w( PCA8561::COM0_07_00, test_bf[ i % 2 ], size );
		wait( 0.2 );
	}
}
