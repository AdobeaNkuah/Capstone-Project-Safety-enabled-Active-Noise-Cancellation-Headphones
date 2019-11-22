// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011
// Modified by Mark Wickert February 2012 to include GPIO ISR start/stop postings

///////////////////////////////////////////////////////////////////////
// Filename: ISRs_fir_float.c
//
// Synopsis: Interrupt service routine for codec data transmit/receive
//           floating point FIR filtering with coefficients in *.h file
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"

// Function Prototypes
long int rand_int(void);
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT  0
#define RIGHT 1

volatile union {
	Uint32 UINT;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;

// LMS filter parameters
#define beta 1e-9        // learning rate
#define alpha 0.7        // leakage factor (leaky LMS algorithm)
#define N 20

/* add any global variables here */
float x_L_buffer[N];       //buffer for left delay samples
float x_R_buffer[N];       //buffer for right delay samples

float w[N] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};             //buffer weights of adapt filter

interrupt void Codec_ISR()
///////////////////////////////////////////////////////////////////////
// Purpose:   Codec interface interrupt service routine  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     CheckForOverrun, ReadCodecData, WriteCodecData
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{                    
	/* add any local variables here */
//	WriteDigitalOutputs(1); // Write to GPIO J15, pin 6; begin ISR timing pulse
	int i;
	float resultL = 0; //initialize the left accumulator
	float resultR = 0; //initialize the right accumulator

 	if(CheckForOverrun())	// overrun error occurred (i.e. halted DSP)
		return;				// so serial port is reset to recover

 	CodecDataIn.UINT = ReadCodecData();		// get input data samples

	//Use the next line to noise test the filter
	x_L_buffer[0] = CodecDataIn.Channel[LEFT];
	x_R_buffer[0] = CodecDataIn.Channel[RIGHT];

	//Filtering using a 32-bit accumulator
	//Update filter history

    for (i = 0; i < N; i++)         //to calculate out of adapt FIR
        resultL += (w[i] * x_L_buffer[i]);  //output of adaptive filter

    for (i = N-1; i>= 0; i--)       //loop updates weights and delays
    {
        w[i] = alpha * w[i] + beta*x_L_buffer[0]*x_R_buffer[i];     //update weights with leaky algorithm
        x_L_buffer[i] = x_L_buffer[i-1];            //update delay samples
        x_R_buffer[i] = x_R_buffer[i-1];            //update delay samples
    }

	//Return 16-bit sample to DAC
	CodecDataOut.Channel[LEFT] = (short) resultL;
	// Copy Right input directly to Right output with no filtering
	// CodecDataOut.Channel[RIGHT] = (short) resultR;
	/* end your code here */
	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
//	WriteDigitalOutputs(0); // Write to GPIO J15, pin 6; end ISR timing pulse
}

//White noise generator for filter noise testing
long int rand_int(void)
{
	static long int a = 100001;

	a = (a*125) % 2796203;
	return a;
}
