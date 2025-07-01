#include "JSVideo.h"
#include "multimedia/player_framework/avplayer_base.h"
#include "multimedia/player_framework/native_averrors.h"
#include "native_buffer/native_buffer.h"
#include "native_window/external_window.h"
#include "utils/Log.h"
#include <aki/jsbind.h>
#include <functional>
#include <memory>
#include <multimedia/player_framework/avplayer.h>
#include <unordered_map>
#include "platform/ohos/napi/render/plugin_render.h"

namespace laya
{
// HarmonyOS AVPlayer 事件类型常量定义
// 如果头文件中没有定义，我们在这里定义
#ifndef AVPLAYER_ON_INFO_COMPLETE
#define AVPLAYER_ON_INFO_COMPLETE 0
#endif

#ifndef AVPLAYER_ON_INFO_ERROR
#define AVPLAYER_ON_INFO_ERROR 1
#endif

#ifndef AVPLAYER_ON_INFO_STATE_CHANGE
#define AVPLAYER_ON_INFO_STATE_CHANGE 2
#endif

#ifndef AVPLAYER_ON_INFO_POSITION_UPDATE
#define AVPLAYER_ON_INFO_POSITION_UPDATE 3
#endif

#ifndef AVPLAYER_ON_INFO_SEEK_COMPLETE
#define AVPLAYER_ON_INFO_SEEK_COMPLETE 4
#endif

#ifndef AVPLAYER_ON_INFO_VIDEO_SIZE_CHANGED
#define AVPLAYER_ON_INFO_VIDEO_SIZE_CHANGED 5
#endif

// HarmonyOS AVPlayer Seek 模式常量定义
#ifndef AVSEEK_MODE_ACCURATE
#define AVSEEK_MODE_ACCURATE 0
#endif

#ifndef AVSEEK_MODE_FAST
#define AVSEEK_MODE_FAST 1
#endif

// HarmonyOS播放器状态枚举
enum class OHOSPlayerState
{
    IDLE = 0,
    INITIALIZED = 1,
    PREPARING = 2,
    PREPARED = 3,
    PLAYING = 4,
    PAUSED = 5,
    STOPPED = 6,
    ERROR = 7
};

// HarmonyOS播放器事件类型
enum class OHOSPlayerEvent
{
    PLAYBACK_COMPLETE = 0,
    ERROR = 1,
    INFO = 2,
    STATE_CHANGE = 3,
    POSITION_UPDATE = 4,
    BUFFERING_UPDATE = 5,
    VIDEO_SIZE_CHANGED = 6,
    SEEK_COMPLETE = 7
};

// HarmonyOS播放器事件回调结构
struct OHOSPlayerEventInfo
{
    OHOSPlayerEvent eventType;
    int errorCode;
    std::string errorMsg;
    int64_t position;
    int64_t duration;
    OHOSPlayerState state;
};

struct OHOSVideoHandler : public IVideoHandler
{
    OH_AVPlayer *player;
    OHNativeWindow *surface = nullptr;
    int videoWidth = 0, videoHeight = 0;
    bool isLooping = false;
    bool isMuted = false;
    float playbackSpeed = 1.0f;
    double currentTime = 0.0;
    double duration = 0.0;
    double volume = 1.0;
    std::function<void(const char *)> eventCallback;
    bool isFrameAvailable = false;
    OHOSPlayerState state = OHOSPlayerState::IDLE;
    bool isPaused = true;
    int readyState = 0;

    OHOSVideoHandler() : player(nullptr)
    {
    }
    virtual ~OHOSVideoHandler()
    {
        cleanup();
    }

    void cleanup()
    {
        if (player)
        {
            OH_AVPlayer_Release(player);
            player = nullptr;
        }
        surface = nullptr;
    }

