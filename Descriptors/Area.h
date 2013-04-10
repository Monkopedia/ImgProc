#ifndef __AREA_H__
#define __AREA_H__

#include "Descriptor.h"
#include <sstream>

namespace ImgProc {

#define MAX_VAL 65535

class Area : public Descriptor {
public:
    Area(int chan) : channel(chan) {

    }

    ~Area() {

    }

    void process(Image* image, Variable* var) {
        float sum = 0;
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                sum += image->get(pos, channel) / MAX_VAL;
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
protected:
    int channel;

};

}   

#endif //__AREA_H__
