# Checkpoint 4
For this programming project checkpoint, you are to test your preemptive multithreading and semaphore code by extending the classical bounded-buffer example to two producers and one consumer.  The two producers will compete with each other for writing. 

You should make a new directory and copy the source files from the previous checkpoint.  Make a copy of testpreempt.c and name it test3threads.c.   This is your opportunity to stress test your semaphore and threads code.  If your previous checkpoints worked but this one doesn’t, then something is wrong with your code.  Debug here and once it works, then apply those changes to your previous checkpoints to see if they still work.


## test3threads.c
Your test3threads.c can be based on testpreempt.c for CheckPoint3.   Continue the use of the <b>3-deep bounded-buffer (3-deep)</b> structure.   

* Instead of Producer() function, make Producer1() and Producer2() functions.  Producer1 should output letters 'A' to 'Z' and repeat forever; Producer2 should output '0' to '9' and repeat forever.
* Consumer() can remain unchanged.
* main() should spawn threads so that Producer1, Producer2, and Consumer run in three threads.

Try different orders of spawning threads.  What do you get?

## Fairness
How “fair” is your scheduler based on the output that you observe?  If you are using the default round-robin (RR) scheduling policy, chances are you will see the output of only one producer; and if you change the order of spawning threads, you may see the output of the other producer.  What kinds of output pattern do you consider to be fair?  Does your code have the problem of starvation?

Either
* show that your scheduling policy is “fair” and explain why, or 
* propose your own solution to make it fair for these threads.  Explain the changes that you made.

## Notice
Turn in a typescript showing compilation of your code using the Makefile from Checkpoint3 (but extended for test3threads.c).  You should use the following two commands (Note: $ is the prompt displayed by the shell and is not part of the command that you type.)  The first one deletes all the compiled files so it forces a rebuild if you have compiled before. The second one compiles it.

```
$ make clean
$ make
```

It should show actual compilation, warning, or error messages.  Note that not all warnings are errors.  The compiler should generate several test3threads.* files with different extensions:
* the .hex file can be opened directly in EdSim51
* the .map file shows the mapping of the symbols to their addresses after linking

