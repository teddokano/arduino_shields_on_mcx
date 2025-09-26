/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"pcal9722_registers.h"

static constexpr int	RESET_PIN	= D8;
static constexpr int	 ADDR_PIN	= D9;
static constexpr int	 MOSI_PIN	= D11;
static constexpr int	 MISO_PIN	= D12;
static constexpr int	 SCLK_PIN	= D13;
static constexpr int	   CS_PIN	= D10;

SPI		spi( ARD_MOSI, ARD_MISO, ARD_SCK, ARD_CS );	//	MOSI, MISO, SCLK, CS

uint8_t	hardware_reset( bool address_setting );
void	register_write( uint8_t dev_addr, uint8_t reg_addr, uint8_t value );
uint8_t	register_read( uint8_t dev_addr, uint8_t reg_addr );

int main( void )
{
	printf( "***** Hello, PCAL9722! *****\r\n" );
	printf( "  Find 8 LEDs are counting in binary\r\n" );
	printf( "  Press button and watch 7-segment LED when detects its input\r\n" );

	uint8_t	dev_addr	= hardware_reset( 0 );

	register_write( dev_addr, Configuration_port_0, 0x00 );	//	port 0 as output
	register_write( dev_addr, Configuration_port_1, 0x00 );	//	port 1 as output
	register_write( dev_addr, Configuration_port_2, 0x3F );	//	port 2 as input

	register_write( dev_addr, Pull_up_pull_down_enable_register_port_2,    0x3F );	//	port 2 pull-up/down enable
	register_write( dev_addr, Pull_up_pull_down_selection_register_port_2, 0x3F );	//	port 2 select pull-up

	uint8_t		count	= 0;
	uint8_t		in;

	while ( true )
	{
		in	= register_read( dev_addr, Input_Port_2 );

		if ( ~in & 0x3F )
			printf( "port2 = 0x%02X\r\n", in );

		register_write( dev_addr, Output_Port_1, in | 0xC0 );
		register_write( dev_addr, Output_Port_0, count++ );

		wait( 0.1 );
	}
}

void register_write( uint8_t dev_addr, uint8_t reg_addr, uint8_t value )
{
	uint8_t	w_data[ 3 ];
	uint8_t	r_data[ 3 ];

	w_data[ 0 ]	= dev_addr << 1;
	w_data[ 1 ]	= reg_addr;
	w_data[ 2 ]	= value;

	spi.write( w_data, r_data, 3 );
}

uint8_t register_read( uint8_t dev_addr, uint8_t reg_addr )
{
	uint8_t	w_data[ 3 ];
	uint8_t	r_data[ 3 ];

	w_data[ 0 ]	= (dev_addr << 1) | 0x1;
	w_data[ 1 ]	= reg_addr;
	w_data[ 2 ]	= 0;

	spi.write( w_data, r_data, 3 );

	return r_data[ 2 ];
}

uint8_t hardware_reset( bool address_setting )
{
	DigitalOut	reset( RESET_PIN, 1 );
	DigitalOut	addr(  ADDR_PIN,  address_setting );

	reset	= 0;
	wait( 0.001 );
	reset	= 1;

	return (0x40 >> 1) + address_setting;
}
