#include <xc.h>
#include "port.h"

// Function to initialize output port
static void PORT_Init(void)
{
    //TRISBbits.TRISB0 = 0;   // Set RB0 as output (PWM1)
    TRISB = 0x00; // bitwise and setting RB0-RB7 as inputs
    //ANSELBbits.ANSELB0 = 0; // Disable analog on RB0
    ANSELB = 0x00; // disabling all analog on the affected pins (rb0-rb7)

    TRISC &= 0xF0; // bitwise and setting RC0-RC3 as inputs 
    ANSELC &= 0xF0; // same thing disabling analog on them 

    
}