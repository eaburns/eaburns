#ifndef _TIMER_H_
#define _TIMER_H_

#include <ktypes.h>

/* Number of cycles of the timer per 1 second. */
extern uint32 hertz;

/* time_event is the call back function type for timed events in the kernel. */
typedef void(*time_event)(void*);

typedef struct _time_event_t {

  /* Call back function for the event. */
  time_event event;

  /* Argument to pass to the callback function. */
  void* arg;

  /* Remaining number of ticks before the event is called. */
  uint32 t_remaining;

  /* Value to use in resetting t_remaining after the event is called. 
   * If t_interval is initialized to 0, then the time_event is only 
   * triggered once. */
  uint32 t_interval;
  
  struct _time_event_t *next;
  
} time_event_t;

/* Timer interrupt routine. */
void timer_interrupt();

/* Enable the timer's irq. */
void timer_enable();

/* Disable the timer's irq. */
void timer_disable();

/* Adds the time_event_t to the list of active time events.  If the time_event_t
 * is already on the list then nothing is changed. */
void time_event_add( time_event_t *e );

/* Removes the time event from the active time event list.  If the time_event_t
 * is not on the list then nothing is changed. */
void time_event_del( time_event_t *e );

/* Initializes a time_event_t such that:
 *  - t_remaining is set to ``interval''
 *  - repeat != TIME_EVENT_REPEAT ? t_interval is set to ``interval''
 *                                : t_interval is set to 0.
 *  - event is set to ``funct''
 *  - arg is set to ``arg'' 
 * If the time event passed to the set function is on the active list, 
 * then no changes are made. */
#define TIME_EVENT_REPEAT 1
#define TIME_EVENT_ONCE 0
void time_event_set( time_event_t *e, uint32 interval, uint32 repeat, 
                     time_event funct, void* arg );
#endif /* _TIMER_H_ */
