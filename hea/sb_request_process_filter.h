//
// Created by cui on 17-9-21.
//
#ifndef SB_SERVER_SB_REQUEST_PROCESS_FILTER_H
#define SB_SERVER_SB_REQUEST_PROCESS_FILTER_H

#include <sb_collection.h>
#include <stdio.h>
#include "sb_client.h"
#include "sb_request.h"

#define FILTER void*(*filter)(sb_client*,sb_request*,void*)

static sb_arraylist *request_process_filters = NULL;

int sb_set_request_process_filters(sb_arraylist *filters);

int sb_add_method_req_process_filters(FILTER);

int sb_init_request_process_filters();

sb_arraylist* sb_get_request_process_filters();

#endif //SB_SERVER_SB_REQUEST_PROCESS_FILTER_H
