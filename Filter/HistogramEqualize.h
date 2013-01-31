#ifndef __HISTOGRAM_EQUALIZE_H__
#define __HISTOGRAM_EQUALIZE_H__

#include "Filter.h"
#include <iostream>

namespace ImgProc {

#define MAX_VAL 65535

class HistogramEqualizer : public Filter {
public:
    HistogramEqualizer(int channel) {
        this->channel = channel;
    }

    ~HistogramEqualizer() {

    }

    void process(Image* image, Image* ret) {
        float* counts = new float[MAX_VAL + 1];
        float scale = ((float)MAX_VAL) / (image->getHeight() * image->getWidth());
        for (int i = 0; i <= MAX_VAL; i++) {
            counts[i] = 0;
        }
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                int ind = (int)(image->get(pos, channel));
                ++counts[ind];
            }
        }
        for (int i = 1; i <= MAX_VAL; i++) {
            counts[i] = (counts[i] + counts[i - 1]);
        }
        for (int i = 0; i <= MAX_VAL; i++) {
            counts[i] *= scale;
        }
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                int ind = (int)(image->get(pos, channel));
                ret->set(pos, channel, counts[ind]);
            }
        }
        delete counts;
    }

private:
    int channel;

};

}   

#endif //__HISTOGRAM_EQUALIZE_H__
