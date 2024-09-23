#include <binder/v8/v8debug/V8WSSv.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <functional>

namespace laya {

    struct vhdinfo {
        struct lws_context *context;
        struct lws_vhost *vhost;
        int *interrupted;
        int *options;
    };


    static int send_http_response(struct lws *wsi, const char* type, const char *str) {
        // 确保从LWS_PRE偏移开始写入
        unsigned char *buffer = (unsigned char *)malloc(LWS_PRE + 1024);
        unsigned char *p = &buffer[LWS_PRE], *end = &buffer[LWS_PRE + 1024];

        // 准备你的文本字符串内容
        int response_len = strlen(str);
        
        // 添加HTTP头部
        if (lws_add_http_header_status(wsi, HTTP_STATUS_OK, &p, end)) {
            free(buffer);
            return 1;
        }

        if (lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_CONTENT_TYPE,
            (const unsigned char *)type, strlen(type), &p, end)) {
            free(buffer);
            return 1;
        }

        if (lws_add_http_header_content_length(wsi, response_len, &p, end)) {
            free(buffer);
            return 1;
        }

        // 完成HTTP头部写入
        if (lws_finalize_http_header(wsi, &p, end)) {
            free(buffer);
            return 1;
        }

        // 写入HTTP头部
        if (lws_write(wsi, &buffer[LWS_PRE], p - (&buffer[LWS_PRE]), LWS_WRITE_HTTP_HEADERS) < 0) {
            free(buffer);
            return 1;
        }

        // 写入HTTP响应体
        if (lws_write(wsi, (unsigned char *)str, response_len, LWS_WRITE_HTTP) < 0) {
            free(buffer);
            return 1;
        }

