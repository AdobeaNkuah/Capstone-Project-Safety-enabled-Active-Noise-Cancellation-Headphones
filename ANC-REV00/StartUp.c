// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011

///////////////////////////////////////////////////////////////////////
// Filename: StartUp.c
//
// Synopsis: Initialize adaptive filter weights and delays
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"

#define N 30            //# of weights (coefficients)
extern float w[N];      //weights for adapt filter
extern float x_R_buffer[N];  //reference signal buffer
extern float x_E_buffer[N];  //error signal buffer

void StartUp()
{
    //initialize board
    DSP_Init();
	InitDigitalOutputs();

	//initialize ANC parameters
	int i = 0;
	for (i = 0; i < N; i++)     //init variables
	{
	    w[i] = 0.0;             //init weights of adaptive FIR
	    x_R_buffer[i] = 0.0;    //init reference and error buffers
	    x_E_buffer[i] = 0.0;
	}
}
