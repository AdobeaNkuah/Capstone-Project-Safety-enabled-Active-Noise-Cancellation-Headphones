// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011
 
///////////////////////////////////////////////////////////////////////
// Filename: main.c
//
// Synopsis: Main program file for demonstration code
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h"   

int main()
{    
	//initialize DSP board and ANC parameters
	StartUp();
	
	// main stalls here, interrupts drive operation 
  	while(1) { 
		;
  	}   
}


