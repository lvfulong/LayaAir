#ifndef __ffplay_PacketQueue _H__
#define __ffplay_PacketQueue _H__

#include "libavcodec/packet.h"
#include "libavutil/fifo.h"
#include <SDL2/SDL_thread.h>
namespace ffplay
{
typedef struct FrameData
{
    int64_t pkt_pos;
} FrameData;
typedef struct MyAVPacketList
{
    AVPacket *pkt;
    int serial;
} MyAVPacketList;
typedef struct PacketQueue
{
    AVFifo *pkt_list;
    int nb_packets;
    int size;
    int64_t duration;
    int abort_request;
    int serial;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);

int packet_queue_put(PacketQueue *q, AVPacket *pkt);

int packet_queue_put_nullpacket(PacketQueue *q, AVPacket *pkt, int stream_index);
/* packet queue handling */
int packet_queue_init(PacketQueue *q);

void packet_queue_flush(PacketQueue *q);

void packet_queue_destroy(PacketQueue *q);

void packet_queue_abort(PacketQueue *q);

void packet_queue_start(PacketQueue *q);

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial);

} // namespace ffplay
#endif