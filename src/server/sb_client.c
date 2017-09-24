//
// Created by cui on 17-9-17.
//

#include <sb_client.h>
#include <stdio.h>
#include <stdlib.h>
#include <sb_server.h>
#include "sb_client.h"

int sb_touch_client(sb_client *client, clienter_status status){
    if(client == NULL){
        error("client is null!\n");
        return fail;
    }
    time(client->time_stamp);
    client->status = status;
    return success;
}

int sb_init_client(sb_client *client, int socket){
    if(client == NULL){
        error("client is null!");
        return fail;
    }
    if(sb_touch_client(client, INITED) == fail){
        return fail;
    }
    client->socket_fd = socket;
    client->request_data = (sb_data_cache*)malloc(sizeof(sb_data_cache));
    if(client->request_data == NULL){
        error("内存不足!");
        return fail;
    }
    return sb_init_data_cache(client->request_data);
}