#include "HttpClientiOS.h"
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include "JCSystemConfig.h"
#import <CFNetwork/CFNetwork.h>
#import <Foundation/Foundation.h>
#import <Foundation/NSURLResponse.h>
#include <map>

@interface FileDownloader : NSObject <NSURLSessionDownloadDelegate>
{
    NSInteger statusCode;
    laya::HttpClientiOS *m_downloader;
}
@property(nonatomic, strong) NSURLSession *session;
@property(nonatomic, strong) NSString *path;
@property(nonatomic, strong) NSURLSessionDownloadTask *task;
- (id)init;
- (void)download:(NSURL *)url filePath:(NSString *)path progress:(laya::HttpClientiOS *)client;
- (void)URLSession:(NSURLSession *)session
                 downloadTask:(NSURLSessionDownloadTask *)downloadTask
    didFinishDownloadingToURL:(NSURL *)location;
- (void)URLSession:(NSURLSession *)session
                 downloadTask:(NSURLSessionDownloadTask *)downloadTask
                 didWriteData:(int64_t)bytesWritten
            totalBytesWritten:(int64_t)totalBytesWritten
    totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite;
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(nullable NSError *)error;
@end

@implementation FileDownloader
- (id)init
{
    return self;
}
- (void)download:(NSURL *)url filePath:(NSString *)path progress:(laya::HttpClientiOS *)client
{
    m_downloader = client;
    NSURLSessionConfiguration *defaultConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    defaultConfig.timeoutIntervalForRequest = client->m_readTimeout;
    // defaultConfig.timeoutIntervalForResource = client->m_connectTimeout;
    self.session =
        [NSURLSession sessionWithConfiguration:defaultConfig delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    self.path = path;
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    self.task = [self.session downloadTaskWithRequest:request];
    [self.task resume];
}
- (void)URLSession:(NSURLSession *)session
                 downloadTask:(NSURLSessionDownloadTask *)downloadTask
    didFinishDownloadingToURL:(NSURL *)location
{
    statusCode = ((NSHTTPURLResponse *)downloadTask.response).statusCode;
    if (statusCode >= 400)
    {
        return;
    }
    NSFileManager *manager = [NSFileManager defaultManager];

    NSError *error;
    [manager moveItemAtPath:location.path toPath:self.path error:&error];
    if (error != nil)
    {
        statusCode = 0;
    }
}
- (void)URLSession:(NSURLSession *)session
                 downloadTask:(NSURLSessionDownloadTask *)downloadTask
                 didWriteData:(int64_t)bytesWritten
            totalBytesWritten:(int64_t)totalBytesWritten
    totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    m_downloader->m_functionOnProgress((unsigned int)totalBytesExpectedToWrite, (unsigned int)totalBytesWritten, 0.0f);
}
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(nullable NSError *)error
{
    if (error != nil)
    {
        statusCode = 0;
    }
    // ���ļ�û��buffer
    std::shared_ptr<laya::Data> data = laya::Data::makeEmpty();
    m_downloader->m_functionOnEnd(data, "", "", 0/*CURLE_OK*/, statusCode, "");
    delete m_downloader;
    // dispatch_semaphore_signal(sem);
}
- (void)cancel
{
    if (self.task != nil)
    {
        [self.task cancel];
    }
}
@end
namespace laya
{
class HttpURLSessionDownloaderImpl
{
  public:
    HttpURLSessionDownloaderImpl(HttpClientiOS *client);

    ~HttpURLSessionDownloaderImpl();

    void getResponseData(char **ppData, int &length, const char *localFilePath, int64_t id);

    void addHeader(const std::string& key, const std::string& value);

    void postData(const char *pData, int nLen);

    void setMethod(const std::string& method);

    int getResponseCode();

    std::string getResponseHeaders();

    void setReadTimeout(int miliseconds);

    void setConnectTimeout(int miliseconds);

    void doRequest();

    void cancel();

    std::string m_method;

    std::map<std::string, std::string> m_headers;

    // int m_readTimeout = 10 * 1000;

    // int m_connectTimeout = 10 * 1000;

    char *m_pData = nullptr;

    int m_nLen = 0;

