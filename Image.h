#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Variable.h"
#include "Vector2.h"
#include <string>

namespace ImgProc {

/**
 * This will hold the data for all basic images.
 *
 * Extends Variable for the future.
 */
class Image : public Variable {
public:
    Image(const char* file);

    virtual ~Image();

    float getGrey(Vector2 position);
    float getR(Vector2 position);
    float getG(Vector2 position);
    float getB(Vector2 position);

    void setGrey(Vector2 position, float value);
    void setR(Vector2 position, float value);
    void setG(Vector2 position, float value);
    void setB(Vector2 position, float value);

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    void save(const char* file);

protected:
    void freeMemory();
    void setSize(int width, int height);

private:
    int width;
    int height;
    float* red;
    float* blue;
    float* green;

};

}

#endif //__IMAGE_H__
