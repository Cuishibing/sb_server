//
// Created by cui on 17-9-18.
//

#include <sb_read_worker.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sb_client.h>
#include <sys/socket.h>
#include <sb_handle_worker.h>

int sb_init_read_worker(sb_read_worker *read_worker){
    if(read_worker == NULL || run == NULL){
        return 0;
    }
    read_worker->buffer = (char*)calloc(buffer_length,sizeof(char));
    if(read_worker->buffer == NULL){
        fprintf(stderr,"内存不足!\n");
        return 0;
    }
    if(read_worker_event_queue == NULL){
        read_worker_event_queue = (sb_queue*)malloc(sizeof(sb_queue));
        if(read_worker_event_queue == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        if(!sb_init_queue(read_worker_event_queue)){
            return 0;
        }
    }
    if(read_worker_event_queue_mutex == NULL){
        read_worker_event_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if(read_worker_event_queue_mutex == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        pthread_mutex_init(read_worker_event_queue_mutex,NULL);//TODO:pthread_mutexattr_t
    }
    if(read_worker_event_queue_cond == NULL){
        read_worker_event_queue_cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
        if(read_worker_event_queue_cond == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        pthread_cond_init(read_worker_event_queue_cond,NULL);//TODO:pthread_condattr_t
    }
    return sb_init_worker(&read_worker->worker,NULL,run,read_worker);//TODO:pthread_attr_t
}

static void* run (void *args){
    sb_element client_store;
    sb_read_worker *thread_holder = (sb_read_worker*)args;
    if(thread_holder == NULL){
        fprintf(stderr,"thread_holder is null!");
        return NULL;
    }
    /*
     * 从事件队列中不断的取出事件(读取数据的clienter),并读取它们。取事件时要同步。
     * */
    while(!thread_holder->worker.is_exit){
        pthread_mutex_lock(read_worker_event_queue_mutex);
        while(!sb_dequeue(read_worker_event_queue,&client_store)){
            pthread_cond_wait(read_worker_event_queue_cond,read_worker_event_queue_mutex);
        }

        pthread_mutex_unlock(read_worker_event_queue_mutex);

        sb_client *current_client = (sb_client*)client_store.value;

        int len = 0;
        do{
            len = read(current_client->socket_fd,thread_holder->buffer,buffer_length);
            if(!sb_put_data_cache(current_client->request_data,thread_holder->buffer)){
                fprintf(stderr,"store data error!\n");
                return NULL;
            }
        }while(len != -1 && len != 0);
        //fprintf(stderr,"%d    %s",current_client->socket_fd,current_client->request_data->data_poll);
        sb_add_handle_event(current_client);
    }
    return NULL;
}

int sb_add_read_event(sb_client *client){
    if(client == NULL){
        return 0;
    }
    sb_element e;
    e.value = client;
    //TODO:入队也要同步
    if(sb_enqueue(read_worker_event_queue,e)){
        pthread_cond_signal(read_worker_event_queue_cond);
        return 1;
    }else return 0;
}