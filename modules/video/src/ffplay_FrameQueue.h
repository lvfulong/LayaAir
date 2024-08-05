#ifndef __ffplay_FrameQueue_H__
#define __ffplay_FrameQueue_H__
#include "ffplay_Config.h"
#include "ffplay_PacketQueue.h"
#include "libavutil/frame.h"
#include <libavcodec/avcodec.h>
namespace ffplay
{
/* Common struct for handling all types of decoded data and allocated render buffers. */
typedef struct Frame
{
    AVFrame *frame;
    AVSubtitle sub;
    int serial;
    double pts;      /* presentation timestamp for the frame */
    double duration; /* estimated duration of the frame */
    int64_t pos;     /* byte position of the frame in the input file */
    int width;
    int height;
    int format;
    AVRational sar;
    int uploaded;
    int flip_v;
} Frame;
typedef struct FrameQueue
{
    Frame queue[FRAME_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    SDL_mutex *mutex;
    SDL_cond *cond;
    PacketQueue *pktq;
} FrameQueue;

int frame_queue_init(FrameQueue *f, PacketQueue *pktq, int max_size, int keep_last);

void frame_queue_destroy(FrameQueue *f);

void frame_queue_signal(FrameQueue *f);

Frame *frame_queue_peek(FrameQueue *f);
Frame *frame_queue_peek_next(FrameQueue *f);

Frame *frame_queue_peek_last(FrameQueue *f);

Frame *frame_queue_peek_writable(FrameQueue *f);

Frame *frame_queue_peek_readable(FrameQueue *f);

void frame_queue_push(FrameQueue *f);

void frame_queue_next(FrameQueue *f);
int frame_queue_nb_remaining(FrameQueue *f);

int64_t frame_queue_last_pos(FrameQueue *f);

} // namespace ffplay
#endif