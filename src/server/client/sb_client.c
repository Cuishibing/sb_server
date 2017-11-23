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
    clients[client->socket_fd] = client;
    if(sb_touch_client(client, INITED) == fail){
        return fail;
    }
    client->socket_fd = socket;
    client->data_cache = (sb_data_cache*)malloc(sizeof(sb_data_cache));
    client->request = (sb_request*)malloc(sizeof(sb_request));
    if(client->data_cache == NULL || client->request == NULL){
        error("内存不足!");
        return fail;
    }
    return sb_init_data_cache(client->data_cache) && sb_init_request(client->request);
}

sb_client* sb_get_client(int socket){
    //TODO:未实现
    return clients[socket];
}

void sb_remove_client(sb_client *client){
    /* TODO:
     * release client's resources.(request object,data cache e.g)
     * */
}