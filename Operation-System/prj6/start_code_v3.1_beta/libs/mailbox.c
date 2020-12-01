#include "string.h"
#include "mailbox.h"
#include "lock.h"

#define MAX_NUM_BOX 32

static mailbox_t mboxs[MAX_NUM_BOX];
static mutex_lock_t mboxs_mutex;


void mbox_init()
{
    int i;
    mutex_lock_init(&mboxs_mutex);
    for(i = 0; i < MAX_NUM_BOX; i++)
    {
        mutex_lock_init(&mboxs[i].mutex);
        condition_init(&mboxs[i].empty);
        condition_init(&mboxs[i].full);

        mboxs[i].name[0] = '\0';
        mboxs[i].front = 0;
        mboxs[i].rear = 0;
        mboxs[i].left_space = MAX_MSG_SZ;
        mboxs[i].user = 0;
    }
}


mailbox_t *mbox_open(char *name)
{
    int i;
    mutex_lock_acquire(&mboxs_mutex);
    for(i = 0; i < MAX_NUM_BOX; i++)
    {
        if(!strcmp(mboxs[i].name, name))
        {
            mboxs[i].user++;
            mutex_lock_release(&mboxs_mutex);
            return &mboxs[i];
        }
    }

    for(i = 0; i < MAX_NUM_BOX; i++)
    {
        if(mboxs[i].name[0] == '\0')
        {
            strcpy(mboxs[i].name, name);
            mboxs[i].user++;
            mutex_lock_release(&mboxs_mutex);
            return &mboxs[i];
        }
    }

    printf("No more left mailboxs\n");
}


void mbox_close(mailbox_t *mailbox)
{
    mutex_lock_acquire(&mboxs_mutex);
    --mailbox->user;
    if(mailbox->user <= 0)
    {
        mutex_lock_init(&mailbox->mutex);  
        condition_init(&mailbox->empty);
        condition_init(&mailbox->full);

        mailbox->name[0] = '\0';
        mailbox->front = 0;
        mailbox->rear = 0;
        mailbox->left_space = MAX_MSG_SZ;
        mailbox->user = 0;     
    }
    mutex_lock_release(&mboxs_mutex);
}

void mbox_send(mailbox_t *mailbox, void *msg, int msg_length)
{
    mutex_lock_acquire(&mailbox->mutex);
    while(mailbox->left_space < msg_length)
        condition_wait(&mailbox->mutex, &mailbox->full);

    if(MAX_MSG_SZ - mailbox->rear < msg_length)
    {
        memcpy((uint8_t *)(mailbox->msg + mailbox->rear), (uint8_t *)msg, MAX_MSG_SZ - mailbox->rear);
        mailbox->rear = (msg_length + mailbox->rear) % MAX_MSG_SZ;
        memcpy((uint8_t *)mailbox->msg, (uint8_t *)(msg + msg_length - mailbox->rear), mailbox->rear);
    }
    else
    {
        memcpy((uint8_t *)(mailbox->msg + mailbox->rear), (uint8_t *)msg, msg_length);
        mailbox->rear += msg_length;
    }

    mailbox->left_space -= msg_length;
    condition_broadcast(&mailbox->empty);
    mutex_lock_release(&mailbox->mutex);
}

void mbox_recv(mailbox_t *mailbox, void *msg, int msg_length)
{
    mutex_lock_acquire(&mailbox->mutex);
    while(MAX_MSG_SZ - mailbox->left_space < msg_length)
        condition_wait(&mailbox->mutex, &mailbox->empty);

    if(MAX_MSG_SZ - mailbox->front < msg_length)
    {
        memcpy((uint8_t *)msg, (uint8_t *)(mailbox->msg + mailbox->front), MAX_MSG_SZ - mailbox->front);
        mailbox->front = (msg_length + mailbox->front) % MAX_MSG_SZ;
        memcpy((uint8_t *)(msg + msg_length - mailbox->front), (uint8_t *)mailbox->msg, mailbox->front);
    }
    else
    {
        memcpy((uint8_t *)msg, (uint8_t *)(mailbox->msg + mailbox->front), msg_length);
        mailbox->front += msg_length;
    }

    mailbox->left_space += msg_length;
    condition_broadcast(&mailbox->full);
    mutex_lock_release(&mailbox->mutex);
}
