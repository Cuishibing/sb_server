//
// Created by cui on 17-9-28.
//
#include <stdio.h>
#include <pthread.h>
#include <server/sb_server.h>
#include <server/request_parser/sb_response_builder.h>
#include <server/client/sb_client.h>
#include <unistd.h>
#include "sb_write_worker.h"
static sb_queue *write_worker_event_queue = NULL;
static pthread_mutex_t *write_worker_event_queue_mutex = NULL;
static pthread_cond_t *write_worker_event_queue_cond = NULL;
int sb_init_write_worker(sb_write_worker *write_worker){
    if(write_worker == NULL || run == NULL){
        return fail;
    }
    if(write_worker_event_queue == NULL){
        write_worker_event_queue = (sb_queue*)malloc(sizeof(sb_queue));
        if(write_worker_event_queue == NULL){
            error("内存不足!\n");
            return fail;
        }
        if(fail == sb_init_queue(write_worker_event_queue)){
            return fail;
        }
    }
    if(write_worker_event_queue_mutex == NULL){
        write_worker_event_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if(write_worker_event_queue_mutex == NULL){
            error("内存不足!\n");
            return fail;
        }
        pthread_mutex_init(write_worker_event_queue_mutex,NULL);//TODO:pthread_mutexattr_t
    }
    if(write_worker_event_queue_cond == NULL){
        write_worker_event_queue_cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
        if(write_worker_event_queue_cond == NULL){
            error("内存不足!\n");
            return fail;
        }
        pthread_cond_init(write_worker_event_queue_cond,NULL);//TODO:pthread_condattr_t
    }
    return sb_init_worker(&write_worker->worker,NULL,run,write_worker);
}

static void* run (void *args){
    sb_element client_store;
    sb_write_worker *thread_holder = (sb_write_worker*)args;
    if(thread_holder == NULL){
        error("thread_holder is null!\n");
        return NULL;
    }
    while(!thread_holder->worker.is_exit){
        pthread_mutex_lock(write_worker_event_queue_mutex);
        while(fail == sb_dequeue(write_worker_event_queue,&client_store)){
            pthread_cond_wait(write_worker_event_queue_cond,write_worker_event_queue_mutex);
        }
        pthread_mutex_unlock(write_worker_event_queue_mutex);
        sb_client *current_client = (sb_client*)client_store.value;
        response_builder* response_builder =  sb_get_response_builder();
        if(response_builder != NULL && response_builder->method_ptr != NULL){
            response_builder->method_ptr(current_client,current_client->target_res);
        }
        close(current_client->socket_fd);
    }
}

int sb_add_write_event(sb_client *client){
    if(client == NULL){
        return fail;
    }
    sb_element e;
    e.value = client;
    //TODO:入队也要同步
    if(success == sb_enqueue(write_worker_event_queue,e)){
        pthread_cond_signal(write_worker_event_queue_cond);
        return success;
    }else return fail;
}
