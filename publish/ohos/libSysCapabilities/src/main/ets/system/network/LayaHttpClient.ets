import http from '@ohos.net.http';
import { BusinessError } from '@ohos.base';
import { Logger } from '../../utils/Logger'
import laya from "liblaya.so";
import { ContextType } from '../../common/Constants';

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
  public constructor() {
  }
  public static create(ptr: number, url: string, localFilePath: string): LayaHttpClient {
    //log.debug("call LayaHttpClient.create: %{public}s %{public}s", url, localFilePath)
    let httpClient = new LayaHttpClient();
    httpClient.ptr = ptr;
    httpClient.url = url;
    httpClient.localFilePath = localFilePath;
    return httpClient;
  }
  public static cancel(connection: LayaHttpClient) {
    connection.httpRequest.destroy();
  }
  public static postData(connection: LayaHttpClient, postData: ArrayBuffer) {
    connection.method = http.RequestMethod.POST;
  }
  public static addHeader(connection: LayaHttpClient, key: string, value: string) {
    if (key.toUpperCase() === "Content-Type".toUpperCase())
    {
        //contentType = value;
    }
    connection.header[key] = value;
  }
  public static setMethod(connection: LayaHttpClient, method: string) {
  }
  public static doRequest(connection: LayaHttpClient) {
    log.debug("call LayaHttpClient.doRequest ")
    connection.httpRequest.request(
       connection.url,
      {
        method: connection.method,
        header: connection.header,
        extraData: connection.postData != null ? connection.postData : null,
        expectDataType: http.HttpDataType.ARRAY_BUFFER, // 可选，指定返回数据的类型
        usingCache: false,
        priority: 1,
      }, (err: BusinessError, data: http.HttpResponse) => {
      if (!err) {
        if (data.result instanceof ArrayBuffer) {
          //napiHttp.HttpClientOHOS_onResponse(connection.ptr, data.result, data.responseCode, data.header.toString());
        }
        else {

        }
        connection.httpRequest.destroy();
      }
      else {
        //napiHttp.HttpClientOHOS_onFailure(connection.ptr, err.code);//TODO
        connection.httpRequest.destroy();
       // HttpClientOHOS_onProgress: (ptr: number, total: number, now: number, speed: number) => void;
      }
    });
  }
  public static setReadTimeout(connection: object, miliseconds: number) {
    log.debug("call LayaHttpClient.setReadTimeout: %{public}d ", miliseconds)
  }
  public static setConnectTimeout(connection: object, miliseconds: number) {
    log.debug("call LayaHttpClient.setConnectTimeout: %{public}d ", miliseconds)
  }
}