//
// Created by cui on 17-11-27.
//
#include <stdlib.h>
#include "sb_server.h"
#include "sb_request_parser.h"

/**
 * 请求解析的接口
 * */
static request_parser* request_parser_interface = NULL;

int sb_set_request_parser(int(*method_ptr)(void *)){
    if(method_ptr != NULL){
        if(request_parser_interface == NULL){
            request_parser_interface = sb_new_request_parser(method_ptr);
        }else{
            request_parser_interface->method_ptr = method_ptr;
        }
        return success;
    }
    return fail;
}

request_parser* sb_get_request_parser(){
    return request_parser_interface;
}

request_parser* sb_new_request_parser(int(*method_ptr)(void *)){
    if(method_ptr == NULL)
        return NULL;
    request_parser *method = (request_parser*)malloc(sizeof(request_parser));
    if(method!=NULL){
        method->method_ptr = method_ptr;
    }
    return method;
}