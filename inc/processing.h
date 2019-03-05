/*
 * Processing.h
 *
 *  Created on: Nov 6, 2018
 *      Author: Yihang GUO
 */

#ifndef PROCESSING_H_
#define PROCESSING_H_


#include "stm32l476xx.h"
#define ARM_MATH_CM4
#include "arm_math.h"
#include "arm_const_structs.h"


void fft_generate_test_data(float32_t*testInput, int buflen, float32_t frequency);

int ADC_speedCalculation(float32_t ADCvalue[]);

#endif /* PROCESSING_H_ */
