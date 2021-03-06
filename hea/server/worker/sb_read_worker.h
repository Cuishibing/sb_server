//
// Created by cui on 17-9-18.
//

#ifndef SB_SERVER_SB_READ_WORKER_H
#define SB_SERVER_SB_READ_WORKER_H

#include "sb_worker.h"
#include "sb_collection.h"

typedef struct {
    sb_worker worker;
    char *buffer;
}sb_read_worker;

int sb_init_read_worker(sb_read_worker *read_worker);

int sb_add_read_event(sb_client *client);

static void* run (void *args);

#endif //SB_SERVER_SB_READ_WORKER_H