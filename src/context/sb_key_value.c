//
// Created by cui on 17-9-20.
//

#include <sb_key_value.h>
#include <memory.h>
#include <stdlib.h>
#include <sb_server.h>

int sb_init_key_value(sb_key_value *key_value){
    if(key_value == NULL){
        return fail;
    }
    return (sb_init_arraylist(&key_value->key_set,10)) && (sb_init_map(&key_value->pool));
}

int sb_put_key_value(const sb_key_value *key_value,const char *key, const void *value){
    sb_element value_element,key_element;
    value_element.value = value;
    key_element.value = key;
    return sb_insert_map(&key_value->pool,key,strlen(key),value_element) &&
           sb_insert_arraylist(&key_value->key_set,key_element,key_value->key_set.length);
}

int sb_remove_key_value(const sb_key_value *key_value,const char *key){
    if(key_value == NULL){
        return fail;
    }
    sb_element temp;
    sb_arraylist *key_set = &key_value->key_set;
    for(int i = 0;i<key_set->length;i++){
        if(sb_get_arraylist(key_set,i,&temp) == success){
            const char *temp_key = temp.value;
            if(strcmp(temp_key,key) == 0){
                sb_remove_position_arraylist(key_set,i,&temp);
                //free(temp.value);
                sb_remove_map(&key_value->pool,key,strlen(key),&temp);
                //free(temp.value);
                return success;
            }
        }else return fail;
    }
    return success;
}

void *sb_get_key_value(const sb_key_value *key_value,const char *key){
    if(key_value != NULL){
        sb_element element;
        if(sb_get_map(&key_value->pool,key,strlen(key),&element)){
            return element.value;
        }
    }
    return NULL;
}