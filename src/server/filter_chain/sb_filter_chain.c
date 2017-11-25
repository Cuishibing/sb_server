//
// Created by cui on 17-10-2.
//

#include <server/filter_chain/sb_filter_chain.h>
#include <server/sb_server.h>
#include <stdio.h>

int sb_add_filter(sb_filter_chain *filter_chain,FILTER){
    if(filter_chain == NULL || filter == NULL){
        return fail;
    }
    int filter_type = filter_chain->filter_type;
    if(filter_type != FILTER_TYPE_0){
        if(filter_type == DEFAULT_FILTER_TYPE){
            filter_chain->filter_type = FILTER_TYPE_0;
        } else{
            error("过滤器类型不符!");
            return fail;
        }
    }
    sb_element element;
    element.value = filter;
    return sb_insert_arraylist(&filter_chain->filter_chain,element,filter_chain->filter_chain.length);
}

int sb_add_filter_1(sb_filter_chain *filter_chain,FILTER_1){
    if(filter_chain == NULL || filter == NULL){
        return fail;
    }
    int filter_type = filter_chain->filter_type;
    if(filter_type != FILTER_TYPE_1){
        if(filter_type == DEFAULT_FILTER_TYPE){
            filter_chain->filter_type = FILTER_TYPE_1;
        } else{
            error("过滤器类型不符!");
            return fail;
        }
    }
    sb_element element;
    element.value = filter;
    return sb_insert_arraylist(&filter_chain->filter_chain,element,filter_chain->filter_chain.length);
}

int sb_init_filter_chain(sb_filter_chain *filter_chain){
    if(filter_chain == NULL)
        return fail;
    filter_chain->filter_type = DEFAULT_FILTER_TYPE;
    return sb_init_arraylist(&filter_chain->filter_chain,10);
}

void* sb_invoke_filter_chain(sb_filter_chain *filter_chain,sb_client *client,void* arg){
    if(filter_chain == NULL)
        return NULL;
    sb_element element;
    FILTER;
    for(int i=0;i<filter_chain->filter_chain.length;i++){
        sb_get_arraylist(&filter_chain->filter_chain,i,&element);
        filter = element.value;
        arg = filter(client,arg);
    }
    return arg;
}