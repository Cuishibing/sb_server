//
// Created by cui on 17-9-28.
//

#ifndef SB_SERVER_SB_WRITE_WORKER_H
#define SB_SERVER_SB_WRITE_WORKER_H

#include "sb_client.h"
#include "sb_worker.h"

typedef struct {
    sb_worker worker;
}sb_write_worker;

int sb_init_write_worker(sb_write_worker *write_worker);

int sb_add_write_event(sb_client *client);

static void* run (void *args);

#endif //SB_SERVER_SB_WRITE_WORKER_H