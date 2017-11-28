//
// Created by cui on 17-11-26.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "sb_static_resource.h"
#include "sb_dynamic_resource.h"

sb_resource* sb_new_resource(sb_resource_type type,const char* resource_name){
    if(resource_name == NULL || strcmp("",resource_name)==0)
        return NULL;
    sb_resource *result = (sb_resource*)malloc(sizeof(sb_resource));
    if(result!=NULL){
        result->type = type;
        result->resource_name = (char*)malloc(sizeof(char)*(strlen(resource_name)+1));
        strcpy(result->resource_name,resource_name);
        return result;
    }
    return NULL;
}

sb_resource** sb_find_resource(const char* target_name){
    sb_dynamic_resource *dy_resource = sb_find_dynamic_resource(target_name);
    if(dy_resource != NULL){
        return (sb_resource**) dy_resource;
    }
    sb_static_resource *sta_resource = sb_find_static_resource(target_name);
    if(sta_resource!=NULL){
        return (sb_resource**)sta_resource;
    }
    return NULL;
}
