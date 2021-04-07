include“humming.h"

static void next_cb(struct context *ctx)
{

}

static void humm_mg_poll_cb(struct mg_connection *conn, int ev, int *ev_data, void *fn_data)
{
    if (ev == MG_EV_POLL)
    {
        struct context *ctx = (struct context *)fn_data;
        ctx->current_fn->fn(ctx, next_cb);
    }
}



static void humm_mg_http_cb(struct mg_connection *conn, int ev, int *ev_data, void *fn_data)
{
    if (ev == MG_EV_HTTP_MSG)
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        struct context *ctx = (struct context *)fn_data;
        char *req = malloc(hm->message.len);
        memcpy(req, hm->message.ptr, hm->message.len);
        ctx->req = req;
    }
}


static void humm_mg_accept_cb(struct mg_connection *conn, int ev, int *ev_data, void *fn_data)
{
    if (ev == MG_EV_ACCEPT)
    {
        struct humm_app *app = (struct humm_app *)fn_data;
        conn->fn_data = malloc(sizeof(struct context));
        if (app->cfg_flags & HUMM_ENABLE_SSL &&  mg_url_is_ssl(app->listen_addr))
        {
            struct mg_tls_opts opts = {
                .cert = app->ssl_key,     
                .certkey = app->ssl_pem, 
            };
            mg_tls_init(conn, &opts);
        }
        conn->fn = humm_mg_http_cb;
    }
}