        free(buffer);
        return 0;
    }

    /*
    参照 https://github.com/warmcat/libwebsockets/tree/master/minimal-examples/ws-server/minimal-ws-server-echo
    需要切换版本。
    */

    //per_session_data__v8dbg* pCurPss = nullptr;
    per_session_data__v8dbg* frontUser = nullptr;
    DebuggerAgent* gpDbgAgent = nullptr;
    int gSessionID=0;
    static int callback_def(
        struct lws *wsi, enum lws_callback_reasons reason,
        void *user, void *in, size_t len) {

        int n;
        int sendMsgLen = 0;
        per_session_data__v8dbg *pss = (per_session_data__v8dbg *)user;

        switch (reason) {
        case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
            char buffer[256];
            lws_hdr_copy(wsi, buffer, sizeof(buffer), WSI_TOKEN_GET_URI);
            lwsl_notice("Header URI: %s\n", buffer);
            if(strcmp(buffer,"/a9f0cbe3-46ba-4b94-b937-fa254f5974e2")==0){
                printf("前端请求调试native，user:%x\n",user);
                frontUser = pss;
            }
            return 0;//都同意，有的是http 1.1
            break;
        case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
            printf("filter connection %x\n",user);
            //返回非0拒绝连接
            break;
        case LWS_CALLBACK_PROTOCOL_INIT:
            break;
        case LWS_CALLBACK_ESTABLISHED:  //有人连进来了
            printf("connection established, %x, %d\n",pss, pss->id);
            if(pss && pss==frontUser){
                pss->pRecvBuff = nullptr;
                pss->index = 0;
                pss->id = gSessionID++;
                pss->nRecvLen = 0;
                pss->pDbgAgent = gpDbgAgent;
                new (&pss->pTaskLock) std::recursive_mutex();
                new (&pss->pSendTask) std::deque<std::string>();
                //new (&pss->pSendTask) std::vector<std::string>();
                gpDbgAgent->onAcceptNewFrontend(pss);

            }else{
                printf("另一个连接建立 %x\n", pss);
                int a = 0;
            }
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: //可以发送了
            //缺省的是继续发送。例如上一次可能只发送了一部分
            n = LWS_WRITE_CONTINUATION;
            if (!pss->continuation) {
                //如果是新的任务，就设置文本或者二进制。
                if (pss->binary)
                    n = LWS_WRITE_BINARY;
                else
                    n = LWS_WRITE_TEXT;
                pss->continuation = 1;
            }
            if (!pss->final)
                n |= LWS_WRITE_NO_FIN;

            //pss->tx += pss->len;
            pss->pTaskLock.lock();
            //一次处理一个
            if (pss->pSendTask.size() > 0) {
                std::string& t1 = pss->pSendTask.front();
                //printf("send:%s\n", t1.substr(0,200).c_str());
                sendMsgLen = t1.length();
                if (pss->pSendBuff) {
                    delete[] pss->pSendBuff;
                }
                pss->pSendBuff = new unsigned char[sendMsgLen + LWS_PRE];
                memcpy(pss->pSendBuff + LWS_PRE, t1.c_str(), sendMsgLen);
                //删掉第一个
                pss->pSendTask.pop_front();
            }
            pss->pTaskLock.unlock();
            if (sendMsgLen > 0) {
                n = lws_write(wsi, &pss->pSendBuff[LWS_PRE], sendMsgLen, (lws_write_protocol)n);
                if (n < 0) {
                    lwsl_err("ERROR %d writing to socket, hanging up\n", n);
                    return 1;
                }
                if (n < (int)sendMsgLen) {
                    lwsl_err("Partial write\n");
                    //这里不知道怎么处理，按理说部分发送的情况不用管。下次LWS_CALLBACK_SERVER_WRITEABLE的时候应该已经全部发送了。
                    //所以pss->continuation应该还是=0
                    //这里直接返回，这样pss->continuation就是1了，是故意的么
                    return -1;
                }
                //printf("write  %x\n",user);
            }
            if (pss->final)
                pss->continuation = 0;
            /* 这会导致websocket报错，写返回-1
            //一次全部处理
            pss->pTaskLock.lock();
            if (pss->pSendTask.size() > 0) {
                for (std::string& t1 : pss->pSendTask) {
                    printf("send:%s\n", t1.c_str());
                    sendMsgLen = t1.length();
                    if (pss->pSendBuff) {
                        delete[] pss->pSendBuff;//TODO 优化
                    }
                    pss->pSendBuff = new unsigned char[sendMsgLen + LWS_PRE];
                    memcpy(pss->pSendBuff + LWS_PRE, t1.c_str(), sendMsgLen);
                    n = lws_write(wsi, &pss->pSendBuff[LWS_PRE], sendMsgLen, (lws_write_protocol)n);
                    if (n < 0) {
                        lwsl_err("ERROR %d writing to socket, hanging up\n", n);
                        return 1;
                    }
                    if (n < (int)sendMsgLen) {
                        lwsl_err("Partial write\n");
                        //这里不知道怎么处理，按理说部分发送的情况不用管。下次LWS_CALLBACK_SERVER_WRITEABLE的时候应该已经全部发送了。
                        //所以pss->continuation应该还是=0
                        //这里直接返回，这样pss->continuation就是1了，是故意的么
                        return -1;
                    }
                }
            }
            pss->pTaskLock.unlock();
            */
            //lws_rx_flow_control(wsi, 1);    //允许在此连接上接收数据。
            lws_callback_on_writable(wsi);//一旦有机会，触发写回调
            break;
        case LWS_CALLBACK_RECEIVE: { // 可以接收了。
                                     // Create a buffer to hold our response
                                     // it has to have some pre and post padding.
                                     // You don't need to care what comes there, libwebsockets
                                     // will do everything for you. For more info see
                                     // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597

            //amsg.first = lws_is_first_fragment(wsi);
            pss->final = lws_is_final_fragment(wsi);    //是否是最后一个信息。
            pss->binary = lws_frame_is_binary(wsi);     //是否是二进制的。
            const size_t remaining = lws_remaining_packet_payload(wsi);
            //lwsl_info("+++ test-echo: RX len %ld final %ld, pss->len=%ld\n",(long)len, (long)pss->final, (long)pss->len);

            if (in && len > 0) {
                if (remaining > 0) {
                    // 帧不完整，需要缓存接收到的数据
                    if (pss->nRecvLen + LWS_PRE + len > sizeof(pss->RecvBuf)) {
                        // 缓存空间不足，处理错误
                        lwsl_err("Receive buffer overflow\n");
                        return -1;
                    }
                    memcpy(&pss->RecvBuf[LWS_PRE] + pss->nRecvLen, in, len);
                    pss->nRecvLen += len;
                    pss->rx += len;
                }
                else {
                    if (len+LWS_PRE > sizeof(pss->RecvBuf)) {
                        lwsl_err("Receive buffer overflow\n");
                        return -1;
                    }
                    if (pss->pDbgAgent) {
                        memcpy(&pss->RecvBuf[LWS_PRE] + pss->nRecvLen, in, len);
                        pss->nRecvLen += (unsigned int)len;
                        pss->RecvBuf[LWS_PRE + pss->nRecvLen] = 0;
                        pss->rx += len;
                        pss->pDbgAgent->onDbgMsg((char*)(pss->RecvBuf + LWS_PRE), pss->nRecvLen);
                        pss->nRecvLen = 0;  //完整了，清零
                        //printf("read  %x\n",user);
                    }
                }
            }

            //lws_rx_flow_control(wsi, 0);//禁止在此连接上接收数据。
            lws_callback_on_writable(wsi);//一旦有机会，触发写回调
            break;
        }

        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_debug("closed\n");
            printf("closed %x\n",pss);
            if(pss && pss==frontUser){
                if ( frontUser->pDbgAgent) {
                    frontUser->pDbgAgent->onFrontEndClose();
                }
                frontUser=nullptr;
            }
            //state = 0;
            break;

        case LWS_CALLBACK_GET_THREAD_ID:
            //return (unsigned long)GetCurrentThreadId();
            break;
        case LWS_CALLBACK_HTTP:
        {
            const char* requestPath = (const char*)in;
            if (strcmp(requestPath, "/json/list") == 0) {
                const char* jsonResponse = R"json(
[
    {
        "description": "laya native debugger",
        "devtoolsFrontendUrl": "/devtools/bundled/js_app.html?ws=localhost:5959/a9f0cbe3-46ba-4b94-b937-fa254f5974e2",
        "id": "1",
        "title": "LayaNative ",
        "type": "node",
        "webSocketDebuggerUrl": "ws://localhost:5959/a9f0cbe3-46ba-4b94-b937-fa254f5974e2"
    }
]
)json";
                //lws_serve_http_file(wsi, jsonResponse, "application/json", NULL, 0);
                return send_http_response(wsi,"application/json",jsonResponse);

            }else if (strcmp(requestPath, "/json/version") == 0) {
                const char* jsonResponse = R"json(
{
    "Browser": "LayaNative V8/3.0",
    "Protocol-Version": "1.3",
    "User-Agent": "LayaNative V8",
    "V8-Version": "7.8.279.23",
    "WebKit-Version": "537.36 (@2336ba86d0d067e7a5df1b596c80e4c1f235a5a3)"
}
                )json";
                //lws_serve_http_file(wsi, jsonResponse, "application/json", NULL, 0);
                return send_http_response(wsi,"application/json",jsonResponse);
            }
        }
            break;
        default:
            break;
        }

        return 0;
    }

