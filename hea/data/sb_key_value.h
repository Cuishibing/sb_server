//
// Created by cui on 17-9-20.
//

#ifndef SB_SERVER_SB_KEY_VALUE_H
#define SB_SERVER_SB_KEY_VALUE_H

#include "sb_tree.h"
#include "sb_collection.h"

typedef struct {
    sb_map pool;
    sb_arraylist key_set;
}sb_key_value;

int sb_init_key_value(sb_key_value *key_value);

int sb_put_key_value(const sb_key_value *key_value,const char *key, const void *value);

/*
 * 根据key删除value，不会释放key和value
 * 调用者需要手动释放内存以免出现内存泄漏
 * * */
int sb_remove_key_value(const sb_key_value *key_value,const char *key);

void *sb_get_key_value(const sb_key_value *key_value,const char *key);

#endif //SB_SERVER_SB_KEY_VALUE_H
