//
// Created by cui on 17-11-27.
//

#ifndef SB_SERVER_SB_REQUEST_PARSER_H
#define SB_SERVER_SB_REQUEST_PARSER_H

static const char *REQUEST_TARGET = "request_target";

typedef struct request_parser_method{
    int(*method_ptr)(void*);
}request_parser;

/**
 * 设置请求解析的接口
 * @param method_ptr 请求解析所使用的函数指针
 * @return 是否设置成功
 * */
int sb_set_request_parser(int(*method_ptr)(void *));

request_parser* sb_get_request_parser();

request_parser* sb_new_request_parser(int(*method_ptr)(void *));

#endif //SB_SERVER_SB_REQUEST_PARSER_H
