#include "UploadTaskiOS.h"
#include <utils/Log.h>
#import <CFNetwork/CFNetwork.h>
#import <Foundation/Foundation.h>
#import <Foundation/NSURLResponse.h>

@interface FileUploadManager : NSObject <NSURLSessionTaskDelegate, NSURLSessionDataDelegate>

@property (nonatomic, strong) NSURLSession *session;
@property (nonatomic, strong) NSURLSessionUploadTask *task;
@property (nonatomic, strong) NSMutableData *responseData;
@property (nonatomic, copy) void (^progressBlock)(double progress, double totalBytesSent, double totalBytesExpectedToSend);
@property (nonatomic, copy) void (^completionBlock)(BOOL success, NSError *error);
@property (nonatomic, copy) void (^headersReceivedBlock)(NSDictionary *headers);
@property (nonatomic, copy) void (^onSuccessBlock)(NSString *data, int statusCode);
@property (nonatomic, copy) void (^onFailBlock)(NSString *error, int statusCode);
@property (nonatomic, assign) BOOL isPaused;

// 添加新的属性
@property (nonatomic, strong) NSDictionary *headers;

- (void)uploadFile:(NSString *)filePath 
            toURL:(NSString *)urlString 
        parameters:(NSDictionary *)parameters 
          progress:(void(^)(double progress, double totalBytesSent, double totalBytesExpectedToSend))progressBlock
        completion:(void(^)(BOOL success, NSError *error))completion
    headersReceived:(void(^)(NSDictionary *headers))headersReceivedBlock;
- (void)pauseUpload;
- (void)resumeUpload;
- (void)cancelUpload;

@end

@implementation FileUploadManager

- (instancetype)init {
    self = [super init];
    if (self) {
        NSURLSessionConfiguration *config = [NSURLSessionConfiguration defaultSessionConfiguration];
        self.session = [NSURLSession sessionWithConfiguration:config 
                                                   delegate:self 
                                              delegateQueue:[NSOperationQueue mainQueue]];
        self.responseData = [NSMutableData data];
        self.isPaused = NO;
    }
    return self;
}

- (void)uploadFile:(NSString *)filePath 
            toURL:(NSString *)urlString 
        parameters:(NSDictionary *)parameters 
          progress:(void(^)(double progress, double totalBytesSent, double totalBytesExpectedToSend))progressBlock
        completion:(void(^)(BOOL success, NSError *error))completion
    headersReceived:(void(^)(NSDictionary *headers))headersReceivedBlock {
    
    self.progressBlock = progressBlock;
    self.completionBlock = completion;
    self.headersReceivedBlock = headersReceivedBlock;
    self.isPaused = NO;
    [self.responseData setLength:0];
    
    // 创建请求
    NSURL *url = [NSURL URLWithString:urlString];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = @"POST";
    
    // 设置边界
    NSString *boundary = [NSString stringWithFormat:@"Boundary-%@", [[NSUUID UUID] UUIDString]];
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@", boundary];
    [request setValue:contentType forHTTPHeaderField:@"Content-Type"];
    
    // 添加自定义请求头
    [self.headers enumerateKeysAndObjectsUsingBlock:^(NSString *key, NSString *value, BOOL *stop) {
        [request setValue:value forHTTPHeaderField:key];
    }];
    
    // 创建请求体
    NSMutableData *body = [NSMutableData data];
    
    // 添加参数
    [parameters enumerateKeysAndObjectsUsingBlock:^(NSString *key, NSString *value, BOOL *stop) {
        [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"\r\n\r\n", key] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"%@\r\n", value] dataUsingEncoding:NSUTF8StringEncoding]];
    }];
    
    // 添加文件数据
    NSData *fileData = [NSData dataWithContentsOfFile:filePath];
    NSString *fileName = [filePath lastPathComponent];
    
    [body appendData:[[NSString stringWithFormat:@"--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"file\"; filename=\"%@\"\r\n", fileName] dataUsingEncoding:NSUTF8StringEncoding]];
    [body appendData:[@"Content-Type: application/octet-stream\r\n\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
    [body appendData:fileData];
    [body appendData:[@"\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
    
    // 添加结束标记
    [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    
    // 设置请求体
    //request.HTTPBody = body;
    
    // 创建上传任务
    self.task = [self.session uploadTaskWithRequest:request fromData:body];
    [self.task resume];
}

- (void)pauseUpload {
    if (self.task && !self.isPaused) {
        [self.task suspend];
        self.isPaused = YES;
    }
}

- (void)resumeUpload {
    if (self.task && self.isPaused) {
        [self.task resume];
        self.isPaused = NO;
    }
}

- (void)cancelUpload {
    if (self.task) {
        [self.task cancel];
        self.task = nil;
        self.isPaused = NO;
    }
}

#pragma mark - NSURLSessionTaskDelegate

- (void)URLSession:(NSURLSession *)session 
              task:(NSURLSessionTask *)task 
   didSendBodyData:(int64_t)bytesSent 
    totalBytesSent:(int64_t)totalBytesSent 
totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend {
    double progress = (double)totalBytesSent / (double)totalBytesExpectedToSend;
    if (self.progressBlock) {
        self.progressBlock(progress, totalBytesSent, totalBytesExpectedToSend);
    }
}

- (void)URLSession:(NSURLSession *)session 
              task:(NSURLSessionTask *)task 
didCompleteWithError:(NSError *)error {
    if (error) {
        if (self.completionBlock) {
            self.completionBlock(NO, error);
        }
        return;
    }
    
    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)task.response;
    if (httpResponse.statusCode == 200) {
        if (self.onSuccessBlock) {
            self.onSuccessBlock([self.responseData base64EncodedStringWithOptions:0], httpResponse.statusCode);
        }
        if (self.completionBlock) {
            self.completionBlock(YES, nil);
        }
    } else {
        NSError *statusError = [NSError errorWithDomain:@"UploadError" 
                                                 code:httpResponse.statusCode 
                                             userInfo:@{NSLocalizedDescriptionKey: @"上传失败"}];   
        if (self.onFailBlock) {
            self.onFailBlock([statusError localizedDescription], httpResponse.statusCode);
        }
        if (self.completionBlock) {
            self.completionBlock(NO, statusError);
        }
    }
}

- (void)URLSession:(NSURLSession *)session 
              task:(NSURLSessionTask *)task 
didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge 
 completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential *))completionHandler {
    // 处理 SSL 证书验证
    if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust]) {
        NSURLCredential *credential = [NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust];
        completionHandler(NSURLSessionAuthChallengeUseCredential, credential);
    } else {
        completionHandler(NSURLSessionAuthChallengePerformDefaultHandling, nil);
    }
}

