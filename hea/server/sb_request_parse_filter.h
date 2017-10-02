//
// Created by cui on 17-9-21.
//
#ifndef SB_SERVER_SB_REQUEST_PARSE_FILTER_H
#define SB_SERVER_SB_REQUEST_PARSE_FILTER_H

#include <sb_collection.h>
#include <stdio.h>
#include "sb_client.h"
#include "sb_request.h"

#define FILTER void*(*filter)(sb_client*,sb_request*,void*)

static sb_arraylist *request_parse_filters = NULL;

int sb_set_request_parse_filters(sb_arraylist *filters);

int sb_add_method_req_parse_filters(FILTER);

int sb_init_request_parse_filters();

sb_arraylist* sb_get_request_parse_filters();

#endif //SB_SERVER_SB_REQUEST_PROCESS_FILTER_H
