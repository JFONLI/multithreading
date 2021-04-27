/* 
 * file: testcoop.c 
 */
#include <8051.h>
#include "preemptive.h"

/* 
 * @@@ [2pt] 
 * declare your global variables here, for the shared buffer 
 * between the producer and consumer.  
 * Hint: you may want to manually designate the location for the 
 * variable.  you can use
 *        __data __at (0x30) type var; 
 * to declare a variable var of the type
 */ 

__data __at (0x20) char buffer[3];
__data __at (0x23) char letter;
__data __at (0x24) char full;
__data __at (0x25) char empty;
__data __at (0x26) char mutex;
__data __at (0x27) char num;
__data __at (0x28) char p1;
__data __at (0x29) char p2;

/* [8 pts] for this function
 * the producer in this test program generates one characters at a
 * time from 'A' to 'Z' and starts from 'A' again. The shared buffer
 * must be empty in order for the Producer to write.
 */
void Producer1(void) {
        /*
         * @@@ [2 pt]
         * initialize producer data structure, and then enter
         * an infinite loop (does not return)
         */


        while (1) {
        /* @@@ [6 pt]
         * wait for the buffer to be available, 
         * and then write the new data into the buffer */
                SemaphoreWait(p2);
                SemaphoreWait(empty);
                SemaphoreWait(mutex);
                
                
                __critical{
                        //buffer = letter;

                        buffer[full] = letter;
                        /*
                        if (empty == 1)
                                buffer[2] = letter;
                        if (empty == 2)
                                buffer[1] = letter;
                        if (empty == 3)
                                buffer[0] = letter;
                        */
                }

                letter = (letter == 'Z') ? 'A' : letter + 1;

                
                SemaphoreSignal(mutex);
                SemaphoreSignal(full);
                SemaphoreSignal(p1);
        }
}

void Producer2(void) {
        /*
         * @@@ [2 pt]
         * initialize producer data structure, and then enter
         * an infinite loop (does not return)
         */


        while (1) {
        /* @@@ [6 pt]
         * wait for the buffer to be available, 
         * and then write the new data into the buffer */
                SemaphoreWait(p1);
                SemaphoreWait(empty);
                SemaphoreWait(mutex);
                
                
                __critical{
                        //buffer = letter;

                        buffer[full] = num;
                        /*
                        if (empty == 1)
                                buffer[2] = letter;
                        if (empty == 2)
                                buffer[1] = letter;
                        if (empty == 3)
                                buffer[0] = letter;
                        */
                }

                num = (num == '9') ? '0' : num + 1;

                
                SemaphoreSignal(mutex);
                SemaphoreSignal(full);
                SemaphoreSignal(p2);
        }
}

/* [10 pts for this function]
 * the consumer in this test program gets the next item from
 * the queue and consume it and writes it to the serial port.
 * The Consumer also does not return.
 */
void Consumer(void) {
        /* @@@ [2 pt] initialize Tx for polling */
        TMOD |= 0x20;
        TH1 = -6;
        SCON = 0x50;
        TR1 = 1;
        
        while (1) {
        /* @@@ [2 pt] wait for new data from producer
         * @@@ [6 pt] write data to serial port Tx, 
         * poll for Tx to finish writing (TI),
         * then clear the flag
         */
                SemaphoreWait(full);
                SemaphoreWait(mutex);
                
                __critical{
                        SBUF = buffer[0];
                        buffer[0] = buffer[1];
                        buffer[1] = buffer[2];
                }

                SemaphoreSignal(mutex);
                SemaphoreSignal(empty);

                while(TI==0);
                TI = 0;
                 
        }
}

/* [5 pts for this function]
 * main() is started by the thread bootstrapper as thread-0.
 * It can create more thread(s) as needed:
 * one thread can acts as producer and another as consumer.
 */
void main(void) {
          /* 
           * @@@ [1 pt] initialize globals 
           * @@@ [4 pt] set up Producer and Consumer.
           * Because both are infinite loops, there is no loop
           * in this function and no return.
           */

        //buffer = 'A'; 
        letter = 'A';
        num = '0';

        SemaphoreCreate(full, 0);
        SemaphoreCreate(empty, 3);
        SemaphoreCreate(mutex, 1);
        SemaphoreCreate(p1, 0);
        SemaphoreCreate(p2, 1);

        ThreadCreate(Producer1);
        ThreadCreate(Producer2);
        Consumer();

        

}

void _sdcc_gsinit_startup(void) {
        __asm
                ljmp  _Bootstrap
        __endasm;
}

void _mcs51_genRAMCLEAR(void) {}
void _mcs51_genXINIT(void) {}
void _mcs51_genXRAMCLEAR(void) {}

void timer0_ISR(void) __interrupt(1) {
        __asm
                ljmp _myTimer0Handler
        __endasm;
}
