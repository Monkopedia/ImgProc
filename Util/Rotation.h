#ifndef __ROTATION_H__
#define __ROTATION_H__

#include "Variable.h"
#include "Vector2.h"
#include <math.h>

#include <sstream>

namespace ImgProc {

class Rotation : public Variable {
public:

    Rotation(float angle) {
        angle *= M_PI / 180.0;
        r[0][0] = cos(angle);
        r[0][1] = -sin(angle);
        r[1][0] = sin(angle);
        r[1][1] = cos(angle);
    }

    Rotation(float r[2][2]) {
        this->r[0][0] = r[0][0];
        this->r[0][1] = r[0][1];
        this->r[1][0] = r[1][0];
        this->r[1][1] = r[1][1];
    }

    Vector2 operator*(const Vector2& point) const {
        Vector2 ret(point.getX() * r[0][0] + point.getY() * r[0][1],
                    point.getX() * r[1][0] + point.getY() * r[1][1]);
        return ret;
    }

    Rotation operator*(const Rotation& other) const {
        float rot[2][2];

        rot[0][0] = r[0][0] * other.r[0][0] + r[0][1] * other.r[1][0];
        rot[0][1] = r[0][0] * other.r[0][1] + r[0][1] * other.r[1][1];
        rot[1][0] = r[1][0] * other.r[0][0] + r[1][1] * other.r[1][0];
        rot[1][1] = r[1][0] * other.r[0][1] + r[1][1] * other.r[1][1];
        Rotation ret(rot);
        return ret;
    }

    virtual std::string toString() {
        std::stringstream stream("Rotation: ");

        for (int i = 0; i < 2; i++) {
            stream << endl;
            for (int j = 0; j < 2; j++) {
                stream << "\t" << r[i][j];
            }
        }

        return stream.str();
    }

private:
    float r[2][2];

};

}

#endif //__ROTATION_H__
