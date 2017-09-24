//
// Created by cui on 17-9-23.
//

#include <sb_client.h>
#include <sb_data_cache.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sb_request_process_filter.h>
#include <sb_request.h>
#include <sb_key_value.h>
#include "sb_request.h"
#include "sb_client.h"
#include "sb_http.h"

/*
 * 这是第一个过滤器链
 * */
void* sb_parse_http_start(sb_client *client, sb_request *request, void *args){
    char *first_cursor = client->request_data->data_poll;
    char *second_cursor = NULL;
    second_cursor = strstr(first_cursor," ");
    if(second_cursor == NULL){
        fprintf(stderr,"bad request");
        return NULL;
    }
    char *method = (char*)calloc((second_cursor - first_cursor + 1),sizeof(char));
    strncpy(method,first_cursor,second_cursor - first_cursor);

    sb_set_request_parameter(request,REQUEST_METHOD,method);
    first_cursor = second_cursor + 1;
    second_cursor = NULL;

    second_cursor = strstr(first_cursor," ");
    if(second_cursor == NULL){
        fprintf(stderr,"bad request");
        return NULL;
    }
    char *url = (char*)calloc((second_cursor - first_cursor + 1),sizeof(char));

    strncpy(url,first_cursor,second_cursor - first_cursor);

    sb_set_request_parameter(request,REQUEST_URL,url);
    first_cursor = second_cursor + 1;
    second_cursor = NULL;

    second_cursor = strstr(first_cursor,"\r\n");
    if(second_cursor == NULL){
        fprintf(stderr,"bad request");
        return NULL;
    }
    char *http_version = (char*)calloc((second_cursor - first_cursor + 1),sizeof(char));
    strncpy(http_version,first_cursor,second_cursor - first_cursor);
    sb_set_request_parameter(request,REQUEST_VERSION,http_version);

    return second_cursor + 2;
}

void* sb_parse_http_head(sb_client *client,sb_request *request,void *args){
    char *first_cursor = (char*)args;
    char *middle_cursor = NULL;
    char *last_cursor = NULL;
    if(first_cursor == NULL){
        return NULL;
    }
    sb_key_value *heads = (sb_key_value*)malloc(sizeof(sb_key_value));
    if(heads == NULL){
        fprintf(stderr,"内存不足!");
        return NULL;
    }
    if(!sb_init_key_value(heads)){
        return NULL;
    }
    while(1){
        last_cursor = strstr(first_cursor,"\r\n");
        if(last_cursor == NULL){
            return NULL;
        }
        middle_cursor = strstr(first_cursor,":");
        if(middle_cursor == NULL){
            return NULL;
        }
        if(middle_cursor >= last_cursor){
            return NULL;
        }
        char *head_name = (char*)calloc(middle_cursor - first_cursor,sizeof(char));
    }
    return NULL;
}

void* sb_parse_http_body(sb_client *client,sb_request *request,void *args){
    return NULL;
}

int sb_init_http_filters(){
    if(sb_init_request_process_filters()){
        sb_add_method_req_process_filters(sb_parse_http_start);
        sb_add_method_req_process_filters(sb_parse_http_head);
        sb_add_method_req_process_filters(sb_parse_http_body);
        return 1;
    }
    return 0;
}