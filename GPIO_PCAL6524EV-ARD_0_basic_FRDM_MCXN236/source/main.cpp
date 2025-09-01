/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6524.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
uint8_t 	i2c_address = (0x44 >> 1);

DigitalOut	reset_pin( D8, 0 );
DigitalOut	addr_pin(  D9, 0 );

int main(void)
{
	printf( "***** Hello, PCAL6524! *****\r\n" );
	printf( "  Find the LEDs counting in binary.\r\n" );
	printf( "  And press the buttons to change port read value shown in serial terminal.\r\n" );

	reset_pin	= 1;

	I2C_device::scan( i2c );

	printf( "\r\nregister addresses\r\n" );
	printf( "  PCAL6524::Input_Port_2          = 0x%02X\r\n", PCAL6524::Input_Port_2          );
	printf( "  PCAL6524::Output_Port_2         = 0x%02X\r\n", PCAL6524::Output_Port_2         );
	printf( "  PCAL6524::Configuration_port_0  = 0x%02X\r\n", PCAL6524::Configuration_port_0  );
	printf( "  PCAL6524::Configuration_port_1  = 0x%02X\r\n", PCAL6524::Configuration_port_1  );
	printf( "  PCAL6524::Configuration_port_2  = 0x%02X\r\n", PCAL6524::Configuration_port_2  );
	printf( "  PCAL6524::Pull_up_pull_down_selection_register_port_2 = 0x%02X\r\n", PCAL6524::Pull_up_pull_down_selection_register_port_2  );
	printf( "  PCAL6524::Pull_up_pull_down_enable_register_port_2    = 0x%02X\r\n", PCAL6524::Pull_up_pull_down_enable_register_port_2     );
	printf( "\r\n" );

	i2c.reg_write( i2c_address, PCAL6524::Configuration_port_0, 0x00 );	// Configure port0 as OUTPUT
	i2c.reg_write( i2c_address, PCAL6524::Configuration_port_1, 0x00 );	// Configure port1 as OUTPUT
	i2c.reg_write( i2c_address, PCAL6524::Configuration_port_2, 0xF0 );	// Configure port2 bit 7~4 as INTPUT

	i2c.reg_write( i2c_address, PCAL6524::Pull_up_pull_down_selection_register_port_2, 0xF0 );	// Pull-up selected for port2 bit 7~4
	i2c.reg_write( i2c_address, PCAL6524::Pull_up_pull_down_enable_register_port_2,    0xF0 );	// Pull-up/down enabled for port2 bit 7~4

	uint8_t	p2;

	while ( true )
	{
		static int count = 0;

		i2c.reg_write( i2c_address, PCAL6524::Output_Port_2, count++ );

		p2	= i2c.reg_read( i2c_address, PCAL6524::Input_Port_2 );

		printf( "port2 : 0x%02X\r\n", p2 );
		wait( 0.2 );
	}
}
