# Checkpoint 2
For this programming project checkpoint, you are to write a preemptive multithreading package and a test case based on the single-buffer producer-consumer example.  Preemptive multithreading means the code for each thread does not need to explicitly do a thread-yield (not to be confused with Python’s generator-yield) in order to switch to another thread, if any.  Of course, it can also do ThreadYield() if it wants.

Your preemptive.h should be mostly the same as your cooperative.h, except any macro names involving __COOPERATIVE_H__ should be replaced with __PREEMPTIVE_H__.  Also, depending on your implementation, you may or may not use a separate thread for the thread manager.  If so, then you may want to define your MAXTHREADS accordingly.  All other API should be the same.  Actually, the ThreadYield() function should be available for preemptive threads, too, because even though the OS can always preempt a thread, a thread should still be able to voluntarily give up control.


## testpreempt.c
### main + test case for preemptive multithreading

Write the code for testpreempt.c, which sets up the startup code and defines main() to set up the producer and consumer.  It can be based on testcoop.c from your CheckPoint 1.

<br>

### Producer and Cosumer
These should be mostly the same as those from testcoop.c.  The differences are
* They should not call ThreadYield().  Instead, rely on the preemptive mechanism to context switch.
* where they access the shared variables, SDCC suggests that you can surround the code fragment using the __critical { } construct, to ensure that the two shared vars are accessed atomically.  It can be like
__critical {
      // your code to be executed atomically
}
but you should to examine the assembly output to see if it does what you expect.  If not, then you may need to temporarily disable interrupts by
EA = 0;
and reenable interrupts by
EA = 1;


* In your Consumer() code, where it sets TMOD, you might want to do 
 TMOD |= 0x20;
instead of TMOD = 0x20, because TMOD is also assigned by the (modified) Bootstrap code to set up the timer interrupt in timer-0 for preemption.  This way, it preserves the Bootstrap code’s setting.

<br>

### ISR
As explained in lecture, the ISR should be defined in the same file as main() in order for SDCC to generate the proper code for ISR.  So, include the following lines at the bottom of your testpreempt.c:

```
    void _mcs51_genRAMCLEAR(void) {}
    void _mcs51_genXINIT(void) {}
    void _mcs51_genXRAMCLEAR(void) {}

    void timer0_ISR(void) __interrupt(1) {
            __asm
                    ljmp _myTimer0Handler
            __endasm;
    }
```
This allows the ISR to call your routine named myTimer0Handler (defined in preemptive.c) to handle the actual interrupt itself.

## preemptive.c
Your preemptive.c can be based on cooperative.c that you did for CheckPoint1.  Make sure the cooperative version works properly before attempting this part.  In addition, you may also want to see these slides for more details on the timer and its use in preemptive scheduling.

<br>

### SAVESTATE and RESTORESTATE
Check to make sure your assembly code saves registers before trashing them.   It can be the same as the cooperative version.

<br>

### Bootstrap()
In Bootstrap, you can set up Timer 0 to cause preemption.  (Timer 1 is already used by UART).  If you want to use Timer 0 in mode 0, you can add this code
```
    TMOD = 0;  // timer 0 mode 0
    IE = 0x82;  // enable timer 0 interrupt; keep consumer polling
                // EA  -  ET2  ES  ET1  EX1  ET0  EX0
    TR0 = 1; // set bit TR0 to start running timer 0
```
before you create the initial thread and context switch to it.

<br>

### ThreadCreate(), ThreadYield(), ThreadExit() 
These functions are largely the same as the cooperative version, except we want to make sure it is executed atomically.  To do this, you could try using the __critical construct just before the { } for the entire function’s body and check if SDCC generates the code you expect; or you may set and clear EA bit before and after the function body code.

<br>

### myTimer0Handler()
This is the new routine you need to write to be the ISR for Timer0, which serves the purpose of preemption.  A straightforward implementation would be to copy the code for ThreadYield() but instead of relying on the compiler to generate RET instruction to return to a function (or subroutine) call, you need to put in the RETI assembly instruction to return from the interrupt (after all, it is invoked as an interrupt service routine).

However, depending on how you write the ThreadYield(), if you write parts of it in C between SAVESTATE and RESTORESTATE, it is likely to use registers (especially R0 and R1 if it needs to use pointers to IRAM).  Because it only saves the bank numbers but not copying the register values, any code that modifies R0 - R7 will trash them and their values cannot be restored by RESTORESTATE.

One solution is for you to insert code to preserve the value of any such registers by copying them to registers that have been saved (e.g., B, DPH, DPL, etc., or your designated memory locations) after SAVESTATE, and copy them back to those registers before the RESTORESTATE.   This is the quickest way to get working code.

Another solution, which may be more robust, is to reserve one thread just for the thread manager, so that you always switch context to this thread (which runs outside the ISR), and it has its own register set and stack, so that trashing R0-R7 is not an issue.  It also has the advantage of keeping the ISR short, especially if the scheduler itself gets more complex.  However, it also means using one thread, so the user has now one fewer thread.


# Notice
Turn in a typescript showing compilation of your code using a modified Makefile (same as for cooperative except the file names are changed to the preemptive version).  You should use the following two commands (Note: $ is the prompt displayed by the shell and is not part of the command that you type.)  The first one deletes all the compiled files so it forces a rebuild if you have compiled before. The second one compiles it.

```
$ make clean
$ make
```

It should show actual compilation, warning, or error messages.  Note that not all warnings are errors.  The compiler should generate several testpreempt.* files with different extensions:
* the .hex file can be opened directly in EdSim51
* the .map file shows the mapping of the symbols to their addresses after linking
