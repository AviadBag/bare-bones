// timer.h -- Defines the interface for all PIT-related functions.
// Written for JamesM's kernel development tutorials.

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define PIT_DATA_PORT    0x40
#define PIT_COMMAND_PORT 0x43

void init_timer(uint32_t frequency);

typedef void (*timer_callback_t)(int); // Ticking callback
void register_timer_callback(timer_callback_t);


#endif