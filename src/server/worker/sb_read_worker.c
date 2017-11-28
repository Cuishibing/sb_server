//
// Created by cui on 17-9-18.
//

#include <sb_read_worker.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sb_handle_worker.h>
#include <sb_server.h>
static sb_queue *read_worker_event_queue = NULL;
static pthread_mutex_t *read_worker_event_queue_mutex = NULL;
static pthread_cond_t *read_worker_event_queue_cond = NULL;
static size_t buffer_length = 512;
int sb_init_read_worker(sb_read_worker *read_worker){
    if(read_worker == NULL || run == NULL){
        return fail;
    }
    read_worker->buffer = (char*)calloc(buffer_length,sizeof(char));
    if(read_worker->buffer == NULL){
        error("内存不足!\n");
        return fail;
    }
    if(read_worker_event_queue == NULL){
        read_worker_event_queue = (sb_queue*)malloc(sizeof(sb_queue));
        if(read_worker_event_queue == NULL){
            error("内存不足!");
            return fail;
        }
        if(fail == sb_init_queue(read_worker_event_queue)){
            return fail;
        }
    }
    if(read_worker_event_queue_mutex == NULL){
        read_worker_event_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if(read_worker_event_queue_mutex == NULL){
            error("内存不足!\n");
            return fail;
        }
        pthread_mutex_init(read_worker_event_queue_mutex,NULL);//TODO:pthread_mutexattr_t
    }
    if(read_worker_event_queue_cond == NULL){
        read_worker_event_queue_cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
        if(read_worker_event_queue_cond == NULL){
            error("内存不足!\n");
            return fail;
        }
        pthread_cond_init(read_worker_event_queue_cond,NULL);//TODO:pthread_condattr_t
    }
    return sb_init_worker(&read_worker->worker,NULL,run,read_worker);//TODO:pthread_attr_t
}

static void* run (void *args){
    sb_element client_store;
    sb_read_worker *thread_holder = (sb_read_worker*)args;
    if(thread_holder == NULL){
        error("thread_holder is null!\n");
        return NULL;
    }
    /*
     * 从事件队列中不断的取出事件(读取数据的clienter),并读取它们。取事件时要同步。
     * */
    while(!thread_holder->worker.is_exit){
        pthread_mutex_lock(read_worker_event_queue_mutex);
        while(fail == sb_dequeue(read_worker_event_queue,&client_store)){
            pthread_cond_wait(read_worker_event_queue_cond,read_worker_event_queue_mutex);
        }

        pthread_mutex_unlock(read_worker_event_queue_mutex);

        sb_client *current_client = (sb_client*)client_store.value;

        ssize_t len = 0;
        do{
            len = read(current_client->socket_fd,thread_holder->buffer,buffer_length);
            if(fail == sb_put_data_cache(current_client->data_cache,thread_holder->buffer)){
                error("store data error!\n");
                return NULL;
            }
        }while(len != -1 && len != 0);
        //fprintf(stderr,"%d    %s",current_client->socket_fd,current_client->data_cache->data_poll);
        sb_add_handle_event(current_client);
    }
    return NULL;
}

int sb_add_read_event(sb_client *client){
    if(client == NULL){
        return fail;
    }
    sb_element e;
    e.value = client;
    //TODO:入队也要同步
    if(success == sb_enqueue(read_worker_event_queue,e)){
        pthread_cond_signal(read_worker_event_queue_cond);
        return success;
    }else return fail;
}