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
float x_R_buffer[N];       //buffer for reference signal delay samples
float x_E_buffer[N];       //buffer for error signal delay samples
int data_flag = 0;         //UART data received flag
char data = 0;              //keyboard data (a = ANC mode) or (p = passthrough mode)

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
	data_flag = IsDataReady_UART2(); //polls status of UART receive flag
	if(data_flag != 0){
	    data = Read_UART2();        // reads UART data (either 'a' or 'p')
	}
	float result = 0; //initialize the accumulator


 	if(CheckForOverrun())	// overrun error occurred (i.e. halted DSP)
		return;				// so serial port is reset to recover

 	CodecDataIn.UINT = ReadCodecData();		// get input data samples

	//Use the next line to noise test the filter
	x_R_buffer[0] = CodecDataIn.Channel[LEFT];
	x_E_buffer[0] = CodecDataIn.Channel[RIGHT];

	// ANC mode: use adaptive filter to produce anti-noise
	if (data == 'a' || data == 0){
	    for (i = N-1; i>= 0; i--)       // calculate out of adaptive filt, updates weights and delays
	    {
	        result += (w[i] * x_R_buffer[i]);
	        w[i] = alpha * w[i] + beta*x_R_buffer[0]*x_E_buffer[i];     //update weights with leaky algorithm
	        x_R_buffer[i] = x_R_buffer[i-1];            //update reference delay samples
	        x_E_buffer[i] = x_E_buffer[i-1];            //update error delay samples
	    }
	}
	// passthrough mode: pass input sample directly to output
	else if(data == 'p'){
	    result = x_R_buffer[0];

	}
	//Return 16-bit sample to DAC
	CodecDataOut.Channel[LEFT] = (short) result;

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
