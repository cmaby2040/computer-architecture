/*----------------
File: Delay.h
Description: Header file for Delay Module
--------------------*/
#ifndef DELAY_H
#define DELAY_H

// Function Prototypes
void initDelay(void);
void delayms(int);
void setCounter(volatile int *);

#define ONETENTH_MS 75	// number for timer to increment in 0.1 ms (75*1 1/3 micro-sec)

#endif