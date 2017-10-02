//
// Created by cui on 17-10-2.
//
#include "sb_server.h"
#include "filter_chain/sb_before_action_filter.h"

int sb_set_before_action_filters(sb_filter_chain *filter_chain){
    if(filter_chain == NULL){
        return fail;
    }
    if(before_action_filter_chain == NULL){
        before_action_filter_chain = filter_chain;
        return success;
    }
    return fail;
}

int sb_add_method_before_action_filters(FILTER){
    return sb_add_filter(before_action_filter_chain,filter);
}

int sb_init_before_action_filters(){
    if(before_action_filter_chain == NULL){
        before_action_filter_chain = (sb_filter_chain*)malloc(sizeof(sb_filter_chain));
        if(before_action_filter_chain == NULL){
            error("内存不足!");
            return fail;
        }
        return sb_init_filter_chain(before_action_filter_chain);
    }
}

sb_filter_chain* sb_get_before_action_filters(){
    return before_action_filter_chain;
}