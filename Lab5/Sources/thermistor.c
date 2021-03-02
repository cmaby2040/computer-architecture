#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "thermistor.h"


static volatile int temp;
static volatile int counter = 0;

#define FIVE_HUNDRED_KHZ_PRESCALER 0b10111
#define MAX_SAMPLE_TIME (0b11 << 5)
#define TEN_BITS_RESOLUTION 0

#define RIGHT_JUSTIFIED 0x80

//2^10 - 1 = 1023 max value

//Min: 0.025 V
//Max 1 V
//5V = 500 degrees

//0-5V = 0-1023
//So each change in the input is 500 / 1023 = +0.48875 degrees

#define INTERVAL_1_TENTH 75000

void initThermistor(void) {    
    ATD0CTL2 = 0;        //sets  the A/D converter prescale values
    ATD0CTL3 = 0;
    ATD0CTL4 = 0;
    ATD0CTL2_AFFC = 1; //Enable fast clear 
    ATD0CTL2_ASCIE = 1; //Enable interrupts
    ATD0CTL3_S1C = 1; //Single conversion
    ATD0CTL4 = 0b10111 | (0b11 << 5) | 0; //right shifts output.at the specified register
    ATD0CTL2_ADPU = 1; //Power up the ADPU
    //Wait 20 microseconds
    int x=0;
    for(x = 0; x < 3600; x++);
    TIOS_IOS3 = 1; //Enable output mode
    TC3 = TCNT +  INTERVAL_1_TENTH; //Set for every 10 ms
    TIE_C3I = 1; //Enable interrupt 3
}
 
void interrupt VectorNumber_Vtimch3 tc3_isr(void) {
    counter++;
    if(counter >= 10) { //Every 100 ms
        counter = 0;
        ATD0CTL5 = 0x80 | 5;  //adjust the  output value
    }
    TC3 = TC3 +  INTERVAL_1_TENTH;
}
void interrupt VectorNumber_Vatd0 atd_isr(void) {
    temp = ATD0DR0;    //collects temp value from the A/D connverter output
}
int getTemp(void) {
    return temp * 4.8876; //4.8876 is needed to accuratiy find the temp 500 degrees  
                          //divided by 1023 gives 0.48875 so multiple the output
                          //by that value
}
