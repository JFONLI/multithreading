# Checkpoint 5
For this programming project checkpoint, you are to (1) add a delay(n) function to your preemptive multithreading and semaphore code, (2) ensure your threads can terminate so its space can be recycled by another thread. (3) test your delay and thread recycling based on (but not identical) to the parking lot example you did earlier in the semester in Python.

There will be less hints for this CheckPoint.  

## delay(n), now()
You are to implement the void delay(unsigned char n) and unsigned char now(void) functions. The former delays the thread by n time units; the latter returns the “current time” (i.e., number of time units that has elapsed since the system reboots).  There are a number of considerations:
* In Python, the time unit for Thread.sleep() is in seconds, but it can be a floating point.  On 8051, we limit the time unit to be an unsigned char (0-255), but what is a good time unit? (e.g., number of seconds, ms, µs, or some multiple of the 8051 timer?)   Note that any time unit that is larger than 1 second is not useful.
* An important consideration is that delay() is not an exact delay but is a delay for “at least n time units” and “less than (n + 0.5) time units” for it to be acceptable (otherwise, it rounds up to n+1 time units, which would not be correct).  Of course, the more accurate the better, but there is an inherent limit on how accurate it can be. 
* Based on the above requirement, state your choice of time unit and provide your justification for how you think you can implement a delay() that meets the requirement above.

Explain how you would implement the delay function in relation to the timer used for preemption.  By default, we set it up for a 13-bit timer for the quantum. Considering that each thread gets its own delay call independently:
* what does your timer-0 ISR have to do to support these multiple delays and now()?  
* what if all threads call delay() and happen to finish their delays all at the same time?  How can you ensure the accuracy of your delay? (i.e., between n and n+0.5 time units)?  
* How does the worst-case delay completion (i.e., all threads finish delaying at the same time) affect your choice of time unit?

You may assume it is okay for now() to wrap around to 0 after it exceeds 255 time units.

Create your own test case to be sure your delay function works before proceeding to the next part.


## Robust Thread Termination and Creation
You may have already implemented the thread termination already, but if you have not, you should modify your code so that a thread can safely terminate by either calling ThreadExit() or return normally from the function.  This is easy, because all you have to do is to push the return address of ThreadExit() on the stack.  ThreadExit() just has to mark the thread as unallocated.

One other thing you need to do is to guard thread creation and termination.  We have a maximum limit of 4 threads.  This means you need to use a semaphore to allow creation of threads up to the max, and any attempt to create additional threads will block until some thread has exited.  Add the proper code in your ThreadCreate() and ThreadExit() code to make use of the semaphore(s) similar to the bounded-buffer example.

One thing about ThreadExit() is that if you are the last thread to exit, then it should enter an infinite loop, instead of returning to nowhere.


## Parking Lot Example
Consider the parking simulation example you in Python for Assignment 8, Part 2.  Instead of making 15 cars and 5 spots, make 5 cars competing for 2 spots.  Make thread for each car.  However, since our threads package supports at most 4 threads (including main), your main() will use a semaphore, similar to the bounded-buffer example to create threads up to the maximum number available. 

Instead of immediately writing out the result, you should maintain a “log” for the events:
* when a car gets the parking spot (what time, which spot)
* when a car exits the parking lot (what time)
Minimally, show the memory dump of your table to reflect the content of this log. 

Extra credit [30 points]  Display the output of your log to UART in a human-readable text format.
