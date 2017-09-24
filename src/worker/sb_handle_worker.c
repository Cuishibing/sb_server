//
// Created by cui on 17-9-21.
//


#include <sb_handle_worker.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sb_request_process_filter.h>
#include <sb_request.h>

int sb_init_handle_worker(sb_handler_worker *handle_worker){
    if(handle_worker == NULL || run == NULL){
        return 0;
    }
    if(handle_worker_event_queue == NULL){
        handle_worker_event_queue = (sb_queue*)malloc(sizeof(sb_queue));
        if(handle_worker_event_queue == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        if(!sb_init_queue(handle_worker_event_queue)){
            return 0;
        }
    }
    if(handle_worker_event_queue_mutex == NULL){
        handle_worker_event_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if(handle_worker_event_queue_mutex == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        pthread_mutex_init(handle_worker_event_queue_mutex,NULL);//TODO:pthread_mutexattr_t
    }
    if(handle_worker_event_queue_cond == NULL){
        handle_worker_event_queue_cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
        if(handle_worker_event_queue_cond == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        pthread_cond_init(handle_worker_event_queue_cond,NULL);//TODO:pthread_condattr_t
    }
    return sb_init_worker(&handle_worker->worker,NULL,run,handle_worker);//TODO:pthread_attr_t
}

int sb_add_handle_event(sb_client *client){
    if(client == NULL){
        return 0;
    }
    sb_element e;
    e.value = client;
    //TODO:入队也要同步
    if(sb_enqueue(handle_worker_event_queue,e)){
        pthread_cond_signal(handle_worker_event_queue_cond);
        return 1;
    }else return 0;
}

static void* run (void *args){
    sb_element client_store;
    sb_handler_worker *thread_holder = (sb_handler_worker*)args;
    if(thread_holder == NULL){
        fprintf(stderr,"thread_holder is null!");
        return NULL;
    }
    /*
     * 从事件队列中不断的取出事件(待处理的clienter)
     * */
    while(!thread_holder->worker.is_exit){
        pthread_mutex_lock(handle_worker_event_queue_mutex);
        while(!sb_dequeue(handle_worker_event_queue,&client_store)){
            pthread_cond_wait(handle_worker_event_queue_cond,handle_worker_event_queue_mutex);
        }
        sb_client *current_client = (sb_client*)client_store.value;

        pthread_mutex_unlock(handle_worker_event_queue_mutex);

        sb_arraylist *request_process_filters = sb_get_request_process_filters();
        if(request_process_filters != NULL){
            sb_element element;
            sb_request *request = (sb_request*)malloc(sizeof(sb_request));
            if(request == NULL){
                fprintf(stderr,"内存不足!");
                return NULL;
            }
            sb_init_request(request);
            FILTER;
            void* args = NULL;
            if(request_process_filters->length > 0){
                sb_get_arraylist(request_process_filters,0,&element);
                filter = element.value;
                args = filter(current_client,request,NULL);
            }
            for(int i=1;i<request_process_filters->length;i++){
                sb_get_arraylist(request_process_filters,i,&element);
                filter = element.value;
                if(filter != NULL)
                    args = filter(current_client,request,args);
            }

        }else{
            fprintf(stderr,"request process filters is not init！");
            return NULL;
        }

    }
    return NULL;
}
