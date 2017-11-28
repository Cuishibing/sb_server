#include "sb_http.h"
#include "sb_server_context.h"
#include "sb_server.h"

#include <stdio.h>
#include <server/resource/sb_static_resource.h>
#include <data/sb_data_cache.h>
#include "sb_resource.h"
#include "sb_dynamic_resource.h"

void test_controller(void* args){
    error("hello world!");
}

typedef struct parent_class{
    int a;
};

typedef struct sub_class{
    struct parent_class *p;
    int a;
};


int main() {
    sb_server_context context;
    sb_init_server_context(&context);
    sb_put_context_property(&context,ROOT_PATH,"/home/cui/Desktop/server_root");

    sb_init_server(&context,8080);

    sb_register_http_model();

    sb_add_dynamic_resource(sb_new_dynamic_resource("/helloworld",test_controller));

    return sb_start_server();

//    sb_server server;
//    sb_server_context context;
//    sb_init_server_context(&context);
//    sb_init_server(&context,8080);
//    sb_put_context_property(&context,ROOT_PATH,"/home/cui/Desktop/server_root/");
//
//    sb_add_dynamic_resource(sb_new_dynamic_resource("helloworld",test_controller));
//
//    sb_resource** resource = sb_find_resource("helloworld");
//
//    if(resource != NULL){
//        if((*resource)->type == DYNAMIC){
//            sb_dynamic_resource *dr = (sb_dynamic_resource*)resource;
//            dr->method_ptr(NULL);
//        }else{
//            sb_static_resource* sta_resource = (sb_static_resource*)resource;
//            error(sta_resource->data.data_poll);
//        }
//    }

//    struct sub_class *s = (struct sub_class*)malloc(sizeof(struct sub_class));
//    s->p = (struct parent_class*)malloc(sizeof(struct parent_class));
//    s->a = 1;
//    s->p->a = 2;
//
//    struct parent_class *p = *((struct parent_class **) s);
//
//    int a;
}