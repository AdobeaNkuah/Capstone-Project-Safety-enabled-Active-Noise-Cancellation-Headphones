// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011

///////////////////////////////////////////////////////////////////////
// Filename: StartUp.c
//
// Synopsis: Placeholder for code run after DSP_Init()
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"

#define N 80
extern float w[N];
extern float x_test_buffer[N];
extern float x_E_buffer[N];


void StartUp()
{
    // initialize DSP board
    DSP_Init();
	InitDigitalOutputs();

	int i = 0;

	for (i = 0; i < N; i++)
	{
	    w[i] = 0.0;
	    x_test_buffer[i] = 0.0;
	    x_E_buffer[i] = 0.0;
	}
}
