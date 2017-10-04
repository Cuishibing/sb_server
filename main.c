
#include "sb_http.h"
#include "sb_server_context.h"
#include "sb_server.h"

int main() {
    sb_server_context context;
    sb_init_server_context(&context);
    sb_put_context_property(&context,ROOT_PATH,"/home/cui/Desktop/server_root/");

    sb_init_server(&context,8080);

    sb_init_http_filters();

    return sb_start_server();
}