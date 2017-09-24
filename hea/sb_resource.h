//
// Created by cui on 17-9-23.
//

#ifndef SB_SERVER_SB_RESOURCE_H
#define SB_SERVER_SB_RESOURCE_H

#include <sb_tree.h>
#include "sb_data_cache.h"

static sb_map *sb_resource_cache = NULL;

static const size_t BUFFER_SIZE = 512;

typedef struct {
    char *name;
    sb_data_cache data;
}sb_resource;

sb_resource* sb_get_resource(const char* root_path,const char* name);

#endif //SB_SERVER_SB_RESOURCE_H
