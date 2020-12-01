#include "queue.h"
#include "sched.h"

#define MAXRAND 72

typedef pcb_t item_t;

void queue_init(queue_t *queue)
{
    queue->head = queue->tail = NULL;
}

int queue_is_empty(queue_t *queue)
{
    if (queue->head == NULL)
    {
        return 1;
    }
    return 0;
}

void queue_push(queue_t *queue, void *item)
{
    item_t *_item = (item_t *)item;
    /* queue is empty */
    if (queue->head == NULL)
    {
        queue->head = item;
        queue->tail = item;
        _item->next = NULL;
        _item->prev = NULL;
    }
    else
    {
        ((item_t *)(queue->tail))->next = item;
        _item->next = NULL;
        _item->prev = queue->tail;
        queue->tail = item;
    }
}


void priority_queue_push(void *item)
{
    item_t *_item = (item_t *)item;

    int priority = _item->priority - 1;//priority from 1 to 5

//     queue is empty 
    if (ready_queue[priority].head == NULL)
    {
        ready_queue[priority].head = item;
        ready_queue[priority].tail = item;
        _item->next = NULL;
        _item->prev = NULL;
    }
    else
    {
        ((item_t *)(ready_queue[priority].tail))->next = item;
        _item->next = NULL;
        _item->prev = ready_queue[priority].tail;
        ready_queue[priority].tail = item;
    }
}


int is_in_queue(queue_t *queue, void *item)
{
    /* queue is empty */
    if(queue_is_empty(queue))
        return FALSE;

    int find = FALSE;
    item_t *temp = (item_t *)(queue->head);
    do{
        if(temp == item)
        {
            find = TRUE;
                break;
        }
        else
            temp = temp->next;                  
    }while(temp); 

    return find;   
}

void *queue_dequeue(queue_t *queue)
{
    item_t *temp = (item_t *)queue->head;

    /* this queue only has one item */
    if (temp->next == NULL)
    {
        queue->head = queue->tail = NULL;
    }
    else
    {
        queue->head = ((item_t *)(queue->head))->next;
        ((item_t *)(queue->head))->prev = NULL;
    }

    temp->prev = NULL;
    temp->next = NULL;

    return (void *)temp;
}


void *priority_queue_dequeue(void)
{
    int valid[MAXPRIORITY];
    int i;
    for(i = 0;i < MAXPRIORITY;i++)
    {
        valid[i] = !queue_is_empty(&(ready_queue[i]));
    }

    do{
        int rand = GET_CP0_COUNT() % MAXRAND;
        
        if(0 < rand && rand <= 10 && valid[0])
        {
            return queue_dequeue(&(ready_queue[0]));
        }
        else if(11 < rand && rand <= 22 && valid[1])
        {
            return queue_dequeue(&(ready_queue[1]));
        }
        else if(23 < rand && rand <= 36 && valid[2])
        {
            return queue_dequeue(&(ready_queue[2]));
        }
        else if(37 < rand && rand <= 52 && valid[3])
        {
            return queue_dequeue(&(ready_queue[3]));
        }
        else if(53 < rand && rand <= 71 && valid[4])
        {
            return queue_dequeue(&(ready_queue[4]));
        }

    }while(1);

}


/* remove this item and return next item */
void *queue_remove(queue_t *queue, void *item)
{
    item_t *_item = (item_t *)item;
    item_t *next = (item_t *)_item->next;

    if (item == queue->head && item == queue->tail)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else if (item == queue->head)
    {
        queue->head = _item->next;
        ((item_t *)(queue->head))->prev = NULL;
    }
    else if (item == queue->tail)
    {
        queue->tail = _item->prev;
        ((item_t *)(queue->tail))->next = NULL;
    }
    else
    {
        ((item_t *)(_item->prev))->next = _item->next;
        ((item_t *)(pcb->next))->prev = _item->prev;
    }

    _item->prev = NULL;
    _item->next = NULL;

    return (void *)next;
}
