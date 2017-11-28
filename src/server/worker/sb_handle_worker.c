//
// Created by cui on 17-9-21.
//

#include <stdio.h>
#include <pthread.h>
#include <server/client/sb_client.h>
#include <server/sb_request.h>
#include <data/sb_key_value.h>
#include <data/sb_data_cache.h>
#include <server/request_parser/sb_response_builder.h>
#include <server/resource/sb_dynamic_resource.h>
#include "sb_request_parser.h"
#include "sb_dispatcher.h"
#include "sb_handle_worker.h"

static sb_queue *handle_worker_event_queue = NULL;
static pthread_mutex_t *handle_worker_event_queue_mutex = NULL;
static pthread_cond_t *handle_worker_event_queue_cond = NULL;

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
     * 从事件队列中不断的取出事件(待处理的client)
     * */
    while(!thread_holder->worker.is_exit){
        pthread_mutex_lock(handle_worker_event_queue_mutex);
        while(fail == sb_dequeue(handle_worker_event_queue,&client_store)){
            pthread_cond_wait(handle_worker_event_queue_cond,handle_worker_event_queue_mutex);
        }
        sb_client *current_client = (sb_client*)client_store.value;

        pthread_mutex_unlock(handle_worker_event_queue_mutex);
        request_parser* request_parser = sb_get_request_parser();
        if(request_parser!=NULL&&request_parser->method_ptr!=NULL){
            if(success == request_parser->method_ptr(current_client)){
                //解析成功
                const char* target = sb_get_request_parameter(current_client->request,REQUEST_TARGET);
                sb_resource** res = sb_find_resource(target);
                if(res != NULL){
                    current_client->target_res = res;
                    struct epoll_event ev;
                    ev.events = EPOLLOUT | EPOLLET;
                    sb_mod_epoll_event(current_client->socket_fd,&ev);
                }
            }else{
                //解析失败
            }
        }

    }
    return NULL;
}