#define RING_DEPTH 4096

    static struct lws_protocols protocols[] = {
        /* first protocol must always be HTTP handler */
        {
            "http-only",   // name
            callback_def, // callback
            sizeof(per_session_data__v8dbg),              // per_session_data_size
            MAX_V8DBG_PAYLOAD
        },
        {
            NULL, NULL, 0,0   /* End of list */
        }
    };

    std::thread* wssvth = nullptr;
    bool interrupted = false;

    void wsserver_run(lws_context* context) {
        int n = 0;
        while (n >= 0 && !interrupted) {
            //nWSSVSleep 是 timeout_ms: 等待超时时间，即没有找到需要处理的连接需要等待的时间，为0则立即返回；
            //这个会一通过回调处理消息。
            int nSleep = 10;
            if (frontUser) {
                frontUser->pTaskLock.lock();
                if (frontUser->pSendTask.size() > 0) {
                    nSleep = 0;
                }
                frontUser->pTaskLock.unlock();
            }
            n = lws_service(context, nSleep);
        }
        lws_context_destroy(context);
    }

    static void lwsl_custom_logger(int level, const char* line) {
        // 将libwebsocket的日志输出到你的日志系统或标准输出等
        printf("%s\n", line);
    }
    // 因为目前只有windows的websocket更新了