    NSURL *m_url;

    NSMutableString *m_headerStr;

    // std::string m_localFilePath;

    // onProgressFunction m_functionOnProgress;

    // onEndFunction m_functionOnEnd;

    HttpClientiOS *m_downloader;

    NSURLSessionDataTask *m_task = nil;

    FileDownloader *m_fileDownloader = nil;
};

HttpURLSessionDownloaderImpl::HttpURLSessionDownloaderImpl(HttpClientiOS *client)
{
    m_downloader = client;
    m_headerStr = [NSMutableString string];
    // m_functionOnEnd = functionOnEnd;
    // m_functionOnProgress = functionOnProgress;
    // m_localFilePath = localFilePath;

    std::string uriEncoded = encodeURI(m_downloader->m_url.c_str());
    NSString *strUrl = [NSString stringWithCString:uriEncoded.c_str() encoding:NSUTF8StringEncoding];
    m_url = [NSURL URLWithString:strUrl];
}

HttpURLSessionDownloaderImpl::~HttpURLSessionDownloaderImpl()
{
    m_url = nil;
    m_headerStr = nil;
    m_task = nil;
}

void HttpURLSessionDownloaderImpl::addHeader(const std::string& key, const std::string& value)
{
    m_headers.insert(std::make_pair(key, value));
}

void HttpURLSessionDownloaderImpl::postData(const char *pData, int nLen)
{
    m_pData = (char *)pData;
    m_nLen = nLen;
}

void HttpURLSessionDownloaderImpl::setMethod(const std::string& method)
{
    m_method = method;
}
static NSURLSession *s_singletonSession = nil;
void HttpURLSessionDownloaderImpl::doRequest()
{

    if (!m_downloader->m_localFilePath.empty())
    {
        m_fileDownloader = [[FileDownloader alloc] init];
        [m_fileDownloader download:m_url
                          filePath:[NSString stringWithUTF8String:m_downloader->m_localFilePath.c_str()]
                          progress:m_downloader];
    }
    else
    {

        NSURLSessionConfiguration *defaultConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
        defaultConfig.timeoutIntervalForRequest = m_downloader->m_readTimeout;
        // defaultConfig.timeoutIntervalForResource = m_downloader->m_connectTimeout;
        //one http request one session
#if  0
        NSURLSession *session = [NSURLSession sessionWithConfiguration:defaultConfig];
#else
        //one singleton shared session
        if (s_singletonSession == nil)
        {
            LOGD("HTTPMaximumConnectionsPerHost %d",defaultConfig.HTTPMaximumConnectionsPerHost);
            defaultConfig.HTTPMaximumConnectionsPerHost = 6;//default is 6
            s_singletonSession = [NSURLSession sessionWithConfiguration:defaultConfig];
            
        }
        NSURLSession *session = s_singletonSession;
        
#endif
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:m_url];
        //if (g_kSystemConfig.m_bUseDcc)
        {
            request.cachePolicy = NSURLRequestReloadIgnoringLocalCacheData; // DCC需关闭缓存
        }
        if (m_pData && m_nLen > 0)
        {
            request.HTTPMethod = @"POST";
            request.HTTPBody = [NSData dataWithBytes:(const void *)m_pData length:m_nLen];
        }
        else
        {
            request.HTTPMethod = @"GET";
        }

        std::map<std::string, std::string>::iterator it = m_headers.begin();
        for (; it != m_headers.end(); it++)
        {
            NSString *key = [NSString stringWithUTF8String:it->first.c_str()];
            NSString *value = [NSString stringWithUTF8String:it->second.c_str()];
            [request addValue:value forHTTPHeaderField:key];
        }
        __block NSInteger statusCode;
        // dispatch_semaphore_t sem = dispatch_semaphore_create(0);
        m_task = [session
            dataTaskWithRequest:request
              completionHandler:^(NSData *_Nullable data, NSURLResponse *_Nullable response, NSError *_Nullable error) {
                if (data != nil)
                {

                    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
                    statusCode = httpResponse.statusCode;

                    NSDictionary *Headers = httpResponse.allHeaderFields;

                    NSString *statusString = [NSHTTPURLResponse localizedStringForStatusCode:statusCode];
                    if (statusCode == 200)
                    {
                        statusString = @"OK";
                    }

                    [m_headerStr appendFormat:@"HTTP/1.1 %ld %@\n", (long)statusCode, statusString];
                    for (NSString *Key in [Headers allKeys])
                    {
                        [m_headerStr appendFormat:@"%@:%@\n", Key, [Headers objectForKey:Key]];
                    }

                    /*if (!m_localFilePath.empty()) {
                        NSError* e;
                        [data writeToFile:[NSString stringWithUTF8String:m_localFilePath.c_str()] options:0 error:&e];
                    }*/
                    auto responseDataLength = data.length;
                    if (responseDataLength <= 0)
                    {
                        std::shared_ptr<Data> d = Data::makeEmpty();
                        m_downloader->m_functionOnEnd(d, "", "", 0/*CURLE_OK*/, statusCode, getResponseHeaders());
                        delete m_downloader;
                        // onFailure(statusCode);
                    }
                    else
                    {
                        std::shared_ptr<Data> d = Data::makeWithCopy((void *)data.bytes, responseDataLength);
                        // request->m_responseCallback(buf, pCurl->m_strLocalAddr, pCurl->m_strSvAddr, 0/*CURLE_OK*/,
                        // pCurl->m_nResponseCode, pCurl->m_strResponseHead);
                        m_downloader->m_functionOnEnd(d, "", "", 0/*CURLE_OK*/, statusCode, getResponseHeaders());
                        delete m_downloader;
                    }
                }
                else
                {
                    if (error != nil && error.code == -999)
                    {
                        // cancel
                        NSLog(@"cancel");
                        delete m_downloader;
                    }
                    else
                    {
                        statusCode = 0;
                        NSLog(@"error no data");
                        // onFailure(statusCode);
                        if (m_downloader->m_functionOnEnd)
                        {
                            // curl ִ��ʧ��
                            static std::string nullstr;
                            std::shared_ptr<Data> data = Data::makeEmpty();
                            m_downloader->m_functionOnEnd(data, "", "", 7/*CURLE_COULDNT_CONNECT*/, statusCode, nullstr);
                        }
                        delete m_downloader;
                    }
                }
                // dispatch_semaphore_signal(sem);
              }];
        [m_task resume];
        // dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
        // return (int)statusCode;
    }
}
std::string HttpURLSessionDownloaderImpl::getResponseHeaders()
{
    return m_headerStr == nil ? "" : [m_headerStr UTF8String];
}

