//
// Created by cui on 17-9-23.
//

#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <server/resource/sb_dynamic_resource.h>
#include "sb_response_builder.h"
#include "sb_request_parser.h"
#include "sb_static_resource.h"
#include "sb_server.h"
#include "sb_client.h"
#include "sb_http.h"
/*
 * 解析请求中的参数(跟在URL后面的)
 * */
void parse_request_url_parameters(sb_request *request){
    const char *url = sb_get_request_parameter(request,REQUEST_URL);
    if(url != NULL){
        char *first_cursor = strstr(url,"?");
        if(first_cursor != NULL){
            char *request_target = (char*)calloc(first_cursor - url + 1,sizeof(char));
            strncpy(request_target,url,first_cursor - url + 1);
            sb_put_key_value(&request->request_data,REQUEST_TARGET,request_target);
        }else{
            char *request_target = (char*)calloc(strlen(url)+1,sizeof(char));
            strncpy(request_target,url,strlen(url));
            sb_put_key_value(&request->request_data,REQUEST_TARGET,request_target);
            return;
        }
        while(1){
            char *middle_cursor = NULL;
            char *last_cursor = NULL;
            if(first_cursor == NULL){
                //没有参数了
                break;
            }else{
                first_cursor = first_cursor + 1;
                middle_cursor = strstr(first_cursor,"=");
                if(middle_cursor == NULL){
                    break;
                }else{
                    char *param_name = (char*)calloc(middle_cursor - first_cursor + 1,sizeof(char));
                    strncpy(param_name,first_cursor,middle_cursor - first_cursor);
                    middle_cursor = middle_cursor + 1;
                    last_cursor = strstr(middle_cursor,"&");
                    char *param_value = NULL;
                    if(last_cursor == NULL){
                        //只有一个参数
                        param_value = (char*)calloc(strlen(middle_cursor)+1,sizeof(char));
                        strncpy(param_value,middle_cursor,strlen(middle_cursor));
                        break;
                    }else{
                        param_value = (char*)calloc(last_cursor - middle_cursor + 1,sizeof(char));
                        strncpy(param_value,middle_cursor,last_cursor - middle_cursor);
                        first_cursor = last_cursor;
                    }
                    sb_put_key_value(&request->request_data,param_name,param_value);
                }
            }
        }
    }
}

/*
 * 这是第一个过滤器链
 * */
void* sb_parse_http_start(sb_client *client, void *args){
    char *first_cursor = client->data_cache->data_poll;
    char *second_cursor = NULL;
    second_cursor = strstr(first_cursor," ");
    if(second_cursor == NULL){
        return NULL;
    }
    char *method = (char*)calloc((second_cursor - first_cursor + 1),sizeof(char));
    strncpy(method,first_cursor,second_cursor - first_cursor);

    sb_set_request_parameter(client->request,REQUEST_METHOD,method);
    first_cursor = second_cursor + 1;
    second_cursor = NULL;

    second_cursor = strstr(first_cursor," ");
    if(second_cursor == NULL){
        error("bad request\n");
        return NULL;
    }
    char *url = (char*)calloc((second_cursor - first_cursor + 1),sizeof(char));

    strncpy(url,first_cursor,second_cursor - first_cursor);

    sb_set_request_parameter(client->request,REQUEST_URL,url);
    first_cursor = second_cursor + 1;
    second_cursor = NULL;

    second_cursor = strstr(first_cursor,"\r\n");
    if(second_cursor == NULL){
        error("bad request\n");
        return NULL;
    }
    char *http_version = (char*)calloc((second_cursor - first_cursor + 1),sizeof(char));
    strncpy(http_version,first_cursor,second_cursor - first_cursor);
    sb_set_request_parameter(client->request,REQUEST_VERSION,http_version);

    parse_request_url_parameters(client->request);

    return second_cursor + 2;
}

