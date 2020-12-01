#ifndef INCLUDE_COND_H_
#define INCLUDE_COND_H_

#include "lock.h"
#include "queue.h"

typedef struct condition
{
    queue_t cond_queue;
} condition_t;

void do_condition_init(condition_t *condition);
void do_condition_wait(mutex_lock_t *lock, condition_t *condition);
void do_condition_signal(condition_t *condition);
void do_condition_broadcast(condition_t *condition);

#endif