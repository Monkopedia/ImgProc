#ifndef __EIGENS_H__
#define __EIGENS_H__

#include "Descriptor.h"
#include <sstream>
#include <math.h>

namespace ImgProc {

#define MAX_VAL 65535

class Eigens : public Descriptor {
public:
    Eigens() {

    }

    ~Eigens() {

    }

    void process(Image* image, Variable* var) {
        MomentCalculator& mc = image->getMomentCalculator();
        float xbar = mc.getMoment(1, 0) / mc.getMoment(0, 0);
        float ybar = mc.getMoment(0, 1) / mc.getMoment(0, 0);
        float matrix[2][2] = {{0}};

        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                Vector2 pos(i, j);
                if (image->getGrey(pos) != 0) {
                    Vector2 diff(pos.x() - xbar, pos.y() - ybar);
                    matrix[0][0] += diff.getX() * diff.getX();
                    matrix[0][1] += diff.getX() * diff.getY();
                    matrix[1][1] += diff.getY() * diff.getY();
                }
            }
        }
        matrix[1][0] = matrix[0][1];
        float t = matrix[0][0] + matrix[1][1];
        float d = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

        // Eigenvalues
        float l1 = t / 2 + sqrt(t * t / 4 - d);
        float l2 = t / 2 - sqrt(t * t / 4 - d);
        
        float ev1[2] = {1, 0};
        float ev2[2] = {0, 1};

        if (matrix[1][0] != 0) {
            ev1[0] = l1 - matrix[1][1];
            ev1[1] = matrix[1][0];
            ev2[0] = l2 - matrix[1][1];
            ev2[1] = matrix[1][0];
        }


        StringVar* strVar = dynamic_cast<StringVar*>(var);
        if (strVar != NULL) {
            std::stringstream sstr;
            //sstr << "[" << matrix[0][0] << " " << matrix[0][1] << ";" << matrix[1][0] << " " << matrix[1][1] << "]" << std::endl;
            sstr << l1 << "," << l2 << std::endl;
            sstr << "|" << ev1[0] << "|" << ev2[0] << "|" << std::endl;
            sstr << "|" << ev1[1] << "|" << ev2[1] << "|" << std::endl;
            strVar->str = sstr.str();
        }
    }
protected:

};

}   

#endif //__EIGENS_H__