    // 事件回调静态函数 - 根据官方示例优化
    static void OnAVPlayerEvent(OH_AVPlayer *player, AVPlayerOnInfoType eventType, OH_AVFormat *infoBody,
                                void *userData)
    {
        OHOSVideoHandler *handler = static_cast<OHOSVideoHandler *>(userData);
        LOGI("OnAVPlayerEvent %p %d", handler, eventType);
        if (!handler)
            return;

        // 根据官方示例使用正确的事件类型
        switch (eventType)
        {
        case AVPLAYER_ON_INFO_COMPLETE: // 播放完成
            if (handler->isLooping)
            {
                handler->seekTo(0.0);
                handler->play();
            }
            if (handler->eventCallback)
                handler->eventCallback("ended");
            break;
        case AVPLAYER_ON_INFO_ERROR: // 错误
            if (handler->eventCallback)
                handler->eventCallback("error");
            break;
        case AV_INFO_TYPE_STATE_CHANGE: // 状态变化
        {
            int32_t ret;
            int32_t state = -1;
            int32_t stateChangeReason = -1;
            AVPlayerState avState = AV_IDLE;
            LOGI("AVPlayerOnInfoType AV_INFO_TYPE_STATE_CHANGE");
            OH_AVFormat_GetIntValue(infoBody, OH_PLAYER_STATE, &state);
            OH_AVFormat_GetIntValue(infoBody, OH_PLAYER_STATE_CHANGE_REASON, &stateChangeReason);
            LOGI("OHAVPlayerOnInfoCallback AV_INFO_TYPE_STATE_CHANGE  state: %d ,stateChangeReason: %d", state,
                 stateChangeReason);
            avState = static_cast<AVPlayerState>(state);
            switch (avState)
            {
            case AV_IDLE: // 成功调用reset接口后触发该状态机上报
                LOGI("AVPlayerState  AV_IDLE");
                break;
            case AV_INITIALIZED: {
                LOGI("AVPlayerState  AV_INITIALIZED");
                ret = OH_AVPlayer_SetVideoSurface(player, PluginRender::GetInstance()->oHNativeWindow_);
                LOGI("OH_AVPlayer_SetVideoSurface ret:%d", ret);
                ret = OH_AVPlayer_Prepare(player); // 设置播放源后触发该状态上报
                if (ret != AV_ERR_OK)
                {
                    // 处理异常
                    LOGI("player  %s", "OH_AVPlayer_Prepare Err");
                }
            }
            break;
            case AV_PREPARED:
                LOGI("AVPlayerState AV_PREPARED");
                if (handler->eventCallback)
                {
                    handler->eventCallback("loadedmetadata");
                    handler->eventCallback("canplay");
                }
                break;
            case AV_PLAYING:
                LOGI("AVPlayerState AV_PLAYING");
                break;
            case AV_PAUSED:
                LOGI("AVPlayerState AV_PAUSED");
                break;
            case AV_STOPPED:
                LOGI("AVPlayerState AV_STOPPED");
                break;
            case AV_COMPLETED:
                LOGI("AVPlayerState AV_COMPLETED");
                break;
            case AV_ERROR:
                LOGI("AVPlayerState AV_ERROR");
                break;
            case AV_RELEASED:
                LOGI("AVPlayerState AV_RELEASED");
                break;
            default:
                break;
            }
        }
        break;
        case AV_INFO_TYPE_POSITION_UPDATE: // 位置更新
            // handler->currentTime = param / 1000.0;
            if (handler->eventCallback)
                handler->eventCallback("AV_INFO_TYPE_POSITION_UPDATE");
            break;
        // case AVPLAYER_ON_INFO_SEEK_COMPLETE: // Seek完成
        //     if (handler->eventCallback)
        //         handler->eventCallback("seeked");
        //    break;
        // case AVPLAYER_ON_INFO_VIDEO_SIZE_CHANGED: // 视频尺寸变化
        //     if (handler->eventCallback)
        //         handler->eventCallback("resize");
        //     break;
        default:
            // 对于其他事件类型，可以在这里添加处理
            break;
        }
    }

