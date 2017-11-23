//
// Created by cui on 17-9-20.
//

#ifndef SB_SERVER_SB_KEY_VALUE_H
#define SB_SERVER_SB_KEY_VALUE_H

#include "sb_tree.h"
#include "sb_collection.h"

/**
 * 一个简单的键值对类,可以提供key set
 * */
typedef struct {
    sb_map pool;
    sb_arraylist key_set;
}sb_key_value;

/**
 * 初始化sb_key_value
 * @param key_value 要初始化的对象
 * @return success 成功,fail 失败
 * */
int sb_init_key_value(sb_key_value *key_value);

/**
 * 向key_value中存放值
 * @param key_value 要存放键值对的对象
 * @param key key
 * @param value value
 * @return success 成功,fail 失败
 * */
int sb_put_key_value(const sb_key_value *key_value,const char *key, const void *value);

/**
 * 根据key删除value，不会释放key和value,调用者需要手动释放内存以免出现内存泄漏.
 * @param key_value 要存放键值对的对象
 * @param key key
 * @return success 成功,fail 失败
 * * */
int sb_remove_key_value(const sb_key_value *key_value,const char *key);

/**
 * 根据key获取值
 * @param 存放键值对的对象
 * @param key key
 * @return key对应的值对象
 * */
void *sb_get_key_value(const sb_key_value *key_value,const char *key);

#endif //SB_SERVER_SB_KEY_VALUE_H