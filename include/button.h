#ifndef BUTTON_H
#define BUTTON_H

#include <xc.h>
#include <stdint.h>

/*  
    Queue Constants and Declarations

    QUEUE_SIZE: Max number of elements in the circular queue.
    Functions provide control to:
        - enqueue bits (0 or 1)
        - dequeue bits
        - clear/reset queue
        - check queue head
        - check if queue is empty
*/

/* -------------------- Queue Constants -------------------- */
#define QUEUE_SIZE 16


/* -------------------- Queue Operations -------------------- */

/*
    Adds a bit (0 or 1) to the queue.

    Parameters:
        bit - value to enqueue (should be 0 or 1)

    Returns:
        1 if successful, 0 if queue is full
*/
uint8_t enqueue(uint8_t bit);


/*
    Removes and returns the front bit in the queue.

    Returns:
        Bit (0 or 1) from the front of the queue,
        or 0xFF if the queue is empty.
*/
uint8_t dequeue(void);


/*
    Returns the bit at the head of the queue without removing it.

    Returns:
        Bit (0 or 1) from the head, or 0xFF if empty.
*/
uint8_t queue_head(void);


/*
    Checks whether the queue is empty.

    Returns:
        1 if empty, 0 otherwise.
*/
uint8_t queue_is_empty(void);


/*
    Clears the queue, resetting head, tail, and count.
*/
void clear_queue(void);


/* -------------------- Button Handling -------------------- */

/*
    Reads the button state on a specific PORTC pin with simple debounce.

    Parameters:
        pin - bit index (0-7) of the PORTC pin to read

    Returns:
        1 if button is pressed (active low), 0 otherwise.
*/
uint8_t read_button(uint8_t pin);


/*
    Handles logic tied to button actions:
        - RC0: Enqueue 1
        - RC1: Enqueue 0
        - RC3: Clear queue
        - RC4: Transmit (to be implemented)

    Should be called in the main loop.
*/
void handle_buttons(void);


#endif // BUTTON_H