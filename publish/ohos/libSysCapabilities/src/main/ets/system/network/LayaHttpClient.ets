import http from '@ohos.net.http';
import { BusinessError } from '@ohos.base';
import { Logger } from '../../utils/Logger'
import laya from "liblaya.so";
import { ContextType } from '../../common/Constants';
import { GlobalContext, GlobalContextConstants } from '../../common/GlobalContext';

let log: Logger = new Logger(0x0001, "LayaHttpClient");
const napiHttp: laya.CPPFunctions = laya.getContext(ContextType.HTTP_NAPI);
export class LayaHttpClient {
  httpRequest = http.createHttp();
  url: string = "";
  localFilePath: string = "";
  method: http.RequestMethod = http.RequestMethod.GET;
  header: object = new Object();
  postData: ArrayBuffer = new ArrayBuffer(0);
  ptr: number = 0;
  connectTimeout: number = 60000;
  readTimeout: number = 60000;
  public constructor() {
  }
  public static create(ptr: number, url: string, localFilePath: string, tag: number) {
    let httpClient = new LayaHttpClient();
    httpClient.ptr = ptr;
    httpClient.url = url;
    httpClient.localFilePath = localFilePath;
    //GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_ARRAY).push(httpClient);
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).set(tag, httpClient);
  }
  public static destroy(tag: number) {
    GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).delete(tag);
  }
  public static cancel(tag: number) {
    let connection: LayaHttpClient = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).get(tag);
    connection.httpRequest.destroy();
  }
  public static postData(tag: number, postData: ArrayBuffer) {
    let connection: LayaHttpClient = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).get(tag);
    connection.method = http.RequestMethod.POST;
  }
  public static addHeader(tag: number, key: string, value: string) {
    let connection: LayaHttpClient = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).get(tag);
    if (key.toUpperCase() === "Content-Type".toUpperCase())
    {
        //contentType = value;
    }
    connection.header[key] = value;
  }
  public static setMethod(tag: number, method: string) {
  }
  public static doRequest(tag: number) {
    let connection: LayaHttpClient = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).get(tag);
    connection.httpRequest.request(
      connection.url,
      {
        method: connection.method,
        header: connection.header,
        extraData: connection.postData != null ? connection.postData : null,
        expectDataType: http.HttpDataType.ARRAY_BUFFER,
        usingCache: false,
        priority: 1,
        connectTimeout: connection.connectTimeout,
        readTimeout: connection.readTimeout,
      }, (err: BusinessError, data: http.HttpResponse) => {
      if (!err) {
        if (data.result instanceof ArrayBuffer) {
          napiHttp.HttpClientOHOS_onResponse(connection.ptr, data.result, data.responseCode, JSON.stringify(data.header));
        }
        else {
          log.error("LayaHttpClient.HttpResponse not ArrayBuffer")
        }
        connection.httpRequest.destroy();
      }
      else {
        napiHttp.HttpClientOHOS_onFailure(connection.ptr, err.code);
        connection.httpRequest.destroy();
      }
    });
  }
  public static setReadTimeout(tag: number, miliseconds: number) {
    let connection: LayaHttpClient = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).get(tag);
    connection.readTimeout = miliseconds;
  }
  public static setConnectTimeout(tag: number, miliseconds: number) {
    let connection: LayaHttpClient = GlobalContext.loadGlobalThis(GlobalContextConstants.LAYA_HTTP_CLIENT_INDEX_MAP).get(tag);
    connection.connectTimeout = miliseconds;
  }
}