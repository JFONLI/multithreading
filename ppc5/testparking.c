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

 //Perfect

__data __at (0x21) char empty;
__data __at (0x22) char mutex;
__data __at (0x23) char empty_thread;
__data __at (0x24) char space1;
__data __at (0x25) char space2;
__data __at (0x26) char car;
__data __at (0x27) char car_id;
__data __at (0x28) char i;
__data __at (0x2A) char car_name[4];


void log(char a, char b, char c){
        TMOD |= 0x20;
        TH1 = -6;
        SCON = 0x50;
        TR1 = 1;

        for(i = 0; i < 5 ; i++){
                if( i==0 ) 
                        SBUF = a;
                else if( i == 1 ) 
                        SBUF = b;
                else if( i == 2 ) 
                        SBUF = (time&7) + '0';
                else if( i == 3 ) 
                        SBUF = c;
                else if( i == 4 ) 
                        SBUF = '\n';

        while( !TI ){}
        TI = 0;
    }
        
}

/* [8 pts] for this function
 * the producer in this test program generates one characters at a
 * time from 'A' to 'Z' and starts from 'A' again. The shared buffer
 * must be empty in order for the Producer to write.
 */

void Producer(void) {
        SemaphoreWait(empty);
        SemaphoreWait(mutex);

        EA = 0;
        if (space1 == '0'){
                //space1 stop car
                space1 = car_name[current_id];
                //log(car_name[current_id], 'i');
                log(car_name[current_id], 'i', '1');

        }
        else if (space2 == '0'){
                //space2 stop car
                space2 = car_name[current_id];
                //log(car_name[current_id], 'i');
                log(car_name[current_id], 'i', '2');
        }
        EA = 1;

        SemaphoreSignal(mutex);

        delay(2);

        EA = 0;
        if (space1 == car_name[current_id]){
                space1 = '0';
                //log(car_name[current_id], 'o');
                log(car_name[current_id], 'o', ' ');
        }
        else if (space2 == car_name[current_id]){
                space2 = '0';
                //log(car_name[current_id], 'o');
                log(car_name[current_id], 'o', ' ');
        }

        EA = 1;
        SemaphoreSignal(empty);
        SemaphoreSignal(empty_thread);
        ThreadExit();

}


void main(void) {

       SemaphoreCreate(mutex, 1);
       SemaphoreCreate(empty, 2); 
       SemaphoreCreate(empty_thread, 3);

        EA = 1;

       car = '1';
       space1 = '0';
       space2 = '0';

        while(1){

                SemaphoreWait(empty_thread);
                car_id = ThreadCreate(Producer);
                car_name[car_id] = car;
                car = (car == '5') ? '1' : car+1;
                
        }

        ThreadExit();

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
