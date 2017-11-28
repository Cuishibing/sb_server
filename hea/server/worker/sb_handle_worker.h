//
// Created by cui on 17-9-21.
//

#ifndef SB_SERVER_SB_HANDLE_WORKER_H
#define SB_SERVER_SB_HANDLE_WORKER_H

#include "sb_worker.h"

typedef struct {
    sb_worker worker;
}sb_handler_worker;

int sb_init_handle_worker(sb_handler_worker *read_worker);

int sb_add_handle_event(sb_client *client);

static void* run (void *args);

#endif //SB_SERVER_SB_HANDLE_WORKER_H