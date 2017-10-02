//
// Created by cui on 17-9-21.
//

#include <stdlib.h>
#include <sb_server.h>
#include "sb_request_process_filter.h"

int sb_set_request_process_filters(sb_arraylist *filters){
    if(filters == NULL){
        return fail;
    }
    if(request_process_filters == NULL){
        request_process_filters = filters;
        return success;
    }
    return fail;
}

int sb_add_method_req_process_filters(FILTER){
    if(filter == NULL){
        return fail;
    }
    sb_element element;
    element.value = filter;
    return sb_insert_arraylist(request_process_filters,element,request_process_filters->length);
}

int sb_init_request_process_filters(){
    if(request_process_filters == NULL){
        request_process_filters = (sb_arraylist*)malloc(sizeof(sb_arraylist));
        if(request_process_filters == NULL){
            error("内存不足!\n");
            return fail;
        }
        return sb_init_arraylist(request_process_filters,5);
    }
    return success;
}

sb_arraylist* sb_get_request_process_filters(){
    return request_process_filters;
}