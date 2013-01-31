#ifndef __INVERT_CHANNEL_H__
#define __INVERT_CHANNEL_H__

#include "Filter.h"
#include <iostream>

namespace ImgProc {

#define MAX_VAL 65535

class InvertChannel : public Filter {
public:
    InvertChannel(int channel) {
        this->channel = channel;
    }

    ~InvertChannel() {

    }

    void process(Image* image, Image* ret) {
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                ret->set(pos, channel, MAX_VAL - image->get(pos, channel));
            }
        }
    }

private:
    int channel;

};

}   

#endif //__INVERT_CHANNEL_H__
