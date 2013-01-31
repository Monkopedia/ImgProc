#ifndef __INVERT_H__
#define __INVERT_H__

#include "Filter.h"

namespace ImgProc {

#define MAX_VAL 65535

class Invert : public Filter {
public:
    ~Invert() {

    }

    void process(Image* image, Image* dest) {
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                dest->setGrey(pos, MAX_VAL - image->getGrey(pos));
//                ret->setR(pos, MAX_VAL - image->getR(pos));
//                ret->setG(pos, MAX_VAL - image->getG(pos));
//                ret->setB(pos, MAX_VAL - image->getB(pos));
            }
        }
    }

};

}   

#endif //__INVERT_H__