#if defined(OS_WINDOWS) || defined(OS_OHOS)   
    void startWSSV(int port, DebuggerAgent* pDbgAgent) {
        gpDbgAgent = pDbgAgent;
        interrupted = false;
        struct lws_context* context;
        // we're not using ssl
        const char* cert_path = NULL;
        const char* key_path = NULL;
        // no special options
        int opts = 0;


        lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_USER/* | LLL_INFO */ /* | LLL_DEBUG */, NULL);

        lws_context_creation_info cinfo;
        memset(&cinfo, 0, sizeof(cinfo));
        cinfo.port = port;
        //cinfo.mounts = &mount;
        cinfo.protocols = protocols;
        //cinfo.extensions = lws_get_internal_extensions();
        //if (!use_ssl) {
        cinfo.ssl_cert_filepath = NULL;
        cinfo.ssl_private_key_filepath = NULL;
        //} else {
        //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
        //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
        //}
        cinfo.gid = -1;
        cinfo.uid = -1;
        // 设置TCP Keep-Alive参数
        //cinfo.ka_time = 60;  // 如果60秒内没有数据交换，就发送一个Keep-Alive探针
        //cinfo.ka_probes = 10; // 发送Keep-Alive探针的最大次数
        //cinfo.ka_interval = 10;  // 每个Keep-Alive探针之间间隔10秒        
        cinfo.timeout_secs = 3000;

        cinfo.options |= LWS_SERVER_OPTION_DISABLE_IPV6;


        // create libwebsocket context representing this server
        context = lws_create_context(&cinfo);

        if (context == NULL) {
            fprintf(stderr, "libwebsocket init failed\n");
            return;
        }

        printf("starting server...\n");
        wssvth = new std::thread(std::bind(wsserver_run, context));;
    }
#else
    void startWSSV(int port, DebuggerAgent* pDbgAgent) {
        gpDbgAgent = pDbgAgent;
        interrupted = false;
        struct lws_context *context;
        // we're not using ssl
        const char *cert_path = NULL;
        const char *key_path = NULL;
        // no special options
        int opts = 0;

        static const struct lws_http_mount mount = {
            /* .mount_next */               NULL,           /* linked-list "next" */
            /* .mountpoint */               "/",            /* mountpoint URL */
            /* .origin */                   ".",            /* serve from dir */
            /* .def */                      "index.html",   /* default filename */
            /* .protocol */                 NULL,
            /* .cgienv */                   NULL,
            /* .extra_mimetypes */          NULL,
            /* .interpret */                NULL,
            /* .cgi_timeout */              0,
            /* .cache_max_age */            0,
            /* .auth_mask */                0,
            /* .cache_reusable */           0,
            /* .cache_revalidate */         0,
            /* .cache_intermediaries */     0,
            /* .origin_protocol */          LWSMPRO_FILE,   /* files in a dir */
            /* .mountpoint_len */           1,              /* char count */
            /* .basic_auth_login_file */    NULL,
        };

        lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_USER/* | LLL_INFO */ /* | LLL_DEBUG */, NULL);

        lws_context_creation_info cinfo;
        memset(&cinfo, 0, sizeof(cinfo));
        cinfo.port = port;
        //cinfo.mounts = &mount;
        cinfo.protocols = protocols;
        cinfo.extensions = lws_get_internal_extensions();
        //if (!use_ssl) {
        cinfo.ssl_cert_filepath = NULL;
        cinfo.ssl_private_key_filepath = NULL;
        //} else {
        //  info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
        //  info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
        //}
        cinfo.gid = -1;
        cinfo.uid = -1;
        cinfo.options |= LWS_SERVER_OPTION_DISABLE_IPV6;


        // create libwebsocket context representing this server
        context = lws_create_context( &cinfo);

        if (context == NULL) {
            fprintf(stderr, "libwebsocket init failed\n");
            return ;
        }

        printf("starting server...\n");
        wssvth  = new std::thread(std::bind(wsserver_run, context));;
    }
#endif
    void stopWSSV() {
        interrupted = true;
        if (wssvth != NULL)
        {
            wssvth->join();
        }
    }
}

