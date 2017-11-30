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

int main() {
    sb_server_context context;
    sb_init_server_context(&context);
    sb_put_context_property(&context,ROOT_PATH,"/home/cui/Desktop/server_root");
    sb_init_server(&context,8080);
    sb_register_http_model();
    sb_add_dynamic_resource(sb_new_dynamic_resource("/helloWorld",test_controller));

    return sb_start_server();

}