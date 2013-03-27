#ifndef __LAPLACIAN_FILTER_H__
#define __LAPLACIAN_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

#define abs(x) (x > 0 ? x : -x)

class LaplacianFilter : public Filter {
public:

    LaplacianFilter(int channel) {
        this->channel = channel;
    }

    ~LaplacianFilter() {

    }

    void process(Image* image, Image* dest) {
        float laplaceMask[3][3] = {
            { 0, 1, 0},
            { 1, -4, 1},
            { 0, 1, 0},
        };
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                float lVal = 0;
                
//                int x = 1, y = 1;
                for (int x = 0; x < 3; ++x) {
                    for (int y = 0; y < 3; ++y) {
                        Vector2 loc(i + x - 1, j + y - 1);
                        lVal += (image->get(loc, channel) * laplaceMask[x][y]);
                 //       printf("Adding: %d %d %lf\n", loc.x(), loc.y(), mask[x][y]);
                    }
                }
               // printf("%lf %lf %lf %lf\n", c.r, c.g, c.b, c.a);
                dest->set(pos, channel, 4 * abs(lVal));
            }
        }
    }

private:
    int channel;

};

}   

#endif //__LAPLACIAN_FILTER_H__
