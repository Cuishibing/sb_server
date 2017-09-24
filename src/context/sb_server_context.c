//
// Created by cui on 17-9-16.
//
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sb_server.h>
#include "sb_server_context.h"


int sb_init_server_context(sb_server_context *context){
    if(context == NULL){
        return fail;
    }
    return sb_init_key_value(&context->properties);
}

int sb_put_context_property(const sb_server_context *context,const char *name, const char *value){
    if(context != NULL){
        return sb_put_key_value(&context->properties,name,value);
    }
    return success;
}

char* sb_get_context_property(const sb_server_context *context,const char *name){
    if(context != NULL){
        return sb_get_key_value(&context->properties,name);
    }
    return NULL;
}

