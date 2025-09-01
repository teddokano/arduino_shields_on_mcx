/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#include	"ldo/PCA9617ADP_ARD_LDO.h"

Pca9617adp_Ard_LDO::Pca9617adp_Ard_LDO(	int ldo_number, int pin0, int pin1, int pin2, float *v_v ) 
	: BusInOut( pin0, pin1, pin2 ), num( ldo_number ), v_values( v_v )
{
	config( OUTPUT );
}
Pca9617adp_Ard_LDO::~Pca9617adp_Ard_LDO(){}

float Pca9617adp_Ard_LDO::voltage( void )
{
	return v_values[ *this ];
}

float Pca9617adp_Ard_LDO::voltage( int v )
{
	return v_values[ v ];
}

Pca9617adp_Ard_LDO1::Pca9617adp_Ard_LDO1( int ldo_number, int pin0, int pin1, int pin2, float *v_v )
	: Pca9617adp_Ard_LDO( ldo_number, pin0, pin1, pin2, v_v ){}
Pca9617adp_Ard_LDO1::~Pca9617adp_Ard_LDO1(){};

Pca9617adp_Ard_LDO2::Pca9617adp_Ard_LDO2( int ldo_number, int pin0, int pin1, float *v_v )
	: Pca9617adp_Ard_LDO( ldo_number, pin0, pin1, BusInOut::nc, v_v ){}
Pca9617adp_Ard_LDO2::~Pca9617adp_Ard_LDO2(){}

float	Pca9617adp_Ard_LDO::v1_values[ v1_variation ] = { 1.8, 2.5, 3.3, 4.96, 0.8 };
float	Pca9617adp_Ard_LDO::v2_values[ v2_variation ] = { 2.5, 3.0, 3.3, 4.96 };
