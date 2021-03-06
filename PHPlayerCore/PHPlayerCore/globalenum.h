//
//  globalenum.h
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef globalenum_h
#define globalenum_h

enum PlayerState{
    PH_STATE_NONE,
    PH_STATE_RUNNING,
    PH_STATE_PAUSED,
    PH_STATE_STOPED
};

enum DecoderType{
    PH_DECODER_NONE,
    PH_DECODER_VIDEO,
    PH_DECODER_AUDIO,
    PH_DECODER_SUBTITLE
};
#endif /* globalenum_h */
