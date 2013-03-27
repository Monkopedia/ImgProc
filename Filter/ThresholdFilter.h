#ifndef __THRESHOLD_FILTER_H__
#define __THRESHOLD_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

class ThresholdFilter : public Filter {
public:

    ThresholdFilter(int channel, float threshold) {
        this->channel = channel;
        this->threshold = threshold;
    }

    ~ThresholdFilter() {

    }

    void process(Image* image, Image* dest) {
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                float c = image->get(pos, channel);
                dest->set(pos, channel, c >= threshold ? MAX_VAL : 0);
            }
        }
    }

private:
    int channel;
    float threshold;

};

}   

#endif //__THRESHOLD_FILTER_H__
