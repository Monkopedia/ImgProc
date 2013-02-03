#ifndef __MEDIAN_FILTER_H__
#define __MEDIAN_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

class MedianFilter : public Filter {
public:

    MedianFilter(int channel) {
        this->channel = channel;
    }

    ~MedianFilter() {

    }

    void process(Image* image, Image* dest) {
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                float c = getMedian(i, j, image);
                Vector2 pos(i, j);
                dest->set(pos, channel, c);
            }
        }
    }

    float getMedian(int i, int j, Image* image) {
        float list[9];
        int index = -1;
        float tmp;

        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                Vector2 loc(i + x - 1, j + y - 1);
                list[++index] = image->get(loc, channel);
            }
        }
        for (int i = 0; i < 9; i++) {
            for (int j = i + 1; j < 9; j++) {
                if (list[j] < list[i]) {
                    tmp = list[j];
                    list[j] = list[i];
                    list[i] = tmp;
                }
            }
        }
        return list[4];
    }

private:
    int channel;

};

}   

#endif //__MEDIAN_FILTER_H__