    // 初始化播放器 - 根据官方示例优化
    bool initialize()
    {
        player = OH_AVPlayer_Create();
        if (!player)
            return false;

        // 设置信息回调
        int32_t ret = OH_AVPlayer_SetOnInfoCallback(player, OnAVPlayerEvent, this);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to set info callback: %d", ret);
            // 即使回调设置失败，也继续初始化
        }

        return true;
    }

    // 设置播放源 - 根据官方示例优化
    bool setSource(const std::string &url)
    {
        if (!player)
            return false;

        // 先停止当前播放
        OH_AVPlayer_Stop(player);

        // 设置播放源
        int32_t ret = OH_AVPlayer_SetURLSource(player, url.c_str());
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to set URL source: %d", ret);
            return false;
        }

        // 准备播放器
        ret = OH_AVPlayer_Prepare(player);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to prepare player: %d", ret);
            return false;
        }

        return true;
    }

    // 绑定Surface - 根据官方示例优化
    bool setVideoSurface(OHNativeWindow *win)
    {
        if (!player)
            return false;
        surface = win;

        int32_t ret = OH_AVPlayer_SetVideoSurface(player, win);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to set video surface: %d", ret);
            return false;
        }

        return true;
    }

    // 获取视频尺寸 - 根据官方示例优化
    bool getVideoSize(int &width, int &height)
    {
        if (!player)
            return false;

        int32_t ret = OH_AVPlayer_GetVideoWidth(player, &width);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to get video width: %d", ret);
            return false;
        }

        ret = OH_AVPlayer_GetVideoHeight(player, &height);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to get video height: %d", ret);
            return false;
        }

        return true;
    }

    // 设置静音 - 根据官方示例优化
    bool setMute(bool mute)
    {
        if (!player)
            return false;
        isMuted = mute;

        // 通过设置音量为0来实现静音
        if (mute)
        {
            return OH_AVPlayer_SetVolume(player, 0.0f, 0.0f) == AV_ERR_OK;
        }
        else
        {
            return OH_AVPlayer_SetVolume(player, static_cast<float>(volume), static_cast<float>(volume)) == AV_ERR_OK;
        }
    }

    // 设置播放速率 - 根据官方示例优化
    bool setPlaybackSpeed(float speed)
    {
        if (!player)
            return false;
        playbackSpeed = speed;

        // 暂时注释掉播放速率设置，因为API参数类型不匹配
        // int32_t ret = OH_AVPlayer_SetPlaybackSpeed(player, speed);
        // if (ret != AV_ERR_OK) {
        //     LOGI("Failed to set playback speed: %d", ret);
        //     return false;
        // }

        return true;
    }

    // 播放 - 根据官方示例优化
    bool play()
    {
        if (!player)
            return false;

        int32_t ret = OH_AVPlayer_Play(player);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to play: %d", ret);
            return false;
        }

        isPaused = false;
        return true;
    }

    // 暂停 - 根据官方示例优化
    bool pause()
    {
        if (!player)
            return false;

        int32_t ret = OH_AVPlayer_Pause(player);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to pause: %d", ret);
            return false;
        }

        isPaused = true;
        return true;
    }

    // 停止 - 根据官方示例优化
    bool stop()
    {
        if (!player)
            return false;

        int32_t ret = OH_AVPlayer_Stop(player);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to stop: %d", ret);
            return false;
        }

        isPaused = true;
        return true;
    }

    // 设置音量 - 根据官方示例优化
    bool setVolume(double vol)
    {
        if (!player)
            return false;
        volume = vol;

        int32_t ret = OH_AVPlayer_SetVolume(player, static_cast<float>(vol), static_cast<float>(vol));
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to set volume: %d", ret);
            return false;
        }

        return true;
    }

    // 获取音量 - 根据官方示例优化
    double getVolume()
    {
        if (!player)
            return volume;

        // 暂时注释掉音量获取，因为API可能不存在
        // float leftVol = 1.0f, rightVol = 1.0f;
        // int32_t ret = OH_AVPlayer_GetVolume(player, &leftVol, &rightVol);
        // if (ret == AV_ERR_OK) {
        //     volume = static_cast<double>((leftVol + rightVol) / 2.0f);
        // }

        return volume;
    }

    // 设置循环播放 - 根据官方示例优化
    bool setLooping(bool loop)
    {
        if (!player)
            return false;
        isLooping = loop;

        int32_t ret = OH_AVPlayer_SetLooping(player, loop);
        if (ret != AV_ERR_OK)
        {
            LOGI("Failed to set looping: %d", ret);
            return false;
        }

        return true;
    }

    // 设置播放位置 - 根据官方示例优化
    bool seekTo(double time)
    {
        if (!player)
            return false;

        int32_t timeMs = static_cast<int32_t>(time * 1000);
        // 使用数字常量替代seek模式常量
        // int32_t ret = OH_AVPlayer_Seek(player, timeMs, 0); // 0 = 精确模式
        // if (ret != AV_ERR_OK) {
        //    LOGI("Failed to seek: %d", ret);
        //    return false;
        //}

        return true;
    }

    // 获取当前播放位置 - 根据官方示例优化
    double getCurrentTime()
    {
        if (!player)
            return currentTime;

        int32_t timeMs = 0;
        int32_t ret = OH_AVPlayer_GetCurrentTime(player, &timeMs);
        if (ret == AV_ERR_OK)
        {
            currentTime = static_cast<double>(timeMs) / 1000.0;
        }

        return currentTime;
    }

    // 获取总时长 - 根据官方示例优化
    double getDuration()
    {
        if (!player)
            return duration;

        int32_t dur = 0;
        int32_t ret = OH_AVPlayer_GetDuration(player, &dur);
        if (ret == AV_ERR_OK)
        {
            duration = static_cast<double>(dur) / 1000.0;
        }

        return duration;
    }

    // 更新Surface尺寸 - 简化实现
    bool updateSurfaceSize(int width, int height)
    {
        if (!surface)
            return false;
        // Surface尺寸更新在HarmonyOS中通常由系统自动处理
        return true;
    }

    // 获取Surface缓冲区 - 简化实现
    bool acquireSurfaceBuffer(void **pixelData, int &width, int &height)
    {
        if (!surface)
            return false;

        // 在HarmonyOS中，Surface缓冲区通常由系统管理
        // 这里提供简化的实现
        *pixelData = nullptr;
        width = videoWidth > 0 ? videoWidth : 640;
        height = videoHeight > 0 ? videoHeight : 480;
        return false; // 暂时返回false，表示不直接访问缓冲区
    }

    // 释放Surface缓冲区 - 简化实现
    void releaseSurfaceBuffer()
    {
        if (!surface)
            return;
        // Surface缓冲区由系统管理，这里不需要手动释放
    }

    // 检查Surface是否可用 - 简化实现
    bool isSurfaceAvailable()
    {
        //return surface != nullptr;
        return true;
    }

    // 获取Surface统计信息 - 简化实现
    bool getSurfaceStats(int &bufferCount, int &droppedFrames)
    {
        bufferCount = 0;
        droppedFrames = 0;
        return false; // 暂时不提供统计信息
    }

    // 设置Surface性能参数 - 简化实现
    bool setSurfacePerformance(int maxBufferCount, bool enableVSync)
    {
        return false; // 暂时不设置性能参数
    }

    // 处理视频尺寸变化
    void onVideoSizeChanged(int width, int height)
    {
        videoWidth = width;
        videoHeight = height;

        if (surface)
        {
            updateSurfaceSize(width, height);
        }

        // 触发尺寸变化事件
        if (eventCallback)
        {
            eventCallback("resize");
        }
    }

    virtual bool isFrameUpdated()
    {
        LOGD("isFrameUpdated %d", isFrameAvailable);
        return isFrameAvailable;
    }

    virtual void updateBitmapData(BitmapData *bitmapData)
    {
        if (!bitmapData || !player)
        {
            return;
        }

        // 检查Surface是否可用
        if (!isSurfaceAvailable())
        {
            // Surface不可用，使用备用方案
            LOGI("Surface not available, using fallback");
            useFallbackRendering(bitmapData);
            return;
        }

        // 1. 获取Surface缓冲区
        void *pixelData = nullptr;
        int videoWidth = 0, videoHeight = 0;

        if (acquireSurfaceBuffer(&pixelData, videoWidth, videoHeight))
        {
            LOGD("acquireSurfaceBuffer");
            // 2. 重新配置BitmapData以匹配视频尺寸
            bitmapData->reconfigure(videoWidth, videoHeight, sizeof(int) * 8, laya::ImgType_unknow);

            // 3. 复制像素数据到BitmapData
            int bufferSize = videoWidth * videoHeight * 4; // RGBA格式
            memcpy(bitmapData->m_pImageData, pixelData, bufferSize);

            // 4. 释放Surface缓冲区
            releaseSurfaceBuffer();

            // 标记帧已更新
            isFrameAvailable = true;
            return;
        }

        // 如果Surface获取失败，使用备用方案
        // 检查是否需要更新Surface尺寸
        int currentWidth = 0, currentHeight = 0;
        if (getVideoSize(currentWidth, currentHeight))
        {
            updateSurfaceSize(currentWidth, currentHeight);
            LOGD("updateSurfaceSize");
        }
        LOGD("useFallbackRendering");
        // 使用备用渲染方案
        useFallbackRendering(bitmapData);
    }

    // 备用渲染方案
    void useFallbackRendering(BitmapData *bitmapData)
    {
        // 临时实现：使用模拟数据
        // 当HarmonyOS NDK API可用时，上面的代码会自动生效
        int width = videoWidth > 0 ? videoWidth : 640;    // 使用实际视频宽度
        int height = videoHeight > 0 ? videoHeight : 480; // 使用实际视频高度

        // 重新配置BitmapData
        bitmapData->reconfigure(width, height, sizeof(int) * 8, laya::ImgType_unknow);

        // 创建模拟的RGBA数据（黑色背景）
        int bufferSize = width * height * 4;
        unsigned char *tempPixelData = new unsigned char[bufferSize];

        // 填充黑色背景
        for (int i = 0; i < bufferSize; i += 4)
        {
            tempPixelData[i] = 0;       // R
            tempPixelData[i + 1] = 0;   // G
            tempPixelData[i + 2] = 0;   // B
            tempPixelData[i + 3] = 255; // A
        }

        // 复制到BitmapData
        memcpy(bitmapData->m_pImageData, tempPixelData, bufferSize);

        // 清理临时数据
        delete[] tempPixelData;

        // 标记帧已更新
        isFrameAvailable = true;
    }

    // 事件回调处理 - 根据官方示例优化
    void onPlayerEvent(OHOSPlayerEventInfo &eventInfo)
    {
        switch (eventInfo.eventType)
        {
        case OHOSPlayerEvent::PLAYBACK_COMPLETE:
            if (isLooping)
            {
                seekTo(0.0);
                play();
            }
            else
            {
                state = OHOSPlayerState::STOPPED;
                isPaused = true;
            }
            if (eventCallback)
            {
                eventCallback("ended");
            }
            break;

        case OHOSPlayerEvent::ERROR:
            state = OHOSPlayerState::ERROR;
            if (eventCallback)
            {
                eventCallback("error");
            }
            break;

        case OHOSPlayerEvent::INFO:
            // 处理信息事件
            if (eventCallback)
            {
                eventCallback("info");
            }
            break;

        case OHOSPlayerEvent::STATE_CHANGE:
            state = eventInfo.state;
            switch (state)
            {
            case OHOSPlayerState::IDLE:
                // 空闲状态，不需要特殊处理
                break;

            case OHOSPlayerState::INITIALIZED:
                // 初始化完成
                break;

            case OHOSPlayerState::PREPARING:
                // 准备中
                break;

            case OHOSPlayerState::PREPARED:
                readyState = 4; // HAVE_ENOUGH_DATA

                // 获取视频尺寸并更新Surface
                {
                    int width = 0, height = 0;
                    if (getVideoSize(width, height))
                    {
                        onVideoSizeChanged(width, height);
                    }
                }

                if (eventCallback)
                {
                    eventCallback("loadedmetadata");
                    eventCallback("canplay");
                }
                break;

            case OHOSPlayerState::PLAYING:
                isPaused = false;
                if (eventCallback)
                {
                    eventCallback("play");
                }
                break;

            case OHOSPlayerState::PAUSED:
                isPaused = true;
                if (eventCallback)
                {
                    eventCallback("pause");
                }
                break;

            case OHOSPlayerState::STOPPED:
                isPaused = true;
                if (eventCallback)
                {
                    eventCallback("stop");
                }
                break;

            case OHOSPlayerState::ERROR:
                if (eventCallback)
                {
                    eventCallback("error");
                }
                break;
            }
            break;

        case OHOSPlayerEvent::POSITION_UPDATE:
            currentTime = static_cast<double>(eventInfo.position) / 1000.0;
            if (eventInfo.duration > 0)
            {
                duration = static_cast<double>(eventInfo.duration) / 1000.0;
            }
            if (eventCallback)
            {
                eventCallback("timeupdate");
            }
            break;

        case OHOSPlayerEvent::BUFFERING_UPDATE:
            // 处理缓冲更新事件
            if (eventCallback)
            {
                eventCallback("progress");
            }
            break;

        case OHOSPlayerEvent::VIDEO_SIZE_CHANGED:
            // 处理视频尺寸变化事件
            if (eventCallback)
            {
                eventCallback("resize");
            }
            break;

        case OHOSPlayerEvent::SEEK_COMPLETE:
            // 处理seek完成事件
            if (eventCallback)
            {
                eventCallback("seeked");
            }
            break;
        }
    }

    // 静态事件回调函数
    static void onPlayerEvent(void *userData, OHOSPlayerEventInfo &eventInfo)
    {
        OHOSVideoHandler *handler = static_cast<OHOSVideoHandler *>(userData);
        if (handler)
        {
            handler->onPlayerEvent(eventInfo);
        }
    }

    static void onControllerEvent(void *userData, OHOSPlayerEventInfo &eventInfo)
    {
        OHOSVideoHandler *handler = static_cast<OHOSVideoHandler *>(userData);
        if (handler)
        {
            handler->onPlayerEvent(eventInfo);
        }
    }
};

