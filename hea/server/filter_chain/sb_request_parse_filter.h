//
// Created by cui on 17-9-21.
//
#ifndef SB_SERVER_SB_REQUEST_PARSE_FILTER_H
#define SB_SERVER_SB_REQUEST_PARSE_FILTER_H

#include <sb_collection.h>
#include <stdio.h>
#include "filter_chain/sb_filter_chain.h"
#include "sb_client.h"
#include "sb_request.h"

static sb_filter_chain *request_builder = NULL;

int sb_set_request_builder(sb_filter_chain *filter_chain);

int sb_add_filter_request_builder(FILTER);

int sb_init_request_builder();

sb_filter_chain* sb_get_request_builder();

int invoke_request_builder(sb_client *client);

static const char *REQUEST_TARGET = "request_target";

#endif //SB_SERVER_SB_REQUEST_PROCESS_FILTER_H
