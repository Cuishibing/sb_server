//
// Created by cui on 17-9-21.
//

#include <stdlib.h>
#include "sb_request_process_filter.h"

int sb_set_request_process_filters(sb_arraylist *filters){
    if(filters == NULL){
        return 0;
    }
    if(request_process_filters == NULL){
        request_process_filters = filters;
        return 1;
    }
    return 0;
}

int sb_add_method_req_process_filters(FILTER){
    if(filter == NULL){
        return 0;
    }
    sb_element element;
    element.value = filter;
    return sb_insert_arraylist(request_process_filters,element,request_process_filters->length);
}

int sb_init_request_process_filters(){
    if(request_process_filters == NULL){
        request_process_filters = (sb_arraylist*)malloc(sizeof(sb_arraylist));
        if(request_process_filters == NULL){
            fprintf(stderr,"内存不足!");
            return 0;
        }
        return sb_init_arraylist(request_process_filters,5);
    }
    return 1;
}

sb_arraylist* sb_get_request_process_filters(){
    return request_process_filters;
}