#include <xc.h>
#include <stdint.h>
#include "button.h"

// CONFIG
#pragma config FEXTOSC = OFF, RSTOSC = HFINTOSC_1MHZ
#pragma config WDTE = OFF, LVP = OFF, CP = OFF

#define _XTAL_FREQ 16000000
#define PREAMBLE 0b1001
#define PREAMBLE_LENGTH 4
#define DATA_LENGTH 8

typedef enum {
    STATE_WAIT_FOR_PREAMBLE,
    STATE_RECEIVE_DATA
} ReceiverState;

void init_io(void)
{
    TRISCbits.TRISC7 = 1;
    ANSELCbits.ANSELC7 = 0;

    TRISB = 0x00;
    ANSELB = 0x00;
    LATB = 0x00;

    OSCFRQ = 0x06;
}

uint8_t read_bit_rc7(void)
{
    __delay_ms(10); // adjust to match transmission timing
    return (PORTCbits.RC7 != 0);
}

uint8_t check_preamble(void)
{
    if (queue_count() < PREAMBLE_LENGTH) return 0;

    uint8_t value = 0;
    for (uint8_t i = 0; i < PREAMBLE_LENGTH; i++) {
        value = (value << 1) | queue_peek(i);
    }

    return (value == PREAMBLE);
}

void discard_preamble(void)
{
    for (uint8_t i = 0; i < PREAMBLE_LENGTH; i++) {
        dequeue(); // safely discard without pointer
    }
}

uint8_t receive_data_byte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < DATA_LENGTH; i++) {
        byte = (byte << 1) | dequeue();
    }
    return byte;
}

void rx_processing(void)
{
    init_io();
    clear_queue();  // from button.c

    ReceiverState state = STATE_WAIT_FOR_PREAMBLE;
    uint8_t bit, data;

    while (1)
    {
        bit = read_bit_rc7();
        enqueue(bit);  // from button.c

        if (state == STATE_WAIT_FOR_PREAMBLE) {
            if (check_preamble()) {
                discard_preamble();
                state = STATE_RECEIVE_DATA;
            }
        }
        else if (state == STATE_RECEIVE_DATA) {
            if (queue_count() >= DATA_LENGTH) {
                data = receive_data_byte();
                LATB = data;
                state = STATE_WAIT_FOR_PREAMBLE;
            }
        }
    }
}
