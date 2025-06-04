#include "button.h"
#include <xc.h>

/* Creating a Queue of size 16 and defining the head and tail, as well as the count.
   all three of which are to start at 0

     Queue structure:
        - SIZE: 16 (fixed capacity)
        - head: Index of the front element
        - tail: Index where the next element will be inserted
        - count: Current number of elements in the queue
*/
static uint16_t queue[QUEUE_SIZE]; // store 16 elements (enough for a preamble and start bits + transmitted data)
static uint8_t head = 0; // front of queue
static uint8_t tail = 0;
static uint8_t count = 0;


// function to add to the queue
uint8_t enqueue(uint8_t bit)
{
    if (count >= QUEUE_SIZE) return 0;  // Queue full
    
    
    queue[tail] = bit;
    tail = (tail + 1) % QUEUE_SIZE;    // buffer wrap-around for circular wrap
    count++; // increment occupied spots
    return 1;
}

// function to remove from the front of queue and return it
uint8_t dequeue(void)
{
    if (count == 0) return 0; // check if queue empty

    uint8_t value = queue[head]; // set value 
    head = (head + 1) % QUEUE_SIZE; // buffer for circular wrap in memory
    count--; // decreases count "spots taken in line"
    return value;
}

// check if the queue is empty or not
uint8_t queue_head(void)
{
    if (count == 0) return 0;
    return queue[head];
}


// function to check if the queue is empty
uint8_t queue_is_empty(void)
{
    return (count == 0);
}


// function to reset(clear) the queue
void clear_queue(void)
{
    head = 0;
    tail = 0;
    count = 0;
}



uint8_t read_button(uint8_t pin)
{

     /*debounce so inputs dont get duplicated
     this is essentially just a delay to prevent it before returning the state of the pin
    */
     __delay_ms(10); 

                    /* (1 << pin) creates a bitmask where only the corresponding bit to the user defined pin
                        PORT C is then has a bitwise and to determine if the the pin is high or low. 
                        it returns a 0 or 1 depending on if the value is equivalent to 0. 
                    */ 

    return ((PORTC & (1 << pin)) == 0); // see above coment explaining this return statement

}



void handle_buttons(void)
{
    // RC0 adds a 1
    if (read_button(0))  // RC0 → enqueue 1
    {
        enqueue(1);      // add logic '1' to queue
        __delay_ms(200); // debounce delay to prevent multiple unintended presses
    }

    else if (read_button(1)) // RC1 → enqueue 0
    {
        enqueue(0);         // add logic '0' to queue
        __delay_ms(200);    // debounce delay
    }

    else if (read_button(3)) // RC3 → clear queue
    {
        clear_queue();       // reset the queue: head, tail, and count to 0
        __delay_ms(200);     // debounce delay
    }

    else if (read_button(4)) // RC4 → transmit queue (to be implemented later)
    {
        /*
            This is a placeholder for future logic:
            When triggered, this will dequeue and transmit all bits using PWM
            and display them on the 8-bit LED array.
        */
        __delay_ms(200); // debounce delay
    }
}

