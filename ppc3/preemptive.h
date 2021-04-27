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

#define CNAME(s) _ ## s
#define L(x) LABEL(x)
#define LABEL(x) x ## $

// create a counting semaphore s that is initialized to n
#define SemaphoreCreate(s, n)  \
    s = n\

// do (busy-)wait() on semaphore s
#define SemaphoreWait(s) \
    SemaphoreWaitBody(s, L(__COUNTER__)) \

#define SemaphoreWaitBody(s, label) \
    { __asm \
        label: \
        MOV ACC, CNAME(s) \
        JZ label \
        dec  CNAME(s) \
      __endasm; }
    
// signal() semaphore s
#define SemaphoreSignal(s)  \
    __asm \
        INC CNAME(s) \
    __endasm;\

typedef char ThreadID;
typedef void (*FunctionPtr)(void);

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);

#endif // __PREEMPTIVE_H__
