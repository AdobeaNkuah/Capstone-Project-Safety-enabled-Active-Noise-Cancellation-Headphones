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
#include <stdio.h>
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

#define alpha 1        // leakage factor (leaky LMS algorithm)
#define N 80
float beta = 1e-9;        // learning rate

/* add any global variables here */
float x_E_buffer[N];       //buffer for error signal delay samples
float x_test_buffer[N];       //buffer for test signal delay samples

float x_test_filtered_buffer;       //buffer for the filtered test signal delay samples

int data_flag = 0;         //UART data received flag
char data = 'a';            //keyboard data (a = ANC mode) or (p = passthrough mode)

float w[N];             //buffer weights of adapt filter

float track_result = 0.0;

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
	x_test_buffer[0] = (float) CodecDataIn.Channel[LEFT];           // send test signal to Left Channel
	x_test_filtered_buffer = (float) CodecDataIn.Channel[RIGHT];    // send filtered test signal to Right Channel

	// ANC mode: use adaptive filter to produce anti-noise
	if (data == 'a' || data == 0){
	    for (i = 0; i< N; i++)
	        result += (w[i] * x_test_buffer[i]);    // y(n) = w(n) * r(n)

	    track_result = result;

	    x_E_buffer[0] = x_test_filtered_buffer - result;  // e(n) = s(n) + y(n)

	    for (i = 0; i<N; i++)
	    {
	        beta += (x_test_buffer[i]*x_test_buffer[i]);
	    }

	    beta = 1/beta;

	    for (i = N-1; i> 0; i--)       // calculate out of adaptive filt, updates weights and delays
	    {
	        w[i] = alpha * w[i] + beta*x_test_buffer[i]*x_E_buffer[0];     //update weights with leaky algorithm
	        x_test_buffer[i] = x_test_buffer[i-1];            //update reference delay samples
	        //x_E_buffer[i] = x_E_buffer[i-1];            //update error delay samples
	    }
	    w[0] = alpha * w[0] + beta*x_test_buffer[0]*x_E_buffer[0];
	}
	// passthrough mode: pass input sample directly to output
	else if(data == 'p'){
	    //result = x_test_buffer[0];
	    x_E_buffer[0] = x_test_filtered_buffer;
	}
	//Return 16-bit sample to DAC
	//CodecDataOut.Channel[LEFT] = (short) x_test_filtered_buffer;     // outputs y(n) (change 'result' to 'x_E' for error)
	CodecDataOut.Channel[LEFT] = (short) x_E_buffer[0];

	// Copy Right input directly to Right output with no filtering
	//CodecDataOut.Channel[RIGHT] = 0;
	/* end your code here */


	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
//	WriteDigitalOutputs(0); // Write to GPIO J15, pin 6; end ISR timing pulse

	/*char buffer[20];
	itoa(x_test_buffer[0],buffer,10);
	Puts_UART2(*buffer);
	*/

}

//White noise generator for filter noise testing
long int rand_int(void)
{
	static long int a = 100001;

	a = (a*125) % 2796203;
	return a;
}
