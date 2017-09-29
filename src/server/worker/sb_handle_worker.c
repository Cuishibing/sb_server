//
// Created by cui on 17-9-21.
//


#include <sb_handle_worker.h>
#include <stdio.h>
#include <pthread.h>
#include <sb_request_process_filter.h>
#include <sb_server.h>

int sb_init_handle_worker(sb_handler_worker *handle_worker){
    if(handle_worker == NULL || run == NULL){
        return fail;
    }
    if(handle_worker_event_queue == NULL){
        handle_worker_event_queue = (sb_queue*)malloc(sizeof(sb_queue));
        if(handle_worker_event_queue == NULL){
            error("内存不足!\n");
            return fail;
        }
        if(fail == sb_init_queue(handle_worker_event_queue)){
            return fail;
        }
    }
    if(handle_worker_event_queue_mutex == NULL){
        handle_worker_event_queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        if(handle_worker_event_queue_mutex == NULL){
            error("内存不足!\n");
            return fail;
        }
        pthread_mutex_init(handle_worker_event_queue_mutex,NULL);//TODO:pthread_mutexattr_t
    }
    if(handle_worker_event_queue_cond == NULL){
        handle_worker_event_queue_cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
        if(handle_worker_event_queue_cond == NULL){
            error("内存不足!\n");
            return fail;
        }
        pthread_cond_init(handle_worker_event_queue_cond,NULL);//TODO:pthread_condattr_t
    }
    return sb_init_worker(&handle_worker->worker,NULL,run,handle_worker);//TODO:pthread_attr_t
}

int sb_add_handle_event(sb_client *client){
    if(client == NULL){
        return fail;
    }
    sb_element e;
    e.value = client;
    //TODO:入队也要同步
    if(success == sb_enqueue(handle_worker_event_queue,e)){
        pthread_cond_signal(handle_worker_event_queue_cond);
        return success;
    }else return fail;
}

static void* run (void *args){
    sb_element client_store;
    sb_handler_worker *thread_holder = (sb_handler_worker*)args;
    if(thread_holder == NULL){
        error("thread_holder is null!\n");
        return NULL;
    }
    /*
     * 从事件队列中不断的取出事件(待处理的clienter)
     * */
    while(!thread_holder->worker.is_exit){
        pthread_mutex_lock(handle_worker_event_queue_mutex);
        while(fail == sb_dequeue(handle_worker_event_queue,&client_store)){
            pthread_cond_wait(handle_worker_event_queue_cond,handle_worker_event_queue_mutex);
        }
        sb_client *current_client = (sb_client*)client_store.value;

        pthread_mutex_unlock(handle_worker_event_queue_mutex);

        sb_arraylist *request_process_filters = sb_get_request_process_filters();
        if(request_process_filters != NULL){
            sb_element element;
            FILTER;
            void* args = NULL;
            if(request_process_filters->length > 0){
                sb_get_arraylist(request_process_filters,0,&element);
                filter = element.value;
                args = filter(current_client,current_client->request,NULL);
            }
            for(int i=1;i<request_process_filters->length;i++){
                sb_get_arraylist(request_process_filters,i,&element);
                filter = element.value;
                if(filter != NULL)
                    args = filter(current_client,current_client->request,args);
            }
        }else{
            error("request process filters is not init！\n");
            return NULL;
        }

    }
    return NULL;
}
