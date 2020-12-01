#ifndef INCLUDE_MAIL_BOX_
#define INCLUDE_MAIL_BOX_

#include "cond.h"

#define MAX_MSG_SZ 100

typedef struct mailbox
{   
    mutex_lock_t mutex;
    condition_t empty;
    condition_t full;

    char name[20];
    char msg[MAX_MSG_SZ];
    int front;
    int rear;
    int left_space;
    int user;

} mailbox_t;


void mbox_init();
mailbox_t *mbox_open(char *);
void mbox_close(mailbox_t *);
void mbox_send(mailbox_t *, void *, int);
void mbox_recv(mailbox_t *, void *, int);

#endif