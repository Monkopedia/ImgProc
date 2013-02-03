#ifndef __Transform_H__
#define __Transform_H__

#include "Variable.h"
#include "Vector2.h"
#include <math.h>

namespace ImgProc {

class Transform : public Variable {
public:

    Transform(float angle, float x, float y) {
        angle *= M_PI / 180.0;
        r[0][0] = cos(angle);
        r[0][1] = -sin(angle);
        r[1][0] = sin(angle);
        r[1][1] = cos(angle);
        r[0][2] = x;
        r[1][2] = y;
        r[2][0] = 0;
        r[2][1] = 0;
        r[2][2] = 1;
    }

    Transform(float m[3][3]) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                this->r[i][j] = m[i][j];
            }
        }
    }

    Vector2 operator*(const Vector2& point) const {
        Vector2 ret(point.getX() * r[0][0] + point.getY() * r[0][1] + r[0][2],
                    point.getX() * r[1][0] + point.getY() * r[1][1] + r[1][2]);
        return ret;
    }

    Transform operator*(const Transform& other) const {
        float rot[3][3];

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                rot[i][j] = 0;
                for (int k = 0; k < 3; k++) {
                    rot[i][j] += r[i][k] * other.r[k][j];
                }
            }
        }

        Transform ret(rot);

        return ret;
    }

    virtual std::string toString() {
        std::stringstream stream("Transform: ");

        for (int i = 0; i < 3; i++) {
            stream << endl;
            for (int j = 0; j < 3; j++) {
                stream << "\t" << r[i][j];
            }
        }

        return stream.str();
    }

private:
    float r[3][3];

};

}

#endif //__Transform_H__