#pragma mark - NSURLSessionDataDelegate

- (void)URLSession:(NSURLSession *)session 
          dataTask:(NSURLSessionDataTask *)dataTask 
    didReceiveData:(NSData *)data {
    [self.responseData appendData:data];
}

- (void)URLSession:(NSURLSession *)session 
          dataTask:(NSURLSessionDataTask *)dataTask 
didReceiveResponse:(NSURLResponse *)response 
 completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler {
    
    // 获取响应头
    NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
    NSDictionary *headers = [httpResponse allHeaderFields];
    
    // 调用 headersReceivedBlock
    if (self.headersReceivedBlock) {
        self.headersReceivedBlock(headers);
    }
    
    // 继续处理响应
    completionHandler(NSURLSessionResponseAllow);
}

- (void)dealloc {
    [self cancelUpload];
    self.session = nil;
    self.task = nil;
    self.responseData = nil;
    self.progressBlock = nil;
    self.completionBlock = nil;
    self.headersReceivedBlock = nil;
    self.onSuccessBlock = nil;
    self.onFailBlock = nil;
}

@end

namespace laya {

void UploadTaskiOS::upload(const Description &description, onSuccessCallback successCallback, onFailCallback failCallback, onCompleteCallback completeCallback, onHeadersReceivedCallback headersReceivedCallback, onProgressUpdateCallback progressUpdateCallback) {

    FileUploadManager* manager = [[FileUploadManager alloc] init];
    m_iosManager = (__bridge_retained void*)manager;

    NSString *nsUrl = [NSString stringWithUTF8String:description.url.c_str()];
    NSString *nsFilePath = [NSString stringWithUTF8String:description.filePath.c_str()];
    
    // 转换 headers
    NSMutableDictionary *headers = [NSMutableDictionary dictionary];
    for (const auto& pair : description.headers) {
        headers[[NSString stringWithUTF8String:pair.first.c_str()]] = 
            [NSString stringWithUTF8String:pair.second.c_str()];
    }
    
    // 转换 formData
    NSMutableDictionary *formData = [NSMutableDictionary dictionary];
    for (const auto& pair : description.formData) {
        formData[[NSString stringWithUTF8String:pair.first.c_str()]] = 
            [NSString stringWithUTF8String:pair.second.c_str()];
    }
    // 设置 FileUploadManager 的 headers
    manager.headers = headers;
    
    [manager uploadFile:nsFilePath
                                          toURL:nsUrl
                                     parameters:formData 
                                       progress:^(double progress, double totalBytesSent, double totalBytesExpectedToSend) {
        if (progressUpdateCallback) {
            progressUpdateCallback(progress, totalBytesSent, totalBytesExpectedToSend);
        }
    } 
                                     completion:^(BOOL success, NSError *error) {
        if (success) {
            if (successCallback) {
                NSData *responseData = manager.responseData;
                NSString *responseString = [[NSString alloc] initWithData:responseData 
                                                               encoding:NSUTF8StringEncoding];
                successCallback(200, [responseString UTF8String]);
            }
        } else {
            if (failCallback) {
                std::string errorMsg = error ? [[error localizedDescription] UTF8String] : "";
                failCallback(error.code, errorMsg);
            }
        }
        
        if (completeCallback) {
            completeCallback();
        }
    }
    headersReceived:^(NSDictionary *headers) {
        if (headersReceivedCallback) {
            // 转换 headers 为 C++ 格式
            std::unordered_map<std::string, std::string> cppHeaders;
            auto cppHeadersPtr = &cppHeaders;
            [headers enumerateKeysAndObjectsUsingBlock:^(NSString *key, NSString *value, BOOL *stop) {
                std::string keyStr = [key UTF8String];
                std::string valueStr = [value UTF8String];
                cppHeadersPtr->insert(std::make_pair(keyStr, valueStr));
            }];
        
            headersReceivedCallback(cppHeaders);
        }
    }];
}

void UploadTaskiOS::pause() {
    if (!m_isPaused) {
        [(__bridge FileUploadManager*)m_iosManager pauseUpload];
        m_isPaused = true;
    }
}

void UploadTaskiOS::resume() {
    if (m_isPaused) {
        [(__bridge FileUploadManager*)m_iosManager resumeUpload];
        m_isPaused = false;
    }
}

void UploadTaskiOS::abort() {
    [(__bridge FileUploadManager*)m_iosManager cancelUpload];
    m_isPaused = false;
}


UploadTaskiOS::~UploadTaskiOS() {
    if (m_iosManager) {
        FileUploadManager* manager = (__bridge_transfer FileUploadManager*)m_iosManager;
        manager = nil;
        m_iosManager = nullptr;
    }
} 
} // namespace laya
