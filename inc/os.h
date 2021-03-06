/*  Title       : os
 *  Filename    : os.h
 *  Author      : iacopo sprenger
 *  Date        : 29.04.2022
 *  Version     : 0.1
 *  Description : operating system definitions
 */

#ifndef OS_H
#define OS_H



/**********************
 *  INCLUDES
 **********************/

#include <stdint.h>

#include <port.h>
#include <hal.h>

/**********************
 *  CONSTANTS
 **********************/

#define OS_THREAD_NAME_LEN	8
#define OS_EVENT_NAME_LEN	8


/**********************
 *  MACROS
 **********************/


/**********************
 *  TYPEDEFS
 **********************/

typedef enum os_thread_state {
	OS_RUNNING,	//Thread is currently running
	OS_READY,	//Thread is ready to run
	OS_SUSPENDED,	//Thread is suspended (waiting on scheduled time)
	OS_WAITING,	//Thread is waiting for an event
	OS_DISABLED	//Thread is disabled
}os_thread_state_t;

typedef enum os_event_state {
	OS_FREE,
	OS_TAKEN

}os_event_state_t;


/**
 * higher threads have higher priority
 **/
typedef uint8_t os_priority_t;

typedef struct os_thread os_thread_t;

typedef struct os_event os_event_t;

struct os_event {
	uint8_t name[OS_EVENT_NAME_LEN];
	os_event_state_t state;
	os_thread_t * owner;
};

struct os_thread {
	uint8_t name[OS_THREAD_NAME_LEN];
	os_thread_t * next;
	os_priority_t priority;
	port_context_t context;
	os_thread_state_t state;
	hal_systick_t suspended_timer;
	os_event_t * waiting_event;
	uint8_t existing;
};

/**********************
 *  VARIABLES
 **********************/


/**********************
 *  PROTOTYPES
 **********************/

/* os_system */
void os_system_init(void);

void os_system_panic(const uint8_t * msg);

void os_system_start(void);


/* os_thread */
void os_thread_create(	os_thread_t * thd,
			os_priority_t prio, 
			void (*entry)(void), 
			uint8_t * stack, 
			uint16_t stack_size);

void os_thread_createI( os_thread_t * thd,
			os_priority_t prio, 
			void (*entry)(void), 
			uint8_t * stack, 
			uint16_t stack_size);

void os_thread_list(void);


/* os_delay */

void os_delay_compute(void);

void os_delay(hal_systick_t delay) __attribute__((naked));

void os_delay_windowed(hal_systick_t * last_wake, hal_systick_t delay);



/* os_event */

void os_event_create(os_event_t * event, os_event_state_t state);

void os_event_wait(os_event_t * event) __attribute__((naked));

void os_event_signal(os_event_t * event) __attribute__((naked));


void os_event_take(os_event_t * event);

void os_event_release(os_event_t * event);

#endif /* OS_H */

/* END */
