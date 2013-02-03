#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <sstream>

/**
 * Utility class to help handle image locations.
 * costructor accepts ints but all locations are stored
 * internally as floats for transformation reasons.
 */
class Vector2 {
public:
    Vector2(int x, int y) {
        this->myX = x;
        this->myY = y;
    }

    Vector2(float x, float y) {
        this->myX = (x);
        this->myY = (y);
    }

    Vector2() {
        myX = 0;
        myY = 0;
    }

    inline float getX() const {
        return myX;
    }

    inline float getY() const {
        return myY;
    }

    inline int x() const {
        return (int)(myX);
    }

    inline int y() const {
        return (int)(myY);
    }

    Vector2 operator+(Vector2 other) {
        Vector2 temp;
        temp.myX = myX + other.myX;
        temp.myY = myY + other.myY;
        return temp;
    }

    Vector2 operator-(Vector2 other) {
        Vector2 temp;
        temp.myX = myX - other.myX;
        temp.myY = myY - other.myY;
        return temp;
    }


    Vector2 &operator=(Vector2 other) {
        myX = other.myX;
        myY = other.myY;
        return *this;
    }

    virtual std::string toString() {
        std::stringstream stream("Vector2 (");

        stream << myX << "," << myY << ")";

        return stream.str();
    }
protected:

private:
    float myX;
    float myY;
};

#endif //__VECTOR2_H__
