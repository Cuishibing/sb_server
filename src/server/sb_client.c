//
// Created by cui on 17-9-17.
//

#include <sb_client.h>
#include <stdio.h>
#include <stdlib.h>
#include "sb_client.h"

int sb_touch_client(sb_client *client, clienter_status status){
    if(client == NULL){
        fprintf(stderr,"client is null!");
        return 0;
    }
    time(client->time_stamp);
    client->status = status;
    return 1;
}

int sb_init_client(sb_client *client, int socket){
    if(client == NULL){
        fprintf(stderr,"client is null!");
        return 0;
    }
    if(!sb_touch_client(client, INITED)){
        return 0;
    }
    client->socket_fd = socket;
    client->request_data = (sb_data_cache*)malloc(sizeof(sb_data_cache));
    if(client->request_data == NULL){
        fprintf(stderr,"内存不足!");
        return 0;
    }
    return sb_init_data_cache(client->request_data);
}