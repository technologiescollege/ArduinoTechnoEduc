/************************************************************************
 *  
 *  NAME:  MowayduinoIo.h
 *
 *  DESCRIPTION:  Library to configure the ports 
 *				 of the mOwayduino Robot.
 *
 *  DATE: 08/11/2013    Rev 1.1    Minirobots, S.L.
 *
 ************************************************************************/

#ifndef mowayduinoio_h
#define mowayduinoio_h

//*************************************
//    I/O ports definitions
//*************************************
// Port names
#define RF_CE   	  0
#define CS_EXP  	  1
#define IRQ_EXP   	  2
#define RF_IRQ   	  3
#define MOT_R_N  	  4					//(PW2)
#define CS_ACC   	  5

#define SPEAKER   	6

#define MOT_L_N   	  7				//(PW4)
#define RF_CS   	  17
#define MOT_R_P   	  9				//(PW1)
#define MOT_L_P   	 10				//(PW3)
#define ENC_R   	 11
#define BATT_AD   	 A11			// (PD6)

#define CS_IO   	 13
#define ENC_L   	 8
#define IR_LR_P   	 18
#define IR_LL_P   	 19
#define IR_CR_P   	 20
#define IR_CL_P   	 21
#define MIC	   	 22
#define LIGHT   	 23
#define CE_EXP   	 30

// Analog/digital ports
#define IR_CR_AD    A0
#define IR_LR_AD    A1
#define IR_CL_AD    A2
#define IR_LL_AD    A3
#define LIGHT_AD    A5
#define MIC_AD	    A4

// Expansion
#define SCK_EXP	SCK
#define SDI_EXP	MOSI
#define SDO_EXP	MISO

#endif

