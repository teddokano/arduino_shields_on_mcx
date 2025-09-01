/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"rtc/PCF85053A.h"
#include	"rtc/tzcode/strptime.h"
#include	<time.h>

I2C			i2c( I2C_SDA, I2C_SCL );	//	SDA, SCL
PCF85053A	rtc( i2c );

void	set_time( void );

int main( void )
{
	printf( "***** Hello, PCF85053A! *****\r\n" );

	if ( rtc.oscillator_stop() )
	{
		printf( "==== oscillator_stop detected :( ====\r\n" );
		set_time();
	}
	else
	{
		printf( "---- RTC has beeing kept running! :) ----\r\n" );
	}

	time_t	current_time;

	while ( true )
	{
		current_time	= rtc.time( NULL );

		printf( "time : %lu, %s", (unsigned long)current_time, ctime( &current_time ) );
		wait( 1 );
	}
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
