/*
 * file: preemptive.h
 *
 * this is the include file for the cooperative multithreading
 * package.  It is to be compiled by SDCC and targets the EdSim51 as
 * the target architecture.
 *
 * CS 3423 Fall 2018
 */

#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define MAXTHREADS 4  /* not including the scheduler */
/* the scheduler does not take up a thread of its own */

__data __at (0x30) char SSP[MAXTHREADS];
__data __at (0x34) char current_id;
__data __at (0x35) char bitmap;
__data __at (0x36) char final_id;
__data __at (0x37) char tmp_SP;
__data __at (0x38) char time_unit;
__data __at (0x39) char time;
__data __at (0x3A) char car_time[4];

#define CNAME(s) _ ## s
#define L(x) LABEL(x)
#define LABEL(x) x##$

// create a counting semaphore s that is initialized to n
#define SemaphoreCreate(s, n)  \
    s = n \

// do (busy-)wait() on semaphore s
#define SemaphoreWait(s) \
    SemaphoreWaitBody(s, L(__COUNTER__)) \

#define SemaphoreWaitBody(s, label) \
    { __asm \
        label: \
        MOV ACC, CNAME(s) \
        JZ label \
        JB ACC.7, label \
        dec  CNAME(s) \
      __endasm; }
    
// signal() semaphore s
#define SemaphoreSignal(s)  \
    __asm \
        INC CNAME(s) \
    __endasm;\


#define delay(n)\
        car_time[current_id] = time + n;\
        while( car_time[current_id] != time ){}\

typedef char ThreadID;
typedef void (*FunctionPtr)(void);

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);
//void delay(unsigned char n);
unsigned char now(void);

#endif // __PREEMPTIVE_H__
