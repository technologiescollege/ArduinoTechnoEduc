/********************************************************************************************
*       File:       stepGenerator.S                        		                            *
*		Version:    1.1.0                                           						*
*      	Date: 		June 14, 2020 	                                    					*
*      	Author: 	Thomas Hørring Olsen                                   					*
*                                                   										* 
*********************************************************************************************
*                       stepGenerator                   		                            *
*                                                                                           *
*		This file contains the assembler implementation of the step generation algorithm 	*
*                                                                                           *
*********************************************************************************************
*   (C) 2020                                                                                *
*                                                                                           *
*   uStepper ApS                                                                            *
*   www.ustepper.com                                                                        *
*   administration@ustepper.com                                                             *
*                                                                                           *
*   The code contained in this file is released under the following open source license:    *
*                                                                                           *
*           Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International         *
*                                                                                           *
*   The code in this file is provided without warranty of any kind - use at own risk!       *
*   neither uStepper ApS nor the author, can be held responsible for any damage             *
*   caused by the use of the code contained in this file !                                  *
*                                                                                           *
********************************************************************************************/
/** @file stepGenerator.S
 * @brief      	This file contains the assembler implementation of the step generation algorithm
 *
 * @author     Thomas Hørring Olsen (thomas@ustepper.com)
 */

.global _stepGenerator

.section .text

_stepGenerator:

.equ _STEPSSINCERESET,0
.equ _CNTSINCELASTSTEP,4
.equ _STEPDELAY,8
.equ _DIRECTION,12
.equ _DECELTOSTOPTHRESHOLD,13
.equ _CONTINOUS,17
.equ _PIDERROR,18
.equ _STATE,19
.equ _MODE,20

push r17
push r18
push r20

ldd r16,z+_CONTINOUS
sbrc r16,0          ;If running continous, we just need to check if its time to give a step
rjmp _checkRdy

ldd r16,z+_PIDERROR
cpi r16,0
brne _runAlgorithm      
ldd r16,z+_STATE
ldi r17,1
cpse r16,r17
rjmp _checkRdy
rjmp _finish

_runAlgorithm:
/************* NOT NEEDED *************
ldd r16,z+_STEPSSINCERESET
ldd r17,z+_DECELTOSTOPTHRESHOLD
cp r16,r17
brne _checkRdy
ldd r16,z+_STEPSSINCERESET+1
ldd r17,z+_DECELTOSTOPTHRESHOLD+1
cp r16,r17
brne _checkRdy
ldd r16,z+_STEPSSINCERESET+2
ldd r17,z+_DECELTOSTOPTHRESHOLD+2
cp r16,r17
brne _checkRdy
ldd r16,z+_STEPSSINCERESET+3
ldd r17,z+_DECELTOSTOPTHRESHOLD+3
cp r16,r17
brne _checkRdy
;ldi r16,1
;std z+_STATE,r16
ldd r16,z+_PIDERROR
cpi r16,1
brlo _finish
************************************/
subi r16,1
std z+_PIDERROR,r16

_checkRdy:
ldd r16,z+_CNTSINCELASTSTEP
ldd r17,z+_CNTSINCELASTSTEP+1
ldd r18,z+_CNTSINCELASTSTEP+2
ldd r20,z+_STEPDELAY+2

cp r18,r20
brlo _cntUp
ldd r20,z+_STEPDELAY+1
cp r17,r20
brlo _cntUp
ldd r20,z+_STEPDELAY
cp r16,r20
brlo _cntUp

ldi r20,0
std z+_CNTSINCELASTSTEP,r20
std z+_CNTSINCELASTSTEP+1,r20
std z+_CNTSINCELASTSTEP+2,r20

ldd r16,z+_DIRECTION
sbrc r16,0
rjmp _CCW

sbi 0x05,2 ;SET _DIRECTION PIN TO CW !
sbi 0x0B,7 ;PULL STEP PIN HIGH !!!

ldd r16,z+_MODE
cpi r16,2			;Check if we are in PID mode
breq _finish

ldd r16,z+_STEPSSINCERESET
ldi r17,1
add r16,r17
std z+_STEPSSINCERESET,r16
ldd r16,z+_STEPSSINCERESET+1
ldi r17,0
adc r16,r17
std z+_STEPSSINCERESET+1,r16
ldd r16,z+_STEPSSINCERESET+2
adc r16,r17
std z+_STEPSSINCERESET+2,r16
ldd r16,z+_STEPSSINCERESET+3
adc r16,r17
std z+_STEPSSINCERESET+3,r16

rjmp _finish

_CCW:

cbi 0x05,2 ;SET _DIRECTION PIN TO CCW !
sbi 0x0B,7 ;PULL STEP PIN HIGH !!!

ldd r16,z+_MODE
cpi r16,2			;Check if we are in PID mode
breq _finish

ldd r16,z+_STEPSSINCERESET
subi r16,1
std z+_STEPSSINCERESET,r16
ldd r16,z+_STEPSSINCERESET+1
sbci r16,0
std z+_STEPSSINCERESET+1,r16
ldd r16,z+_STEPSSINCERESET+2
sbci r16,0
std z+_STEPSSINCERESET+2,r16
ldd r16,z+_STEPSSINCERESET+3
sbci r16,0
std z+_STEPSSINCERESET+3,r16
rjmp _finish

_cntUp:
ldi r20,1
add r16,r20
ldi r20,0
adc r17,r20
adc r18,r20

std z+_CNTSINCELASTSTEP,r16
std z+_CNTSINCELASTSTEP+1,r17
std z+_CNTSINCELASTSTEP+2,r18

_finish:
cbi 0x0B,7 ; PULL STEP PIN LOW !!!
pop r20
pop r18
pop r17
pop r31
pop r30
pop r16
out 0x3F,r16
pop r16
reti