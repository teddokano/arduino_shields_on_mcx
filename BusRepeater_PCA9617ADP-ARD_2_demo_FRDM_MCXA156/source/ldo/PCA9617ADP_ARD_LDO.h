/*
 *  @author Tedd OKANO
 *
 *  Released under the MIT license
 */

#ifndef ARDUINO_PCA9617ADP_ARD_LDO_H
#define ARDUINO_PCA9617ADP_ARD_LDO_H

#include	"r01lib.h"
#include	"BusInOut.h"
#include	<stdint.h>

/** Pca9617adp_Ard_LDO class
 *	
 *  @class Pca9617adp_Ard_LDO
 */

class Pca9617adp_Ard_LDO : public BusInOut {
public:
	using BusInOut::operator=;
	
	/** Create a Pca9617adp_Ard_LDO instance with specified pins
	 *
	 * @param ldo_number 1 or 2 to specify the LDO
	 */
	Pca9617adp_Ard_LDO(	int ldo_number, int pin0, int pin1, int pin2, float *v_v );
	~Pca9617adp_Ard_LDO();

	/** Get Pca9617adp_Ard_LDO output voltage
	 *
	 * @return voltage
	 */
	float	voltage( void );

	/** Get Pca9617adp_Ard_LDO output voltage
	 *
	 * @param v setting value but no output will be changed
	 * @return voltage
	 */
	float	voltage( int v );

	constexpr static int v1_variation = 5;
	constexpr static int v2_variation = 4;
	
	static float	v1_values[ v1_variation ];
	static float	v2_values[ v2_variation ];
	
private:
	int				num;
	float			*v_values;

};

class Pca9617adp_Ard_LDO1 : public Pca9617adp_Ard_LDO {
public:
	using Pca9617adp_Ard_LDO::operator=;

	Pca9617adp_Ard_LDO1( int ldo_number = 1, int pin0 = D5, int pin1 = D4, int pin2 = D3, float *v_v = v1_values );
	~Pca9617adp_Ard_LDO1();
};

class Pca9617adp_Ard_LDO2 : public Pca9617adp_Ard_LDO {
public:
	using Pca9617adp_Ard_LDO::operator=;

	Pca9617adp_Ard_LDO2( int ldo_number = 2, int pin0 = D2, int pin1 = D1, float *v_v = v2_values );
	~Pca9617adp_Ard_LDO2();
};

#endif //	ARDUINO_PCA9617ADP_ARD_LDO_H
