#ifndef TIMER_H
#define TIMER_H

extern uint32_t timer0_counter_1 ; // ticks
extern uint32_t time_passed; // microseconds

void init_timer_0(void);
void init_timer_1(void);
void init_timer_2(void);
void init_timer_3(void);

#endif