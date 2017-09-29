#include <stdio.h>
#include <stdlib.h>
#include <sb_data_cache.h>
#include <sb_read_worker.h>
#include <sb_request_process_filter.h>
#include <sb_http.h>
#include "sb_server_context.h"
#include "sb_server.h"

void* first_filter(sb_client *client, sb_request *request, void *args){
    fprintf(stderr,"%d\t%s",client->socket_fd,client->data_cache->data_poll);
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
    sb_put_context_property(&context,ROOT_PATH,"/home/cui/Desktop/server_root/");

    sb_init_server(&context,8080);

    sb_init_http_filters();

    return sb_start_server();

//    sb_resource *properties = sb_get_resource("./","Makefile");
//    sb_resource *second_properties = sb_get_resource("./","Makefile");
//    fprintf(stderr,"%s",second_properties->data.data_poll);
//
//    sb_data_cache cache;
//    sb_init_data_cache(&cache);
//    sb_put_data_cache(&cache,"cuishibingshigehaoren");
//    sb_put_data_cache(&cache,"hahahaha");
//    sb_trim_data_cache(&cache);
//    sb_put_data_cache(&cache," expand");

//    char *test_request = "GET /?name=cui&age=32 HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:55.0) Gecko/20100101 Firefox/55.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 22\r\nCookie: Idea-e9d235c6=fce01ac6-2f81-432f-82d3-4b857ea439e6\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\n\r\nname=cuishiibng&age=23";
//    sb_client client;
//    sb_data_cache cache;
//    client.data_cache = &cache;
//    sb_init_data_cache(client.data_cache);
//    client.data_cache->data_poll = test_request;
//    sb_request request;
//    sb_init_request(&request);
//    //sb_parse_http_start(&client,&request,NULL);
//    sb_parse_http_body(&client,&request,sb_parse_http_head(&client,&request,sb_parse_http_start(&client,&request,NULL)));
//
//    sb_key_value *heads = sb_get_request_parameter(&request,REQUEST_HEADS);
//    if(heads != NULL){
//        sb_element element;
//        for (int i = 0; i < heads->key_set.length; ++i) {
//            sb_get_arraylist(&heads->key_set,i,&element);
//            fprintf(stderr,"%s:%s\n",element.value,sb_get_key_value(heads,element.value));
//        }
//    }

    return 0;
}