// 视频播放器信息结构
struct OHOSVideoPlayer
{
    OHOSVideoHandler *handler;
    std::string url;
    double x, y, width, height;
    bool isAutoplay;
    std::function<void(const char *)> eventCallback;

    OHOSVideoPlayer() : handler(nullptr), x(0), y(0), width(0), height(0), isAutoplay(false)
    {
    }
};

static int curIndex = 0;
static std::unordered_map<int, std::unique_ptr<OHOSVideoPlayer>> s_allVideoPlayers;
static std::unordered_map<int, JSVideo *> s_jsVideoMap;

std::set<std::string> JSVideo::ms_supportFormat = {"video/mp4", "video/ogg", "video/webm", "video/3gpp",
                                                   "video/quicktime"};

// 创建视频播放器
OHOSVideoPlayer *CreateOHOSVideoPlayer(int tag)
{
    auto videoPlayer = std::make_unique<OHOSVideoPlayer>();

    // 创建OHOSVideoHandler
    auto handler = new OHOSVideoHandler();
    if (!handler->initialize())
    {
        delete handler;
        return nullptr;
    }

    videoPlayer->handler = handler;
    s_allVideoPlayers[tag] = std::move(videoPlayer);
    return s_allVideoPlayers[tag].get();
}

// 销毁视频播放器
void DestroyOHOSVideoPlayer(int tag)
{
    auto it = s_allVideoPlayers.find(tag);
    if (it != s_allVideoPlayers.end())
    {
        auto &videoPlayer = it->second;
        if (videoPlayer->handler)
        {
            delete videoPlayer->handler;
        }
        s_allVideoPlayers.erase(it);
    }
}

