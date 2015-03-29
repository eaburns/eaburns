#include <timer.h>
#include <interrupt.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <retval.h>
#include <irq.h>
#include <sched.h>

static time_event_t *head = NULL;
uint32 hertz = 1000;

void timer_interrupt( struct regs r, word_t ip )
{
  time_event_t *p;

  for( p = head; p != NULL; p = p->next ) {

    if( --(p->t_remaining) == 0 ) {
      if( p->t_interval == 0 ) { 
        time_event_del( p );
      } else {
        p->t_remaining = p->t_interval;
      }
      p->event( p->arg );
    }

  }

}

void timer_enable()
{
  if( enable_irq( 0 ) == RET_FAILURE )
    kprintf( "timer: failed to enable.\n" );
}

void timer_disable()
{
  if( disable_irq( 0 ) == RET_FAILURE )
    kprintf( "timer: failed to disable.\n" );
}

void time_event_set( time_event_t *e, uint32 interval, uint32 repeat, 
                     time_event funct, void* arg )
{
  time_event_t *p;

  /* If the time even `e' is active, then return with no changes. */
  for( p = head; p != NULL && p != e; p = p->next );
  if( p == e ) { return; }

  e->t_remaining = interval;
  e->t_interval = (repeat == TIME_EVENT_REPEAT? interval: 0);
  e->event = funct;
  e->arg = arg; 
  e->next = NULL;
}

void time_event_del( time_event_t *e )
{
  time_event_t *p, *j;

  for( j = NULL, p = head; p != NULL && p != e; j = p, p = p->next );

  /* Case: event not found. */
  if( p != e ) { return; }

  /* Case: event is head. */
  if( j == NULL ) {
    head = head->next;

  /* Case: event is non-head. */
  } else {
    j->next = p->next;
  }
}

void time_event_add( time_event_t *e )
{
  time_event_t *p;

  /* If the time even `e' is active, then return with no changes. */
  for( p = head; p != NULL && p != e; p = p->next );
  if( p == e ) { return; }

  /* Initialize head on first call to time_event_add. */
  if( head == NULL )
    head->next = NULL;

  e->next = head;
  head = e;
}
