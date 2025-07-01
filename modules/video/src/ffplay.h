#ifndef __ffplay_H__
#define __ffplay_H__

#include "ffplay_Clock.h"
#include "ffplay_Config.h"
#include "ffplay_Decoder.h"
#include "ffplay_FrameQueue.h"
#include "ffplay_PacketQueue.h"

namespace ffplay
{
constexpr int IO_BUFFER_SIZE = 4096;
typedef struct AudioParams
{
    int freq;
    AVChannelLayout ch_layout;
    enum AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
} AudioParams;

enum
{
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

enum class EVideoState
{
    HAVE_NOTHING = 0,
    HAVE_METADATA = 1,
    HAVE_CURRENT_DATA = 2,
    HAVE_FUTURE_DATA = 3,
    HAVE_ENOUGH_DATA = 4,
};
typedef struct VideoState
{
    SDL_Thread *read_tid;
    const AVInputFormat *iformat;
    int abort_request;
    int force_refresh;
    int paused;
    int last_paused;
    int queue_attachments_req;
    int seek_req;
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
    int read_pause_return;
    AVFormatContext *ic;
    int realtime;

    Clock audclk;
    Clock vidclk;
    Clock extclk;

    FrameQueue pictq;
    FrameQueue subpq;
    FrameQueue sampq;

    Decoder auddec;
    Decoder viddec;
    Decoder subdec;

    int audio_stream;

    int av_sync_type = AV_SYNC_AUDIO_MASTER; // AV_SYNC_AUDIO_MASTER AV_SYNC_VIDEO_MASTER, AV_SYNC_EXTERNAL_CLOCK

    double audio_clock;
    int audio_clock_serial;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    AVStream *audio_st;
    PacketQueue audioq;
    int audio_hw_buf_size;
    uint8_t *audio_buf;
    uint8_t *audio_buf1;
    unsigned int audio_buf_size; /* in bytes */
    unsigned int audio_buf1_size;
    int audio_buf_index; /* in bytes */
    int audio_write_buf_size;
    int audio_volume;
    int muted;
    struct AudioParams audio_src;
    struct AudioParams audio_filter_src;
    struct AudioParams audio_tgt;
    struct SwrContext *swr_ctx;
    int frame_drops_early;
    int frame_drops_late;

    enum ShowMode
    {
        SHOW_MODE_NONE = -1,
        SHOW_MODE_VIDEO = 0,
        SHOW_MODE_WAVES,
        SHOW_MODE_RDFT,
        SHOW_MODE_NB
    } show_mode;
    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;
    AVTXContext *rdft;
    av_tx_fn rdft_fn;
    int rdft_bits;
    float *real_data;
    AVComplexFloat *rdft_data;
    int xpos;
    double last_vis_time;
    SDL_Texture *vis_texture;
    SDL_Texture *sub_texture;
    SDL_Texture *vid_texture;

    int subtitle_stream;
    AVStream *subtitle_st;
    PacketQueue subtitleq;

    double frame_timer;
    double frame_last_returned_time;
    double frame_last_filter_delay;
    int video_stream;
    AVStream *video_st;
    PacketQueue videoq;
    double
        max_frame_duration; // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    struct SwsContext *sub_convert_ctx;
    int eof;

    char *filename;
    int width, height, xleft, ytop;
    int step;

    int vfilter_idx;
    AVFilterContext *in_video_filter;  // the first filter in the video chain
    AVFilterContext *out_video_filter; // the last filter in the video chain
    AVFilterContext *in_audio_filter;  // the first filter in the audio chain
    AVFilterContext *out_audio_filter; // the last filter in the audio chain
    AVFilterGraph *agraph;             // audio filter graph

    int last_video_stream, last_audio_stream, last_subtitle_stream;

    SDL_cond *continue_read_thread;

    //
    AVDictionary *format_opts = nullptr;
    AVDictionary *codec_opts = nullptr;
    AVDictionary *swr_opts = nullptr;
    AVDictionary *sws_dict = nullptr;
    const char *input_filename = "";
    /**/
    static int startup_volume;
    static int decoder_reorder_pts;
    int audio_disable = 0;
    static int video_disable;
    static const char *window_title;
    static int64_t start_time;
    static int64_t duration;
    static int autoexit;
    // static int loop;
    static int infinite_buffer;
    static int subtitle_disable;
    static const char *wanted_stream_spec[AVMEDIA_TYPE_NB];
    static int seek_by_bytes;
    static int genpts;
    static int find_stream_info;
    static int lowres;
    static const char *audio_codec_name;
    static const char *subtitle_codec_name;
    static const char *video_codec_name;
    static int fast;
    static const char *hwaccel;
    static int filter_nbthreads;
    static char *afilters;
    static int show_status;
    static enum ShowMode s_show_mode;
    static int autorotate;
    static const char **vfilters_list;
    static int framedrop;
    static int display_disable;
    static double rdftspeed;
    /* current context */
    // int is_full_screen;
    int64_t audio_callback_time;
    SDL_AudioDeviceID audio_dev;
    SDL_RendererInfo renderer_info = {0};

    SwsContext *img_convert_ctx = nullptr;
    std::function<void(unsigned char *data, int width, int height, int bufferSize)> render_callback;

    EVideoState m_videoState = EVideoState::HAVE_NOTHING;
    std::function<void(const char *)> m_emitFunc;

    bool m_loop = false;
    bool m_autoplay = false;

    int videoWidth = 0;
    int videoHeight = 0;

    unsigned char *m_video_buffer = nullptr;
    int m_video_buffer_size = 0;

    // unsigned char *m_currentReadPtr = nullptr;
    // int m_leftSize = 0;
    unsigned char *m_iobuffer_ptr = nullptr;
    int m_position = 0;
} VideoState;

bool stream_open(VideoState *is, const char *filename, const AVInputFormat *iformat);
bool stream_open(VideoState *is, unsigned char *buffer, int length, const AVInputFormat *iformat);
void do_pause(VideoState *is);
void do_play(VideoState *is);
void do_exit(VideoState *is);
void stream_seek(VideoState *is, int64_t pos, int64_t rel, int by_bytes);
} // namespace ffplay
#endif
