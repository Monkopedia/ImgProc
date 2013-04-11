#ifndef __MOMENT_INVARIANT_H__
#define __MOMENT_INVARIANT_H__

#include "Descriptor.h"
#include <sstream>

namespace ImgProc {

#define MAX_VAL 65535

class MomentInvariant : public Descriptor {
public:
    MomentInvariant(int inv) : invariant(inv) {

    }

    ~MomentInvariant() {

    }

    void process(Image* image, Variable* var) {
        float inv = 0;
        MomentCalculator& mc = image->getMomentCalculator();
        float n20;
        float n02;
        float n11;
        float n30;
        float n12;
        float n21;
        float n03;

        switch (invariant) {
        case 1:
            inv = mc.getNormalMoment(2, 0) + mc.getNormalMoment(0, 2);
            break;
        case 2:
            n20 = mc.getNormalMoment(2, 0);
            n02 = mc.getNormalMoment(0, 2);
            n11 = mc.getNormalMoment(1, 1);
            inv = (n20 - n02) * (n20 - n02) + 4 * n11 * n11;
            break;
        case 3:
            n30 = mc.getNormalMoment(3, 0);
            n12 = mc.getNormalMoment(1, 2);
            n21 = mc.getNormalMoment(2, 1);
            n03 = mc.getNormalMoment(0, 3);
            inv = (n30 - 3*n12) * (n30 - 3*n12) + (3*n21 - n03) * (3*n21 - n03);
            break;
        case 4:
            n30 = mc.getNormalMoment(3, 0);
            n12 = mc.getNormalMoment(1, 2);
            n21 = mc.getNormalMoment(2, 1);
            n03 = mc.getNormalMoment(0, 3);
            inv = (n30 + n12) * (n30 + n12) + (n21 + n03) * (n21 + n03);
            break;
        case 5:
            n30 = mc.getNormalMoment(3, 0);
            n12 = mc.getNormalMoment(1, 2);
            n21 = mc.getNormalMoment(2, 1);
            n03 = mc.getNormalMoment(0, 3);
            inv = (n30 - 3*n12) * (n30 + n12) * ((n30 + n12)*(n30 + n12) - 3 * (n21 + n03) * (n21 + n03)) + 
                  (3*n21 - n03) * (n21 + n03) * (3*(n30 + n12) * (n30 + n12) - (n21 + n03) * (n21 + n03));
            break;
        case 6:
            n20 = mc.getNormalMoment(2, 0);
            n02 = mc.getNormalMoment(0, 2);
            n11 = mc.getNormalMoment(1, 1);
            n30 = mc.getNormalMoment(3, 0);
            n12 = mc.getNormalMoment(1, 2);
            n21 = mc.getNormalMoment(2, 1);
            n03 = mc.getNormalMoment(0, 3);
            inv = (n20 - n02) * ((n30 + n12) * (n30 + n12) - (n21 + n03) * (n21 + n03)) + 
                    4 * n11 * (n30 + n12) * (n21 + n03);
            break;
        case 7:
            n20 = mc.getNormalMoment(2, 0);
            n02 = mc.getNormalMoment(0, 2);
            n11 = mc.getNormalMoment(1, 1);
            n30 = mc.getNormalMoment(3, 0);
            n12 = mc.getNormalMoment(1, 2);
            n21 = mc.getNormalMoment(2, 1);
            n03 = mc.getNormalMoment(0, 3);
            inv = (3 * n21 - n03) * (n30 + n12) * ((n30 + n12) * (n30 + n12) - 3 * (n21 + n03) * (n21 + n03)) - 
                  (n30 - 3 * n12) * (n21 + n03) * (3 * (n30 + n12) * (n30 + n12) - (n21 + n03) * (n21 + n03));
            break;
        }

        //inv /= MAX_VAL;
        StringVar* strVar = dynamic_cast<StringVar*>(var);
        if (strVar != NULL) {
            std::stringstream sstr;
            sstr << inv;
            strVar->str = sstr.str();
        }
    }
protected:
    int invariant;

};

}   

#endif //__MOMENT_INVARIANT_H__