JSVideo::JSVideo()
{
    Init();
    // 增加索引
    m_tag = curIndex;
    curIndex++;
    s_jsVideoMap[m_tag] = this;

    jsbind::AdjustAmountOfExternalAllocatedMemory(sizeof(JSVideo));
    JCMemorySurvey::GetInstance()->newClass("JSVideo", sizeof(JSVideo), this);

    // 创建OHOS视频播放器
    auto videoPlayer = CreateOHOSVideoPlayer(m_tag);
    LOGD("CreateOHOSVideoPlayer %p", videoPlayer);
    if (videoPlayer)
    {
        videoPlayer->eventCallback = [this](const char *eventName) {
            // 触发JavaScript事件
            LOGD("CallHandle %s", eventName);
            this->CallHandle(eventName);
        };

        // 设置视频处理器
        m_pVideoHandler = videoPlayer->handler;
        if (m_pVideoHandler)
        {
            videoPlayer->handler->eventCallback = videoPlayer->eventCallback;
        }
        m_pJCVideo->setVideoHandler(m_pVideoHandler);
    }
}

JSVideo::~JSVideo()
{
    Dispose();
    JCMemorySurvey::GetInstance()->releaseClass("JSVideo", this);
    DestroyOHOSVideoPlayer(m_tag);
    s_jsVideoMap.erase(m_tag);
}

