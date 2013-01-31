#ifndef __MASK_FILTER_H__
#define __MASK_FILTER_H__

#include "Filter.h"
#include <stdio.h>

namespace ImgProc {

#define MAX_VAL 65535

class MaskFilter : public Filter {
public:

    MaskFilter(float mask[3][3]) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                this->mask[i][j] = mask[i][j];
            }
        }
    }

    ~MaskFilter() {

    }

    void process(Image* image, Image* dest) {
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                Color c(0, 0, 0, 0);
//                int x = 1, y = 1;
                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        Vector2 loc(i + x - 1, j + y - 1);
                        Color l = (image->get(loc) * mask[x][y]);
                        c.addTo(l);
                 //       printf("Adding: %d %d %lf\n", loc.x(), loc.y(), mask[x][y]);
                    }
                }
               // printf("%lf %lf %lf %lf\n", c.r, c.g, c.b, c.a);
                dest->set(pos, c);
            }
        }
    }

private:
    float mask[3][3];

};

}   

#endif //__MASK_FILTER_H__
