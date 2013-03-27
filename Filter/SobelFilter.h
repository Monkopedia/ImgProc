#ifndef __SOBEL_FILTER_H__
#define __SOBEL_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

#define abs(x) (x > 0 ? x : -x)

class SobelFilter : public Filter {
public:

    SobelFilter(int channel) {
        this->channel = channel;
    }

    ~SobelFilter() {

    }

    void process(Image* image, Image* dest) {
        float xMask[3][3] = {
            { -1, -2, -1},
            { 0, 0, 0},
            { 1, 2, 1},
        };
        float yMask[3][3] = {
            { -1, 0, 1},
            { -2, 0, 2},
            { -1, 0, 1},
        };
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                float xVal = 0;
                float yVal = 0;
                
//                int x = 1, y = 1;
                for (int x = 0; x < 3; ++x) {
                    for (int y = 0; y < 3; ++y) {
                        Vector2 loc(i + x - 1, j + y - 1);
                        xVal += (image->get(loc, channel) * xMask[x][y]);
                        yVal += (image->get(loc, channel) * yMask[x][y]);
                 //       printf("Adding: %d %d %lf\n", loc.x(), loc.y(), mask[x][y]);
                    }
                }
               // printf("%lf %lf %lf %lf\n", c.r, c.g, c.b, c.a);
                dest->set(pos, channel, abs(xVal) + abs(yVal));
            }
        }
    }

private:
    int channel;

};

}   

#endif //__SOBEL_FILTER_H__
