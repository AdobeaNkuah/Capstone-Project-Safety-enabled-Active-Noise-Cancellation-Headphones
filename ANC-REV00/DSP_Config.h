// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2011
 
///////////////////////////////////////////////////////////////////////
// Filename: DSP_Config.h
//
// Synopsis: Declarations for configuring the codec parameters
//
///////////////////////////////////////////////////////////////////////

#ifndef	DSP_CONFIG_H_INCLUDED
#define DSP_CONFIG_H_INCLUDED

#include "OMAPL138_Support_DSP.h"  

#define DEMO 0    // '0' for ANC demo, '1' for sound recognition demo

// uncomment just the line for the sample rate when using the OMAP-L138
//#define SampleRateSetting 		AIC3106Fs48kHz		// 48kHz sample rate
//#define SampleRateSetting 		AIC3106Fs96kHz		// 96kHz sample rate
//#define SampleRateSetting 		AIC3106Fs32kHz		// 32kHz sample rate
#define SampleRateSetting 		AIC3106Fs24kHz		// 24kHz sample rate
//#define SampleRateSetting 		AIC3106Fs16kHz		// 16kHz sample rate
//#define SampleRateSetting 		AIC3106Fs12kHz		// 12kHz sample rate
//#define SampleRateSetting 		AIC3106Fs8kHz		//  8kHz sample rate

#if DEMO == 0
    #define CodecType   0

#elif DEMO == 1
    #define SampleRateSetting         AIC3106Fs48kHz      // 48kHz sample rate
    #define CodecType   2

#endif


#endif
