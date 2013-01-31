#ifndef __ROTATION_H__
#define __ROTATION_H__

#include "Variable.h"
#include "Vector2.h"
#include <math.h>

namespace ImgProc {

class Rotation : public Variable {
public:

    Rotation(float angle) {
        angle *= PI / 180.0;
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

    Vector2 operator*(const Vector2& point) {
        Vector2 ret(point.x * r[0][0] + point.y * r[0][1],
                    point.x * r[1][0] + point.y * r[1][1]);
        return ret;
    }

    Rotation operator*(const Rotation& other) {
        float rot[2][2];

        rot[0][0] = r[0][0] * other.r[0][0] + r[0][1] * other.r[1][0];
        rot[0][1] = r[0][0] * other.r[0][1] + r[0][1] * other.r[1][1];
        rot[1][0] = r[1][0] * other.r[0][0] + r[1][1] * other.r[1][0];
        rot[1][1] = r[1][0] * other.r[0][1] + r[1][1] * other.r[1][1];
        Rotation ret(rot);
        return ret;
    }

private:
    float r[2][2];

}

}

#endif //__ROTATION_H__
