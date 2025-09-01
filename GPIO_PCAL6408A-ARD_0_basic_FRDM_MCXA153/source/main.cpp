/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6408A.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
uint8_t 	i2c_address = (0x40 >> 1);

DigitalOut	reset_pin( D8, 0 );
DigitalOut	addr_pin(  D9, 0 );

int main(void)
{
	printf( "***** Hello, PCAL6408A! *****\r\n" );
	printf( "  Find the LEDs counting in binary.\r\n" );
	printf( "  And press the buttons to change port read value shown in serial terminal.\r\n" );
	
	reset_pin	= 1;

	I2C_device::scan( i2c );

	printf( "\r\nregister addresses\r\n" );
	printf( "  PCAL6408A::Input_Port    = 0x%02X\r\n", PCAL6408A::Input_Port    );
	printf( "  PCAL6408A::Output_Port   = 0x%02X\r\n", PCAL6408A::Output_Port   );
	printf( "  PCAL6408A::Configuration = 0x%02X\r\n", PCAL6408A::Configuration );
	printf( "  PCAL6408A::Pull_up_pull_down_enable    = 0x%02X\r\n", PCAL6408A::Pull_up_pull_down_enable    );
	printf( "  PCAL6408A::Pull_up_pull_down_selection = 0x%02X\r\n", PCAL6408A::Pull_up_pull_down_selection );
	printf( "\r\n" );

	i2c.reg_write( i2c_address, PCAL6408A::Configuration, 0xF0 );	// Configure port0 bit 7~4 as INTPUT

	i2c.reg_write( i2c_address, PCAL6408A::Pull_up_pull_down_selection, 0xF0 );	// Pull-up selected for port0 bit 7~4
	i2c.reg_write( i2c_address, PCAL6408A::Pull_up_pull_down_enable,    0xF0 );	// Pull-up/down enabled for port0 bit 7~4

	uint8_t	p0;

	while ( true )
	{
		static int count = 0;

		i2c.reg_write( i2c_address, PCAL6408A::Output_Port, count++ );

		p0	= i2c.reg_read( i2c_address, PCAL6408A::Input_Port );

		printf( "port0 : 0x%02X\r\n", p0 );
		wait( 0.2 );
	}
}
