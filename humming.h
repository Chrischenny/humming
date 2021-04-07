/**
 * @file humming.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#define _GNU_SOURCE
#include <features.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "mongoose.h"

#define HUMM_ENABLE_SSL      1

struct humm_str
{
    char *str;
    size_t len;
};


struct request
{
    struct humm_str method, uri, query, proto;             // Request/response line
    struct humm_str headers[MG_MAX_HTTP_HEADERS];          // Headers
    struct humm_str body;                                  // Body
    struct humm_str message;  // Request line + headers + body
};


struct response
{
    int state_code;
    char **headers;
    char *body;
};


/**
 * @brief 
 * 
 */
typedef void (*next)(struct context *);



/**
 * @brief 
 * 
 */
typedef void (*humm_call_back)(struct context *, next);


/**
 * @brief 
 * 
 */
struct humm_call_back_list
{
    humm_call_back fn;
    struct humm_call_back_list *next;
};


/**
 * @brief humming
 * 
 */
struct humm_app
{
    struct mg_connection *conn;
    struct humm_call_back_list *list_head;
    uint8_t cfg_flags;
    char *listen_addr;
    char *ssl_key;
    char *ssl_pem;
};


/**
 * @brief 
 * 
 */
struct context
{
    struct request *req;
    struct response *res;
    struct humm_app *app;
    struct humm_call_back_list *current_fn;
};


/**
 * @brief Create a humm app object
 * 
 * @return struct humm_app *
 */
struct humm_app* create_humm_app();


/**
 * @brief 
 * 
 */
void destroy_humm_app();


/**
 * @brief humming app
 * 
 * @return int8_t 
 */
int8_t init_humm_app(struct humm_app *, struct humm_opts *);


/**
 * @brief 
 * 
 * @param uri 
 */
void humm_app_use(struct humm_app *, char *uri, humm_call_back);


/**
 * @brief port
 * 
 * @param port 
 */
void humm_run(struct humm_app *, char *port);
