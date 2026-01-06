/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"rtc/PCF2131.h"
#include	"rtc/tzcode/strptime.h"
#include	<time.h>

#define		USE_I2C

#ifdef	USE_I2C
	I2C		interface( I2C_SDA, I2C_SCL );	//	SDA, SCL
	#define	INTF_STR	"I2C"
#else
	SPI		interface( ARD_MOSI, ARD_MISO, ARD_SCK, ARD_CS );	//	MOSI, MISO, SCLK, CS
	#define	INTF_STR	"SPI"
#endif

PCF2131		rtc( interface );

InterruptIn	intA( D8 );
InterruptIn	intB( D9 );

bool int_flagA	= false;
bool int_flagB	= false;

void	int_cause_monitor( uint8_t* status );
void	pin_int_callbackA( void );
void	pin_int_callbackB( void );
void	set_time( void );

int main( void )
{
	printf( "***** Hello, PCF2131! (%s interface) *****\r\n", INTF_STR );

	if ( rtc.oscillator_stop() )
	{
		printf( "==== oscillator_stop detected :( ====\r\n" );
		set_time();
	}
	else
	{
		printf( "---- RTC has beeing kept running! :) ----\r\n" );
	}

	rtc.int_clear();
	intA.fall( pin_int_callbackA );
	intB.fall( pin_int_callbackB );

	rtc.timestamp( 1, PCF2131::LAST, PCF2131::INT_B );
	rtc.timestamp( 2, PCF2131::LAST, PCF2131::INT_B );
	rtc.timestamp( 3, PCF2131::LAST, PCF2131::INT_B );
	rtc.timestamp( 4, PCF2131::LAST, PCF2131::INT_B );

	rtc.periodic_interrupt_enable( PCF2131::EVERY_SECOND, PCF2131::INT_A );
	rtc.alarm( PCF2131::SECOND, 37, PCF2131::INT_B );
	
	while ( true )
	{
		if ( int_flagA || int_flagB )
		{
			if ( int_flagA )
			{
				int_flagA	= false;
				printf( "[INT-A] " );
			}
			if ( int_flagB )
			{
				int_flagB	= false;
				printf( "[INT-B] " );
			}

			uint8_t status[3];
			rtc.int_clear( status );
			int_cause_monitor( status );
		}
	}
}

void int_cause_monitor( uint8_t* status )
{
	printf( "status:" );

	for (int i = 0; i < 3; i++)
		printf( " %02X", status[i] );

	printf( ", " );

	if ( status[0] & 0x80 )
	{
		time_t current_time = rtc.time( NULL );
		printf( "INT:every min/sec, time:%lu %s", (unsigned long)current_time, ctime( &current_time ) );
	}
	if ( status[0] & 0x40 )
	{
		printf( "INT:watchdog" );
	}
	if ( status[0] & 0x10 )
	{
		printf( "INT:alarm " );
		printf( "########## ALARM ########## \r\n" );
	}
	if ( status[1] & 0x08 )
	{
		printf( "INT:battery switch over" );
	}
	if ( status[1] & 0x04 )
	{
		printf( "INT:battery low" );
	}
	if ( status[2] & 0xF0 )
	{
		for ( int i = 0; i < 4; i++ )
			if ( status[2] & (0x80 >> i) )
				printf( "INT:timestamp%d\r\n", i + 1 );

		for ( int i = 0; i < 4; i++ )
		{
			time_t ts = rtc.timestamp(i + 1);
			printf( "  TIMESTAMP%d: %s", i + 1, ctime( &ts ) );
		}
	}
}

void pin_int_callbackA( void ) {
  int_flagA	= true;
}

void pin_int_callbackB( void ) {
  int_flagB	= true;
}

#define	TIMEZONE		+9
#define	DATETIME_FORMAT	"%Y-%m-%d %H:%M:%S"

void set_time( void ) {
	struct tm	now_tm;

#ifdef SEMIHOST_OPERATION

	#define	CURRENT_TIME_INIT_MESSAGE	"from PC using \"time.h\" API via semihost"

	char current_time[64];
	time_t	t = time(NULL) + ((TIMEZONE) * 3600);
	strftime( current_time, sizeof( current_time ), DATETIME_FORMAT, gmtime( &t ) );

	gmtime_r( &t, &now_tm );

#else

	#define	DATETIME_FOR_INITIALIZE		"2024-12-20 23:59:30"
	#define	DATETIME_FORMAT				"%Y-%m-%d %H:%M:%S"
	#define	CURRENT_TIME_INIT_MESSAGE	"from \"DATETIME_FOR_INITIALIZE\" in source code since no current datetime information source available"

	const char* current_time	= DATETIME_FOR_INITIALIZE;
	const char* format			= DATETIME_FORMAT;

	strptime( current_time, format, &now_tm );

#endif

	rtc.set( &now_tm );

	printf( "RTC got new time information %s\r\n", CURRENT_TIME_INIT_MESSAGE );
	printf( "  date and time is initialized to %s\r\n", current_time );
}
