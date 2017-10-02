//
// Created by cui on 17-10-2.
//

#ifndef SB_SERVER_SB_BEFORE_ACTION_FILTER_H
#define SB_SERVER_SB_BEFORE_ACTION_FILTER_H

#include <sb_collection.h>
#include <stddef.h>
#include "sb_request_parse_filter.h"

static sb_filter_chain *before_action_filter_chain = NULL;

int sb_set_before_action_filters(sb_filter_chain *filter_chain);

int sb_add_method_before_action_filters(FILTER);

int sb_init_before_action_filters();

sb_filter_chain* sb_get_before_action_filters();

#endif //SB_SERVER_SB_BEFORE_ACTION_FILTER_H
