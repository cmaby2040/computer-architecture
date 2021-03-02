#include "mc9s12dg256.h"
#include "delay.h"
#include "siren.h"


#define WAVEFORM_INTERVAL (ONETENTH_MS * 500)
void initSiren(void) {
    TIOS_IOS5 = 1; // set to output-compare
}

void turnOnSiren(void) {
    // Set PTN to 1 on trigger
    TCTL1_OM5 = 1;
    TCTL1_OL5 = 1;
    CFORC_FOC5 = 1; //Force write

    // Set PTN to toggle on trigger
    TCTL1_OM5 = 0;
    TCTL1_OL5 = 1;
    

    TC5 = TCNT + WAVEFORM_INTERVAL;
    TIE_C5I = 1; // enable interrupt channel
}
void interrupt VectorNumber_Vtimch5 timer5_isr() {
    TC5 = TCNT + WAVEFORM_INTERVAL;
}

void turnOffSiren(void) {
    TIE_C5I = 0; // disable interrupt channel
    
    // Set PTN to low
    TCTL1_OM5 = 1;
    TCTL1_OL5 = 0;
    CFORC_FOC5 = 1; //Force write
}