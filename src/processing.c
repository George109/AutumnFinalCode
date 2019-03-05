/*
 * processing.c
 *
 *  Created on: 6 Nov 2018
 *      Author: Yihang GUO
 */

#include "processing.h"


void fft_generate_test_data(float32_t*testInput, int buflen, float32_t frequency)
	 {
	 	uint16_t i;
	 	for(i = 0; i < buflen; i++)
	 	{
	 		// Signal, plus DC offset
	 		testInput[i] = 1 + sin(i*frequency*2.0*M_PI/4000);
	 		// Small amount of noise
	 		//buf[i] += ((float32_t)rand()/(float32_t)RAND_MAX);
	 	}
	 }



int ADC_speedCalculation(float32_t ADCvalue[]){
	//float32_t input[1024];
	float32_t output[2048]={0};
	float32_t realOutput[1024]={0};

	float32_t maxvalue = 0;
	uint32_t maxindex = 0;

	float32_t frequency = 0;
	float32_t speed_ms = 0;

	int speed = 0;
	int i = 0;

	//int frq = 917;
	//int frq = 500;
	//int frq = 1800;

	arm_rfft_fast_instance_f32 data;
	data.pTwiddleRFFT = twiddleCoef_rfft_1024;
	arm_rfft_fast_init_f32(&data,1024);


	//fft_generate_test_data(input, 1024, 978);
	//fft_generate_test_data(input, 1024, frq);


	arm_rfft_fast_f32(&data, ADCvalue, output, 0);
	arm_cmplx_mag_f32(output, realOutput, 1024);

	//special case init
	for (i = 0; i < 2; i++ ){
		realOutput[i] = 0;
	}
	realOutput[512] = 0;


	arm_max_f32(realOutput, 1024, &maxvalue, &maxindex);

	//frequency = (float32_t)maxindex * 4000 / 1024; //for the sampling frequency of 4000 Hz

	frequency = (float32_t)maxindex * 40000 / 1024; //80 MHz/(200*10) = 40000

	speed_ms = frequency * 300000000 / (2 * 10587000000);

	speed = (int) speed_ms;

	return speed;
}
