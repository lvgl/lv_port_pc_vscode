/**
 * @file    Event management with pthreads
 * @brief   Implementation of an event mechanism using POSIX threads.
 * @date    2024-09-03
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Structure representing an event, consisting of a condition variable and a mutex */
typedef struct Event
{
    pthread_cond_t cond;      /* Condition variable used to signal the event */
    pthread_mutex_t mutex;    /* Mutex to protect access to the condition variable */
} Event_t;

/**
 * @brief   Create an event object
 *
 * Allocates memory for an Event_t structure and initializes its condition variable and mutex.
 *
 * @param   None
 * @return  Pointer to the created Event_t object, or NULL if memory allocation fails
 */
Event_t *event_create(void)
{
    Event_t *event = (Event_t *)malloc(sizeof(Event_t));  /* Allocate memory for the event */
    if (event)  /* Check if allocation was successful */
    {
        pthread_cond_init(&event->cond, NULL);  /* Initialize the condition variable */
        pthread_mutex_init(&event->mutex, NULL);  /* Initialize the mutex */
    }
    return event;  /* Return the created event object */
}

// ........................................................................................................
/**
 * @brief   Delete an event object
 *
 * Destroys the condition variable and mutex associated with the event, then frees the memory.
 *
 * @param   event  Pointer to the Event_t object to be deleted
 * @return  None
 */
void event_delete(Event_t *event)
{
    if (event)  /* Check if the event object is valid */
    {
        pthread_cond_destroy(&event->cond);  /* Destroy the condition variable */
        pthread_mutex_destroy(&event->mutex);  /* Destroy the mutex */
        free(event);  /* Free the memory allocated for the event object */
    }
}

// ........................................................................................................
/**
 * @brief   Signal an event
 *
 * Signals the condition variable associated with the event, waking up any waiting threads.
 *
 * @param   event  Pointer to the Event_t object to be signaled
 * @return  None
 */
void event_signal(Event_t *event)
{
    if (event)  /* Check if the event object is valid */
    {
        pthread_mutex_lock(&event->mutex);  /* Lock the mutex before signaling */
        pthread_cond_signal(&event->cond);  /* Signal the condition variable */
        pthread_mutex_unlock(&event->mutex);  /* Unlock the mutex after signaling */
    }
}

// ........................................................................................................
/**
 * @brief   Wait for an event
 *
 * Waits for the condition variable associated with the event to be signaled.
 *
 * @param   event  Pointer to the Event_t object to wait for
 * @return  None
 */
void event_wait(Event_t *event)
{
    if (event)  /* Check if the event object is valid */
    {
        pthread_mutex_lock(&event->mutex);  /* Lock the mutex before waiting */
        pthread_cond_wait(&event->cond, &event->mutex);  /* Wait for the condition variable to be signaled */
        pthread_mutex_unlock(&event->mutex);  /* Unlock the mutex after waiting */
    }
}
