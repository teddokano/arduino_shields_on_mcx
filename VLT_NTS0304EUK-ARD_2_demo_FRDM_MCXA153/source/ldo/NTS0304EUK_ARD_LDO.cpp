/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"ldo/NTS0304EUK_ARD_LDO.h"

Nts0304euk_Ard_LDO::Nts0304euk_Ard_LDO(	int ldo_number, int pin0, int pin1, int pin2, float *v_v ) 
	: BusOut( pin0, pin1, pin2 ), num( ldo_number ), v_values( v_v ){}
Nts0304euk_Ard_LDO::~Nts0304euk_Ard_LDO(){}

float Nts0304euk_Ard_LDO::voltage( void )
{
	return v_values[ *this ];
}

float Nts0304euk_Ard_LDO::voltage( int v )
{
	return v_values[ v ];
}

Nts0304euk_Ard_LDO1::Nts0304euk_Ard_LDO1( int ldo_number, int pin0, int pin1, int pin2, float *v_v )
	: Nts0304euk_Ard_LDO( ldo_number, pin0, pin1, pin2, v_v ){}
Nts0304euk_Ard_LDO1::~Nts0304euk_Ard_LDO1(){};

Nts0304euk_Ard_LDO2::Nts0304euk_Ard_LDO2( int ldo_number, int pin0, int pin1, float *v_v )
	: Nts0304euk_Ard_LDO( ldo_number, pin0, pin1, BusInOut::nc, v_v ){}
Nts0304euk_Ard_LDO2::~Nts0304euk_Ard_LDO2(){}

float	Nts0304euk_Ard_LDO::v1_values[ v1_variation ] = { 1.2, 1.8, 2.5, 3.3, 0.95 };
float	Nts0304euk_Ard_LDO::v2_values[ v2_variation ] = { 1.8, 2.5, 3.3, 4.96 };