void* sb_parse_http_head(sb_client *client,void *args){
    char *first_cursor = (char*)args;
    char *middle_cursor = NULL;
    char *last_cursor = NULL;
    sb_key_value *heads = (sb_key_value*)malloc(sizeof(sb_key_value));
    if(heads == NULL){
        error("内存不足!\n");
        return NULL;
    }
    if(!sb_init_key_value(heads)){
        return NULL;
    }
    int heads_num = 0;
    while(1){
        if(first_cursor == NULL){
            break;
        }
        last_cursor = strstr(first_cursor,"\r\n");
        if(last_cursor == NULL){
            break;
        }
        if(last_cursor == first_cursor){
            first_cursor += 2;
            break;
        }
        middle_cursor = NULL;
        middle_cursor = strstr(first_cursor,":");
        if(middle_cursor == NULL){
            break;
        }
        if(middle_cursor >= last_cursor){
            break;
        }
        char *head_name = (char*)calloc(middle_cursor - first_cursor + 1,sizeof(char));
        strncpy(head_name,first_cursor,middle_cursor - first_cursor);
        middle_cursor = middle_cursor + 1;
        char *head_body = (char*)calloc(last_cursor - middle_cursor + 1,sizeof(char));
        strncpy(head_body,middle_cursor,last_cursor - middle_cursor);
        sb_put_key_value(heads,head_name,head_body);
        heads_num++;
        first_cursor = last_cursor + 2;
    }
    sb_set_request_parameter(client->request,REQUEST_HEADS,heads);
    return first_cursor;
}

/*
 * 这是最后一个请求解析过滤器,要返回sb_client
 *
 * 请求数据的解析需要根据请求的类型和头部信息进行不同的解析
 * 如果是GET请求，不需要解析
 * 如果是POST请求，进一步判断ContentType的值
 * 如果是application/x-www-form-urlencoded
 * 解析body中的数据
 * 如果是multipart/form-data，要得到boundary进行解析参数
 * */
void* sb_parse_http_body(sb_client *client,void *args){
    //char *first_cursor = (char*)args;

    const char *request_method = sb_get_request_parameter(client->request,REQUEST_METHOD);
    if(strcmp(request_method,"GET") == 0){
        //这是一个GET请求,不用进行下一步的解析过程
        return client;
    }
    return client;
}

void sb_build_http_success_response(sb_client *client,void *args){
    sb_static_resource *resource = (sb_static_resource*)args;
    sb_print_http_start(client,200,"OK");
    sb_print_http_head(client,"Content-Type","text/html");//这里应该根据具体情况设定
    if(resource != NULL){
        sb_print_http_head_int(client,"Content-Length",resource->data.length);
        send(client->socket_fd,"\r\n",2,0);
        send(client->socket_fd,resource->data.data_poll,resource->data.length,0);
        send(client->socket_fd,"\r\n",2,0);
    }
}

void sb_print_http_start(sb_client *client,int code,const char *reason){
    assert(client!=NULL);
    const char *http_version = sb_get_request_parameter(client->request,REQUEST_VERSION);
    char str_code[11];
    memset(str_code,'\0',11);
    sprintf(str_code,"%d",code);
    send(client->socket_fd,http_version,strlen(http_version),0);
    send(client->socket_fd," ",1,0);
    send(client->socket_fd,str_code,strlen(str_code),0);
    send(client->socket_fd," ",1,0);
    send(client->socket_fd,reason,strlen(reason),0);
    send(client->socket_fd,"\r\n",4,0);
}

void sb_print_http_head(sb_client *client,const char *head_name,const char *head_value){
    send(client->socket_fd,head_name,strlen(head_name),0);
    send(client->socket_fd,": ",2,0);
    send(client->socket_fd,head_value,strlen(head_value),0);
    send(client->socket_fd,"\r\n",2,0);
}

void sb_print_http_head_int(sb_client *client,const char *head_name,const int head_value){
    char str_value[11];
    memset(str_value,'\0',11);
    sprintf(str_value,"%d",head_value);
    sb_print_http_head(client,head_name,str_value);
}

void sb_print_http_body(sb_client *client,sb_static_resource *resource){

}

static int parse_request_composite(void* args){
    sb_client *client = (sb_client*)args;
    void* return_value = sb_parse_http_start(client,NULL);
    return_value = sb_parse_http_head(client,return_value);
    return_value = sb_parse_http_body(client,return_value);
    return return_value == NULL ? fail:success;
}

static int build_response_composite(void *args,sb_resource **res){
    sb_client *client = (sb_client*)args;
    if((*res)->type == STATIC){
        sb_static_resource* static_res = (sb_static_resource*)res;
        sb_build_http_success_response(client,static_res);
    }else{
        sb_dynamic_resource* dynamic_resource = (sb_dynamic_resource*)res;
        dynamic_resource->method_ptr(client);
        sb_print_http_start(client,200,"OK");
    }

    return success;
}

void sb_register_http_model(){
    sb_set_request_parser(parse_request_composite);
    sb_set_response_builder(build_response_composite);
}