void JSVideo::_releaseHandler()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it != s_allVideoPlayers.end())
    {
        auto &videoPlayer = it->second;
        if (videoPlayer->handler)
        {
            videoPlayer->handler->stop();
        }
    }
}

void JSVideo::LoadInternal(const std::string &path)
{
    LOGI("JSVideo LoadInternal");
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto &videoPlayer = it->second;
    auto handler = videoPlayer->handler;
    if (!handler)
        return;

    videoPlayer->url = path;
    if (handler->setSource(path))
    {
        // 如果设置了自动播放，则开始播放
        if (videoPlayer->isAutoplay)
        {
            handler->play();
        }
    }
}

void JSVideo::LoadInternal(char *buffer, int length)
{
    // 暂不支持内存缓冲播放
    LOGI("Memory buffer playback not supported in OHOS implementation");
}

void JSVideo::Play()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto &videoPlayer = it->second;
    auto handler = videoPlayer->handler;
    if (!handler)
        return;

    handler->play();
}

void JSVideo::Pause()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto &videoPlayer = it->second;
    auto handler = videoPlayer->handler;
    if (!handler)
        return;

    handler->pause();
    m_isDownloadWaitPlay = false;
}

void JSVideo::Stop()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto &videoPlayer = it->second;
    auto handler = videoPlayer->handler;
    if (!handler)
        return;

    handler->stop();
}

