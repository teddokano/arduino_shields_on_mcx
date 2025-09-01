/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"r01lib.h"
#include	"pin_control.h"

extern "C" {
#include	"fsl_utick.h"
//#include	"pwm.h"
}

#define	TRIGGER_PORALITY	0

extern	DigitalOut	r;
extern	DigitalOut	g;
extern	DigitalOut	b;
extern	DigitalOut	trig;
DigitalOut			*target_ptr;

void led_control_callback( void );

void init_pin_control( void )
{
	target_ptr	= &b;
	
//	pwm_start();
	
	Ticker	t;
	t.attach( led_control_callback, 0.01 );
}


void led_control_callback( void )
{
	static int	count	= 0;

#if 0
	int			c200;
	c200	= (&b == target_ptr) ? count % 200 : 100;
	pwm_update( (100 < c200) ? 200 - c200 : c200 );
#endif
	
	led_pin_control( count );
	
	count++;
}

void led_set_color( float temp, float ref )
{
	if ( (ref + 2) < temp )
		target_ptr	= &r;
	else if ( (ref + 1) < temp )
		target_ptr	= &g;
	else
		target_ptr	= &b;
	
	trig	= ~TRIGGER_PORALITY;
}

void led_all( bool v )
{
	r		= v;
	g	= v;
	b	= v;
}

void led_pin_control( int v )
{
	static const int k	= 32;
	
	v	%= k;
	if ( v < (k - 5) )
		led_all( PIN_LED_OFF );
	else
		*target_ptr	= PIN_LED_ON;
	
//	PRINTF( "### %d\r\n", v );
}

void ibi_trigger_output( void )
{
	trig	= TRIGGER_PORALITY;
}
