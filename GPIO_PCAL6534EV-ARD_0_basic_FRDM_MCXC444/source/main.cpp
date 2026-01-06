/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6534.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
uint8_t 	i2c_address = (0x44 >> 1);

DigitalOut	reset_pin( D8, 0 );
DigitalOut	addr_pin(  D9, 0 );

int main(void)
{
	PRINTF( "***** Hello, PCAL6534! *****\r\n" );
	printf( "  Find the LEDs counting in binary.\r\n" );
	printf( "  And press the buttons to change port read values shown in serial terminal.\r\n" );

	reset_pin	= 1;

	I2C_device::scan( i2c );

	printf( "\r\nregister addresses\r\n" );
	printf( "  PCAL6534::Input_Port_3          = 0x%02X\r\n", PCAL6534::Input_Port_3         );
	printf( "  PCAL6534::Input_Port_4          = 0x%02X\r\n", PCAL6534::Input_Port_4         );
	printf( "  PCAL6534::Output_Port_2         = 0x%02X\r\n", PCAL6534::Output_Port_2        );
	printf( "  PCAL6534::Configuration_port_2  = 0x%02X\r\n", PCAL6534::Configuration_port_2 );
	printf( "  PCAL6534::Configuration_port_3  = 0x%02X\r\n", PCAL6534::Configuration_port_3 );
	printf( "  PCAL6534::Configuration_port_4  = 0x%02X\r\n", PCAL6534::Configuration_port_4 );
	printf( "  PCAL6534::Pull_up_pull_down_selection_register_port_3 = 0x%02X\r\n", PCAL6534::Pull_up_pull_down_selection_register_port_3 );
	printf( "  PCAL6534::Pull_up_pull_down_selection_register_port_4 = 0x%02X\r\n", PCAL6534::Pull_up_pull_down_selection_register_port_4 );
	printf( "  PCAL6534::Pull_up_pull_down_enable_register_port_3    = 0x%02X\r\n", PCAL6534::Pull_up_pull_down_enable_register_port_3    );
	printf( "  PCAL6534::Pull_up_pull_down_enable_register_port_4    = 0x%02X\r\n", PCAL6534::Pull_up_pull_down_enable_register_port_4    );
	printf( "\r\n" );

	i2c.reg_write( i2c_address, PCAL6534::Configuration_port_2, 0x00 );	// Configure port2 as OUTPUT
	i2c.reg_write( i2c_address, PCAL6534::Configuration_port_3, 0xE0 );	// Configure port3 bit 7~5 as INPUT
	i2c.reg_write( i2c_address, PCAL6534::Configuration_port_4, 0x03 );	// Configure port4 bit 1 and 0 as INPUT

	i2c.reg_write( i2c_address, PCAL6534::Pull_up_pull_down_selection_register_port_3, 0xE0 );	// Pull-up selected for port3 bit 7~5
	i2c.reg_write( i2c_address, PCAL6534::Pull_up_pull_down_selection_register_port_4, 0x03 );	// Pull-up selected for port4 bit 7~5
	i2c.reg_write( i2c_address, PCAL6534::Pull_up_pull_down_enable_register_port_3,    0xE0 );	// Pull-up/down enabled for port2 bit 1 and 0
	i2c.reg_write( i2c_address, PCAL6534::Pull_up_pull_down_enable_register_port_4,    0x03 );	// Pull-up/down enabled for port2 bit 1 and 0

	uint8_t	p3, p4;

	while ( true )
	{
		static int count = 0;

		i2c.reg_write( i2c_address, PCAL6534::Output_Port_2, count++ );

		p3	= i2c.reg_read( i2c_address, PCAL6534::Input_Port_3 );
		p4	= i2c.reg_read( i2c_address, PCAL6534::Input_Port_4 );

		printf( "port3 and port4 : 0x%02X 0x%02X\r\n", p3, p4 );
		wait( 0.2 );
	}
}
