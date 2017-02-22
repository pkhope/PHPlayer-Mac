//
//  Demuxer.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Demuxer_hpp
#define Demuxer_hpp

#include <stdio.h>

struct AVStream;
class PacketQueue;
class PHPlayerCore;

class Demuxer {
public:
    Demuxer(PHPlayerCore *player);
    ~Demuxer();
    
    bool start();
    void demux();
    void seek(int64_t position);
    
    AVStream *getVideoStream();
    AVStream *getAudioStream();
    AVStream *getSubtitleStream();
    PacketQueue *getVideoPacketQueue();
    PacketQueue *getAudioPacketQueue();
    PacketQueue *getSubtitlePacketQueue();
    
private:
    PHPlayerCore *player;
    AVStream *videoStream;
    AVStream *audioStream;
    AVStream *subtitleStream;
    PacketQueue *videoPacketQueue;
    PacketQueue *audioPacketQueue;
    PacketQueue *subtitlePacketQueue;
    bool isRequestSeek;
    int64_t seekPosition;
};
#endif /* Demuxer_hpp */