void HttpURLSessionDownloaderImpl::cancel()
{
    if (m_fileDownloader != nil)
    {
        [m_fileDownloader cancel];
    }
    else if (m_task != nil)
    {
        [m_task cancel];
    }
}
HttpClientiOS::HttpClientiOS(const std::string& url, const std::string&localFilePath, const onProgressFunction &functionOnProgress,
                             const onEndFunction &functionOnEnd, std::weak_ptr<HttpClientManager> httpClientManager)
    : IHttpClient(httpClientManager)
{
    m_url = url;
    m_localFilePath = localFilePath;
    m_functionOnEnd = functionOnEnd;
    m_functionOnProgress = functionOnProgress;
    m_impl = new HttpURLSessionDownloaderImpl(this);
}

HttpClientiOS::~HttpClientiOS()
{
    delete m_impl;
}

void HttpClientiOS::doRequest()
{
    m_impl->doRequest();
}

void HttpClientiOS::addHeader(const std::string& key, const std::string& value)
{
    m_impl->addHeader(key, value);
}
void HttpClientiOS::postData(const char *pData, int nLen)
{
    m_impl->postData(pData, nLen);
}

void HttpClientiOS::setMethod(const std::string& method)
{
    m_impl->setMethod(method);
}

void HttpClientiOS::setReadTimeout(int miliseconds)
{
    m_readTimeout = miliseconds / 1000.0;
}

void HttpClientiOS::setConnectTimeout(int miliseconds)
{
    m_connectTimeout = miliseconds / 1000.0;
}

void HttpClientiOS::cancel()
{
    m_impl->cancel();
}
} // namespace laya
