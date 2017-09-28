//
// Created by cui on 17-9-28.
//

#ifndef SB_SERVER_SB_WRITE_WORKER_H
#define SB_SERVER_SB_WRITE_WORKER_H

#include "sb_client.h"
#include "sb_worker.h"

static sb_queue *write_worker_event_queue = NULL;
static pthread_mutex_t *write_worker_event_queue_mutex = NULL;
static pthread_cond_t *write_worker_event_queue_cond = NULL;

typedef struct {
    sb_worker worker;
}sb_write_worker;

int sb_init_write_worker(sb_write_worker *write_worker);

int sb_add_write_event(sb_client *client);

static void* run (void *args);

#endif //SB_SERVER_SB_WRITE_WORKER_H
