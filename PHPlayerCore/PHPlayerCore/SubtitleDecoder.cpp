//
//  SubtitleDecoder.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "SubtitleDecoder.hpp"
#include "spdlog/spdlog.h"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
#include "SubtitleRenderer.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"
extern "C" {
#include "libavutil/pixdesc.h"
#include "VideoToolbox.h"
}

SubtitleDecoder::SubtitleDecoder(PHPlayerCore *player):Decoder(player)
{
    subtitleQueue = new SubtitleQueue(16);
}

SubtitleDecoder::~SubtitleDecoder()
{
    delete subtitleQueue;
}

SubtitleQueue *SubtitleDecoder::getSubtitleQueue()
{
    return subtitleQueue;
}

bool SubtitleDecoder::open()
{
    packetQueue = player->getDemuxer()->getSubtitlePacketQueue();
    AVStream *stream;
    stream = player->getDemuxer()->getSubtitleStream();
    if (stream == NULL) {
        return false;
    }
    
    AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec) {
        return false;
    }
    
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        return false;
    }
    
    int ret = 0;
    if ((ret = avcodec_parameters_to_context(codecContext, stream->codecpar)) < 0) {
        return false;
    }
    
    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        return false;
    }
    return true;
}

void SubtitleDecoder::start()
{
    std::thread decodeThread(&SubtitleDecoder::decode, this);
    decodeThread.detach();
}

void SubtitleDecoder::decode()
{
    AVPacket *packet = 0;
    while (true) {
        
        packet = packetQueue->front();
        if (packet == 0) {
            break;
        }
        
        if (strcmp((char*)packet->data, "flush") == 0) {
            avcodec_flush_buffers(codecContext);
            av_packet_free(&packet);
            spdlog::get("phplayer")->info("Flush codec buffers.");
            continue;
        }
        
        AVSubtitle *sub = (AVSubtitle*)av_malloc(sizeof(AVSubtitle));
        int got_sub;
        avcodec_decode_subtitle2(codecContext, sub, &got_sub, packet);
        if (got_sub) {
            subtitleQueue->push(sub);
        }
        av_packet_free(&packet);
        
    }
}

void SubtitleDecoder::clear()
{
    subtitleQueue->clear();
}

void SubtitleDecoder::close()
{
//    packetQueue->push(0);
    subtitleQueue->push(0);
}