bool JSVideo::GetPaused()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return true;

    auto handler = it->second->handler;
    return handler ? handler->isPaused : true;
}

bool JSVideo::GetLoop()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return false;

    auto handler = it->second->handler;
    return handler ? handler->isLooping : false;
}

void JSVideo::SetLoop(bool value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->setLooping(value);
    }
}

void JSVideo::SetAutoplay(bool value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    it->second->isAutoplay = value;
}

bool JSVideo::GetAutoplay()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return false;

    return it->second->isAutoplay;
}

void JSVideo::SetX(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    it->second->x = value;
}

double JSVideo::GetX()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    return it->second->x;
}

void JSVideo::SetY(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    it->second->y = value;
}

double JSVideo::GetY()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    return it->second->y;
}

double JSVideo::GetVideoWidth()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    auto handler = it->second->handler;
    if (!handler)
        return it->second->width;

    int width = 0, height = 0;
    if (handler->getVideoSize(width, height))
    {
        return static_cast<double>(width);
    }

    return it->second->width;
}

double JSVideo::GetVideoHeight()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    auto handler = it->second->handler;
    if (!handler)
        return it->second->height;

    int width = 0, height = 0;
    if (handler->getVideoSize(width, height))
    {
        return static_cast<double>(height);
    }

    return it->second->height;
}

