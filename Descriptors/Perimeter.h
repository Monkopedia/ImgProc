#ifndef __PERIMETER_H__
#define __PERIMETER_H__

#include "Descriptor.h"
#include <sstream>

namespace ImgProc {

#define MAX_VAL 65535

class Perimeter : public Descriptor {
public:
    Perimeter(int chan) : channel(chan) {

    }

    ~Perimeter() {

    }

    void process(Image* image, Variable* var) {
        float sum = 0;
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                float val = image->get(pos, channel);
                if (getValue(i, j, image) != val) {
                    sum += val / MAX_VAL;
                }
            }
        }
        //sum /= MAX_VAL;
        StringVar* strVar = dynamic_cast<StringVar*>(var);
        if (strVar != NULL) {
            std::stringstream sstr;
            sstr << sum;
            strVar->str = sstr.str();
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


protected:
    int channel;

};

}   

#endif //__PERIMETER_H__
