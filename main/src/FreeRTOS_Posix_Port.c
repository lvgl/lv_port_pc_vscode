#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Event
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} Event_t;

Event_t *event_create(void)
{
    Event_t *event = (Event_t *)malloc(sizeof(Event_t));
    if (event)
    {
        pthread_cond_init(&event->cond, NULL);
        pthread_mutex_init(&event->mutex, NULL);
    }
    return event;
}

void event_delete(Event_t *event)
{
    if (event)
    {
        pthread_cond_destroy(&event->cond);
        pthread_mutex_destroy(&event->mutex);
        free(event);
    }
}

void event_signal(Event_t *event)
{
    if (event)
    {
        pthread_mutex_lock(&event->mutex);
        pthread_cond_signal(&event->cond);
        pthread_mutex_unlock(&event->mutex);
    }
}

void event_wait(Event_t *event)
{
    if (event)
    {
        pthread_mutex_lock(&event->mutex);
        pthread_cond_wait(&event->cond, &event->mutex);
        pthread_mutex_unlock(&event->mutex);
    }
}