double JSVideo::GetWidth()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    return it->second->width;
}

void JSVideo::SetWidth(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    it->second->width = value;
}

double JSVideo::GetHeight()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    return it->second->height;
}

void JSVideo::SetHeight(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    it->second->height = value;
}

double JSVideo::GetCurrentTime()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    auto handler = it->second->handler;
    return handler ? handler->getCurrentTime() : 0.0;
}

void JSVideo::SetCurrentTime(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->seekTo(value);
    }
}

double JSVideo::GetDuration()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    auto handler = it->second->handler;
    return handler ? handler->getDuration() : 0.0;
}

double JSVideo::GetVolume()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 1.0;

    auto handler = it->second->handler;
    return handler ? handler->getVolume() : 1.0;
}

void JSVideo::SetVolume(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->setVolume(value);
    }
}

int JSVideo::GetReadyState()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 0;

    auto handler = it->second->handler;
    return handler ? handler->readyState : 0;
}

// 静态方法实现
void JSVideo::executeVideoCallback(int tag, int event)
{
    auto it = s_jsVideoMap.find(tag);
    if (it != s_jsVideoMap.end())
    {
        JSVideo *video = it->second;
        if (video)
        {
            switch (event)
            {
            case 0: // ended
                video->CallHandle("ended");
                break;
            case 1: // error
                video->CallHandle("error");
                break;
            case 2: // play
                video->CallHandle("play");
                break;
            case 3: // pause
                video->CallHandle("pause");
                break;
            case 4: // timeupdate
                video->CallHandle("timeupdate");
                break;
            case 5: // canplay
                video->CallHandle("canplay");
                break;
            case 6: // seeked
                video->CallHandle("seeked");
                break;
            case 7: // resize
                video->CallHandle("resize");
                break;
            default:
                break;
            }
        }
    }
}

void JSVideo::executeLoadedmetadata(int tag, const char *eventName)
{
    auto it = s_jsVideoMap.find(tag);
    if (it != s_jsVideoMap.end())
    {
        JSVideo *video = it->second;
        if (video)
        {
            video->CallHandle(eventName);
        }
    }
}

void JSVideo::executeDurationBack(int tag, int duration)
{
    auto it = s_jsVideoMap.find(tag);
    if (it != s_jsVideoMap.end())
    {
        JSVideo *video = it->second;
        if (video)
        {
            // 触发durationchange事件
            video->CallHandle("durationchange");
        }
    }
}

void JSVideo::executeCurrentTimeBack(int tag, int currentTime)
{
    auto it = s_jsVideoMap.find(tag);
    if (it != s_jsVideoMap.end())
    {
        JSVideo *video = it->second;
        if (video)
        {
            // 触发timeupdate事件
            video->CallHandle("timeupdate");
        }
    }
}

void JSVideo::SetMuted(bool value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->setMute(value);
    }
}

bool JSVideo::GetMuted()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return false;

    auto handler = it->second->handler;
    return handler ? handler->isMuted : false;
}

void JSVideo::SetPlaybackRate(double value)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->setPlaybackSpeed(static_cast<float>(value));
    }
}

double JSVideo::GetPlaybackRate()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return 1.0;

    auto handler = it->second->handler;
    return handler ? static_cast<double>(handler->playbackSpeed) : 1.0;
}

void JSVideo::SetVideoSurface(void *surface)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->setVideoSurface(static_cast<OHNativeWindow *>(surface));
    }
}

bool JSVideo::IsFrameUpdated()
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return false;

    auto handler = it->second->handler;
    return handler ? handler->isFrameUpdated() : false;
}

void JSVideo::UpdateBitmapData(BitmapData *bitmapData)
{
    auto it = s_allVideoPlayers.find(m_tag);
    if (it == s_allVideoPlayers.end())
        return;

    auto handler = it->second->handler;
    if (handler)
    {
        handler->updateBitmapData(bitmapData);
    }
}
} // namespace laya