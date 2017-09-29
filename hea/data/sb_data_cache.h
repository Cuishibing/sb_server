//
// Created by cui on 17-9-17.
//

#ifndef SB_SERVER_SB_DATA_CACHE_H
#define SB_SERVER_SB_DATA_CACHE_H

#include <stddef.h>

typedef struct {
    char *data_poll;
    size_t length;
    size_t size;
}sb_data_cache;

static size_t DEFAULT_SIZE = 1024 << 1;

int sb_init_data_cache(sb_data_cache *cache);

static int expand_cache_size(sb_data_cache *cache);

int sb_put_data_cache(sb_data_cache *cache,char *buffer);

int sb_clear_data_cache(sb_data_cache *cache);

int sb_trim_data_cache(sb_data_cache *cache);

#endif //SB_SERVER_SB_DATA_CACHE_H
