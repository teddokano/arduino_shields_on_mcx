/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"I2C_device.h"
#include	"rtc/PCF2131.h"
#include	"rtc/RTC_NXP.h"

#define		USE_I2C

#ifdef	USE_I2C
I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
uint8_t 	i2c_address = (0xA6 >> 1);
I2C_device	device( i2c, i2c_address );
#define	INTF_STR	"I2C"
#else
SPI			spi( ARD_MOSI, ARD_MISO, ARD_SCK, ARD_CS );	//	MOSI, MISO, SCLK, CS
SPI_for_RTC	device( spi );
#define	INTF_STR	"SPI"
#endif

void	set_time( void );
void	show_time( void );
uint8_t	bcd2dec( uint8_t v );
uint8_t	dec2bcd( uint8_t v );

int main( void )
{
	printf( "***** Hello, PCF2131! (%s interface) *****\r\n", INTF_STR );

	if ( device.reg_r( PCF2131::Seconds ) & 0x80 )
	{
		printf( "==== oscillator_stop detected :( ====\r\n" );
		set_time();
	}
	else
	{
		printf( "---- RTC has beeing kept running! :) ----\r\n" );
	}

	while ( true )
	{
		show_time();
		wait( 1 );
	}
}

void	show_time( void )
{
	static const char	*month_name[]	= {
					"January", "February", "March", "April", "May", "June",
					"July", "August", "September", "October", "November", "December"
				};

	uint8_t		bf[ 8 ];

	device.reg_r( PCF2131::_100th_Seconds, bf, sizeof( bf ) );

	for ( unsigned int i = 0; i < sizeof( bf ); i++ )
		bf[ i ]	= bcd2dec( bf[ i ] );

	printf( "%4d-%s-%2d, %02d:%02d:%02d\r\n",  bf[ 7 ] + 2000, month_name[ bf[ 6 ] - 1 ], bf[ 4 ], bf[ 3 ], bf[ 2 ], bf[ 1 ] );
}

void set_time( void ) {
	uint8_t		bf[ 8 ];

	bf[ 0 ]	= dec2bcd(  0 );			//	1/00 sec
	bf[ 1 ]	= dec2bcd( 55 );			//	sec
	bf[ 2 ]	= dec2bcd( 59 );			//	min
	bf[ 3 ]	= dec2bcd( 23 );			//	hour
	bf[ 4 ]	= dec2bcd( 20 );			//	day
	bf[ 6 ]	= dec2bcd( 12 );			//	month (1 ~ 12)
	bf[ 7 ]	= dec2bcd( 2024 - 2000 );	//	year

	device.bit_op8( PCF2131::Control_1, ~0x28, 0x20 );
	device.bit_op8( PCF2131::SR_Reset,  (uint8_t)(~0x80), 0x80 );

	device.reg_w( PCF2131::_100th_Seconds, bf, sizeof( bf ) );

	device.bit_op8( PCF2131::Control_1, ~0x20, 0x00 );
}

uint8_t	bcd2dec( uint8_t v )
{
	return (v >> 4) * 10 + (v & 0x0F);
}

uint8_t	dec2bcd( uint8_t v )
{
	return ((v / 10) << 4) + (v % 10);
}
