#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sb_data_cache.h>
#include <pthread.h>
#include <sb_read_worker.h>
#include <sb_worker.h>
#include <sb_key_value.h>
#include <sb_request.h>
#include <sb_client.h>
#include <sb_request_process_filter.h>
#include <sb_resource.h>
#include <sb_http.h>
#include "sb_server_context.h"
#include "sb_server.h"
#include "sb_read_worker.h"
#include "sb_key_value.h"

void* first_filter(sb_client *client, sb_request *request, void *args){
    fprintf(stderr,"%d\t%s",client->socket_fd,client->request_data->data_poll);
    return NULL;
}

void set_filters(){
    sb_arraylist *filters = (sb_arraylist*)malloc(sizeof(sb_arraylist));
    sb_set_request_process_filters(filters);
    sb_add_method_req_process_filters(first_filter);
}

int main() {
    sb_server_context context;
    sb_init_server_context(&context);
    sb_init_server(&context,8080);

    sb_init_http_filters();

    return sb_start_server();

//    sb_resource *properties = sb_get_resource("./","Makefile");
//    sb_resource *second_properties = sb_get_resource("./","Makefile");
//    fprintf(stderr,"%s",second_properties->data.data_poll);

//    sb_data_cache cache;
//    sb_init_data_cache(&cache);
//    sb_put_data_cache(&cache,"cuishibingshigehaoren");
//    sb_put_data_cache(&cache,"hahahaha");
//    sb_trim_data_cache(&cache);
//    sb_put_data_cache(&cache," expand");

//    char *test_request = "GET /cuishibing HTTP/1.1\r\n";
//    sb_client client;
//    sb_data_cache cache;
//    client.request_data = &cache;
//    sb_init_data_cache(client.request_data);
//    client.request_data->data_poll = test_request;
//    sb_request request;
//    sb_init_request(&request);
//    sb_parse_http_start(&client,&request,NULL);

    return 0;
}