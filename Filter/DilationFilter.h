#ifndef __DILATION_FILTER_H__
#define __DILATION_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

class DilationFilter : public Filter {
public:

    DilationFilter(int channel) {
        this->channel = channel;
    }

    ~DilationFilter() {

    }

    void process(Image* image, Image* dest) {
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                float c = getValue(i, j, image);
                Vector2 pos(i, j);
                dest->set(pos, channel, c);
            }
        }
    }

    float getValue(int i, int j, Image* image) {
        float tmp = 0;

        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if ((x != 1) || (y != 1)) {
                    Vector2 loc(i + x - 1, j + y - 1);
                    if (image->get(loc, channel) >= (MAX_VAL / 2)) {
                        tmp = MAX_VAL;
                    }
                }
            }
        }
        return tmp;
    }

private:
    int channel;

};

}   

#endif //__DILATION_FILTER_H__
