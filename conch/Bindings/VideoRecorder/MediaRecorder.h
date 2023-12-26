
#ifndef MEDIARECORDER_H
#define MEDIARECORDER_H
#ifdef USE_FFMPEG



#include "ImageDef.h"
//#include <render/audio/AudioRender.h>
#include "ThreadSafeQueue.h"
#include "thread"
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
extern "C" {

#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
//#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}


#define DEFAULT_SAMPLE_RATE    44100
class AudioFrame {
public:
	AudioFrame(uint8_t * data, int dataSize, bool hardCopy = true) {
		this->dataSize = dataSize;
		this->data = data;
		this->hardCopy = hardCopy;
		if (hardCopy) {
			this->data = static_cast<uint8_t *>(malloc(this->dataSize));
			memcpy(this->data, data, dataSize);
		}
	}

	~AudioFrame() {
		if (hardCopy && this->data)
			free(this->data);
		this->data = nullptr;
	}

	uint8_t * data = nullptr;
	int dataSize = 0;
	bool hardCopy = true;
};
typedef NativeImage VideoFrame;

class AVOutputStream {
public:
    AVOutputStream() {
        m_pStream = nullptr;
        m_pCodecCtx = nullptr;
        m_pFrame = nullptr;
        m_pTmpFrame = nullptr;
        m_pSwrCtx = nullptr;
        m_pSwsCtx = nullptr;
        m_NextPts = 0;
        m_SamplesCount = 0;
        m_EncodeEnd = 0;
    }

    ~AVOutputStream(){}
public:
    AVStream *m_pStream;
    AVCodecContext *m_pCodecCtx;
    volatile int64_t m_NextPts;
    volatile int m_EncodeEnd;
    int m_SamplesCount;
    AVFrame *m_pFrame;
    AVFrame *m_pTmpFrame;
    SwsContext *m_pSwsCtx;
    SwrContext *m_pSwrCtx;
};

struct RecorderParam {
    //video
    int frameWidth;
    int frameHeight;
    long videoBitRate;
    int fps;

    //audio
    int audioSampleRate;
    int channelLayout;
    int sampleFormat;
};

class MediaRecorder {
public:
    MediaRecorder(const char *url, RecorderParam *param);
    ~MediaRecorder();
    //开始录制
    int StartRecord();
    //添加音频数据到音频队列
    int OnFrame2Encode(AudioFrame *inputFrame);
    //添加视频数据到视频队列
    int OnFrame2Encode(VideoFrame *inputFrame);
    //停止录制
    int StopRecord();

private:

    static void StartMediaEncodeThread(MediaRecorder *recorder);
    //分配音频缓冲帧
    AVFrame *AllocAudioFrame(AVSampleFormat sample_fmt, uint64_t channel_layout, int sample_rate, int nb_samples);
    //分配视频缓冲帧
    AVFrame *AllocVideoFrame(AVPixelFormat pix_fmt, int width, int height);
    //写编码包到媒体文件
    int WritePacket(AVFormatContext *fmt_ctx, AVRational *time_base, AVStream *st, AVPacket *pkt);
    //添加媒体流程
    void AddStream(AVOutputStream *ost, AVFormatContext *oc, AVCodec **codec, AVCodecID codec_id);
    //打印 packet 信息
    void PrintfPacket(AVFormatContext *fmt_ctx, AVPacket *pkt);
    //打开音频编码器
    int OpenAudio(AVFormatContext *oc, AVCodec *codec, AVOutputStream *ost);
    //打开视频编码器
    int OpenVideo(AVFormatContext *oc, AVCodec *codec, AVOutputStream *ost);
    //编码一帧音频
    int EncodeAudioFrame(AVOutputStream *ost);
    //编码一帧视频
    int EncodeVideoFrame(AVOutputStream *ost);
    //释放编码器上下文
    void CloseStream(AVOutputStream *ost);

private:
    RecorderParam    m_RecorderParam = {0};
    AVOutputStream   m_VideoStream;
    AVOutputStream   m_AudioStream;
    char             m_OutUrl[1024] = {0};
    AVOutputFormat  *m_OutputFormat = nullptr;
    AVFormatContext *m_FormatCtx = nullptr;
    AVCodec         *m_AudioCodec = nullptr;
    AVCodec         *m_VideoCodec = nullptr;
    //视频帧队列
    ThreadSafeQueue<VideoFrame *>
                     m_VideoFrameQueue;
    //音频帧队列
    ThreadSafeQueue<AudioFrame *>
                     m_AudioFrameQueue;
    int              m_EnableVideo = 0;
    int              m_EnableAudio = 0;
    bool    m_Exit = false;
    //音频编码线程
    thread          *m_pAudioThread = nullptr;
    //视频编码线程
    thread          *m_pVideoThread = nullptr;
    thread          *m_pMediaThread = nullptr;
};

#endif // ANDROID
#endif //MEDIARECORDER_H
