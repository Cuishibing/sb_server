//
// Created by cui on 17-9-17.
//

#ifndef SB_SERVER_SB_DATA_CACHE_H
#define SB_SERVER_SB_DATA_CACHE_H

#include <stddef.h>
/**
 * 一个简单的数据缓存
 * */
typedef struct {
    char *data_poll;
    size_t length;
    size_t size;
}sb_data_cache;

/**
 * 缓存默认大小
 * */
static size_t DEFAULT_SIZE = 512;

/**
 * 初始化
 * @param cache 要初始化的缓存对象
 * @return success 成功,fail 失败
 * */
int sb_init_data_cache(sb_data_cache *cache);

/**
 * 动态增加缓存的大小
 * @param cache 要扩展大小的缓存对象
 * @return success 成功,fail 失败
 * */
static int expand_cache_size(sb_data_cache *cache);

/**
 * 向缓存中写入数据,这里没有接收数据的大小,所以大小是在内部计算出来的,数据以'\0'结尾
 * @param cache 缓存对象
 * @param buffer 数据
 * @return success 成功,fail 失败
 * */
int sb_put_data_cache(sb_data_cache *cache,const char *buffer);

/**
 * 清除缓存对象中的数据,只是清除数据
 * @param cache 缓存对象
 * @return success 成功,fail 失败
 * */
int sb_clear_data_cache(sb_data_cache *cache);

/**
 * 清除掉多于的空间(动态增加缓存的大小会有一部分冗于)
 * @param cache 要trim的缓存
 * @return success 成功,fail 失败
 * */
int sb_trim_data_cache(sb_data_cache *cache);

/**
 * 释放一个缓存对象内部所有的数据所占用的空间,并没有释放缓存对象本身.不释放缓存对象本身是考虑到有可能这个
 * 缓存对象是在栈上开辟的.
 * */
int sb_free_data_cache(sb_data_cache *data_cache);

#endif //SB_SERVER_SB_DATA_CACHE_H