//
// Created by cui on 17-9-23.
//

#include <sb_request.h>
#include <sb_server.h>

int sb_init_request(sb_request *request){
    if(request == NULL){
        return fail;
    }
    return sb_init_key_value(&request->request_data)
            && sb_init_key_value(&request->attribute_data);
}

void* sb_get_request_parameter(const sb_request *request, const char *name){
    return sb_get_key_value(&request->request_data,name);
}

int sb_set_request_parameter(const sb_request *request, const char *name,const void *value){
    return sb_put_key_value(&request->request_data,name,value);
}

int sb_set_request_attribute(const sb_request *request, const char *name, const void *value){
    return sb_put_key_value(&request->attribute_data,name,value);
}

void* sb_get_request_attribute(const sb_request *request,const char *name){
    return sb_get_key_value(&request->attribute_data,name);
}