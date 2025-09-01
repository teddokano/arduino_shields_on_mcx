/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"gpio/PCAL6416A.h"

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
uint8_t 	i2c_address = (0x40 >> 1);

DigitalOut	reset_pin( D8, 0 );
DigitalOut	addr_pin(  D9, 0 );

int main(void)
{
	printf( "***** Hello, PCAL6416A! *****\r\n" );
	printf( "  Find the LEDs counting in binary.\r\n" );
	printf( "  And press the buttons to change port read values shown in serial terminal.\r\n" );

	reset_pin	= 1;

	I2C_device::scan( i2c );

	printf( "\r\nregister addresses\r\n" );
	printf( "  PCAL6416A::Input_Port_1          = 0x%02X\r\n", PCAL6416A::Input_Port_1          );
	printf( "  PCAL6416A::Output_Port_0         = 0x%02X\r\n", PCAL6416A::Output_Port_0         );
	printf( "  PCAL6416A::Configuration_port_0  = 0x%02X\r\n", PCAL6416A::Configuration_port_0  );
	printf( "  PCAL6416A::Configuration_port_1  = 0x%02X\r\n", PCAL6416A::Configuration_port_1  );
	printf( "  PCAL6416A::Pull_up_pull_down_selection_register_1 = 0x%02X\r\n", PCAL6416A::Pull_up_pull_down_selection_register_1  );
	printf( "  PCAL6416A::Pull_up_pull_down_enable_register_1    = 0x%02X\r\n", PCAL6416A::Pull_up_pull_down_enable_register_1     );
	printf( "\r\n" );

	i2c.reg_write( i2c_address, PCAL6416A::Configuration_port_0, 0x00 );	// Configure port0 as OUTPUT
	i2c.reg_write( i2c_address, PCAL6416A::Configuration_port_1, 0xFF );	// Configure port1 as INPUT

	i2c.reg_write( i2c_address, PCAL6416A::Pull_up_pull_down_selection_register_1, 0xFF );	// Pull-up selected for port1
	i2c.reg_write( i2c_address, PCAL6416A::Pull_up_pull_down_enable_register_1,    0xFF );	// Pull-up/down enabled for port1

	uint8_t	p0, p1;

	while ( true )
	{
		static int count = 0;

		i2c.reg_write( i2c_address, PCAL6416A::Output_Port_0, count++ );

		p0	= i2c.reg_read( i2c_address, PCAL6416A::Input_Port_0 );
		p1	= i2c.reg_read( i2c_address, PCAL6416A::Input_Port_1 );

		printf( "port0 and port1 : 0x%02X 0x%02X\r\n", p0, p1 );
		wait( 0.2 );
	}
}
