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
    sb_element element;
    element.value = filter;
    return sb_insert_arraylist(&filter_chain->filter_chain,element,filter_chain->filter_chain.length);
}

int sb_init_filter_chain(sb_filter_chain *filter_chain){
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