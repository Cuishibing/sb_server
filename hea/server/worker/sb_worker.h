//
// Created by cui on 17-9-17.
//

#ifndef SB_SERVER_SB_WORKER_H
#define SB_SERVER_SB_WORKER_H

#include <sys/types.h>
#include "sb_client.h"
#include "sb_collection.h"

typedef struct {
    pthread_t  thread;
    void*(*run)(void*);
    int is_exit;
}sb_worker;

//typedef struct {
//    sb_client clienter;
//}sb_worker_event;

int sb_init_worker(sb_worker *worker,pthread_attr_t *attr,void*(*run)(void*),void *args);

#endif //SB_SERVER_SB_WORKER_H
