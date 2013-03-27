#ifndef __ERODE_FILTER_H__
#define __ERODE_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

class ErodeFilter : public Filter {
public:

    ErodeFilter(int channel) {
        this->channel = channel;
    }

    ~ErodeFilter() {

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
        float tmp = MAX_VAL;

        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if ((x != 1) || (y != 1)) {
                    Vector2 loc(i + x - 1, j + y - 1);
                    if (image->get(loc, channel) < (MAX_VAL / 2)) {
                        tmp = 0;
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

#endif //__ERODE_FILTER_H__
