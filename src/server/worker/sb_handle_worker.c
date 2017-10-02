//
// Created by cui on 17-9-21.
//

#include <stdio.h>
#include <pthread.h>
#include <server/filter_chain/sb_before_action_filter.h>
#include <server/client/sb_client.h>
#include <http/sb_http.h>
#include <data/sb_resource.h>
#include <data/sb_data_cache.h>
#include "filter_chain/sb_request_parse_filter.h"
#include "sb_server.h"
#include "sb_handle_worker.h"

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

/*
 *把请求交给过滤器来处理(before_action_filter)
 * */
int put_request_before_action_filter(sb_client *client){
    if(client == NULL){
        return fail;
    }
    sb_filter_chain *before_action_filter = sb_get_before_action_filters();
    if(before_action_filter != NULL){
        sb_element element;
        void *result_arg = NULL;
        for(int i=0;i<before_action_filter->filter_chain.length;i++){
            sb_get_arraylist(&before_action_filter->filter_chain,i,&element);
            FILTER = element.value;
            result_arg = filter(client,result_arg);
        }
        return success;
    }
    return fail;
}

/*
 * 把请求给请求解析过滤器处理
 * */
int put_request_request_parse_filter(sb_client *client){
    if(client == NULL){
        return fail;
    }
    sb_filter_chain *request_parse_filter = sb_get_request_parse_filters();
    if(request_parse_filter != NULL){
        sb_element element;
        void *result_arg = NULL;
        for(int i=0;i<request_parse_filter->filter_chain.length;i++){
            sb_get_arraylist(&request_parse_filter->filter_chain,i,&element);
            FILTER = element.value;
            result_arg = filter(client,result_arg);
        }
        return success;
    }
    return fail;
}

/*
 * 根据请求的参数来寻找资源
 * 资源包括静态资源也包括函数(action)
 * 优先匹配静态资源
 * */
int search_resource_by_target(sb_client *client){
    char *target = sb_get_request_parameter(client->request,REQUEST_TARGET);
    if(target != NULL){
        sb_resource *target_resource = sb_get_resource(target);
        if(target_resource != NULL){
            //找到了静态资源
            sb_clear_data_cache(client->data_cache);
            sb_put_data_cache(client->data_cache,"200 HTTP/1.1 OK\r\n\r\n");
            sb_put_data_cache(client->data_cache,target_resource->data.data_poll);
        }
    }
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

        if(put_request_request_parse_filter(current_client)){
            /*
             * 在一系列请求处理方法处理完后应该返回一个sb_request对象,然后把这个request对象传递给filter处理入口函数,
             * 再传递给action处理入口函数.
             * */
            put_request_before_action_filter(current_client);

        }else{
            error("请求解析错误!\n");
        }

    }
    return NULL;
}
