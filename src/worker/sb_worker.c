//
// Created by cui on 17-9-18.
//

#include <pthread.h>
#include <sb_worker.h>
#include <stdio.h>
#include "sb_worker.h"

int sb_init_worker(sb_worker *worker,pthread_attr_t *attr,void*(*run)(void*),void *args){
    if(worker == NULL || run == NULL){
        return 0;
    }
    worker->run = run;
    worker->is_exit = 0;
    //pthread_create成功返回0，失败返回的是错误代码
    if(pthread_create(&worker->thread,attr,worker->run,args)){
        fprintf(stderr,"pthread create失败!");
        return 0;
    }
    return 1;
}