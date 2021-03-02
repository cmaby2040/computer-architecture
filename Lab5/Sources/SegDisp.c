/*--------------------------------------------
File: SegDisp.c
Description:  Segment Display Module
---------------------------------------------*/

#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "delay.h"
#include "SegDisp.h"

// Prototypes for internal functions
unsigned char getDisplayValue(char ch);

#define NUM_7_SEG_DISPLAYS 4
//Convert character to 7 segment display version
#define CHAR_TO_NUM(a) (a - 0x30)

#define SEG_UPDATE_INTERVAL (ONETENTH_MS * 10) // Every 1 ms

static unsigned char pos[NUM_7_SEG_DISPLAYS];
static unsigned display_decimal_bitmask = 0x00;

static unsigned char NUM_TO_7_SEG_TBL[10] = { 0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67 }; //TODO
volatile int i = 0;

/*---------------------------------------------
Function: initDisp
Description: initializes hardware for the 
             7-segment displays.
-----------------------------------------------*/
void initDisp(void) 
{
	// Complete this function
    //Pg. 24 on dragon 12 manual
    //PORT B connects to the Anode
    //PB0-PB7
    //PP0-PP3 controls common cathode
    DDRB = 0xFF;
    DDRP = 0x1F; //0x0F
    clearDisp();

  TIOS_IOS1 = 1; // set TC1 to output-compare
	TIE_C1I = 1; // enable interrupt channel 1
	TC1 = TCNT + SEG_UPDATE_INTERVAL; //Set for every 10 ms
}

/*---------------------------------------------
Function: clearDisp
Description: Clears all displays.
-----------------------------------------------*/
void clearDisp(void) 
{
    int x;
    for(x = 0; x < 4; x++) {
        dispChars[x] = 0;
    }
}


/*---------------------------------------------
Function: setCharDisplay
Description: Receives an ASCII character (ch)
             and translates
             it to the corresponding code to 
             display on 7-segment display.  Code
             is stored in appropriate element of
             codes for identified display (dispNum).
-----------------------------------------------*/
void setCharDisplay(char ch, byte dispNum) 
{
	// Complete this function
    unsigned char value = getDisplayValue(ch);
    unsigned char dot_on = dispChars[dispNum] & 0x80;

    dispChars[dispNum] = value | dot_on;
}

unsigned char getDisplayValue(char ch) {
    if(ch == ' ') {
        return (unsigned char) 0x00;
    } else if (ch == 'A') {
        return (unsigned char)0x77;
    } else {
        return (unsigned char) NUM_TO_7_SEG_TBL[CHAR_TO_NUM(ch)];
    }
}

void interrupt VectorNumber_Vtimch1 timer1_isr(void) {
    PTP = (PTP & 0xA0) | ~(1 << i);    ///here is a change was originally a 0xF0
    PORTB = dispChars[i];
    
    i++;
    i = i % 4;
    TC1 = (TC1 + SEG_UPDATE_INTERVAL); //Update interrupt time
}

/*
 * Enables the dot at that position
*/
void turnOnDP(int position) 
{
    pos[position] |= 0x80;
}

/*
 * Disable the dot at that position
*/
void turnOffDP(int pos)
{
    pos[position] &= 0x3F;
}