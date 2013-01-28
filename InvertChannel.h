#ifndef __INVERT_H__
#define __INVERT_H__

#include "Filter.h"

namespace ImgProc {

#define MAX_VAL 65535

class Invert : public Filter {
public:
    ~Invert() {

    }

    Image* process(Image* image) {
        Image* ret = new Image(image->getWidth(), image->getHeight());
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                ret->setGrey(pos, MAX_VAL - image->getGrey(pos));
//                ret->setR(pos, MAX_VAL - image->getR(pos));
//                ret->setG(pos, MAX_VAL - image->getG(pos));
//                ret->setB(pos, MAX_VAL - image->getB(pos));
            }
        }
        return ret;
    }

};

}   

#endif //__INVERT_H__
