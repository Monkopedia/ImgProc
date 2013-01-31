#ifndef __COLOR_IMAGE_H__
#define __COLOR_IMAGE_H__

#include "Image.h"

namespace ImgProc {

/**
 * This is just like a normal image except it stores all data
 * in Colors rather than doubles.
 */
class ColorImage : public Image {
public:
    ColorImage(const char* file);
    ColorImage(const ImgProc::Image& base);
    ColorImage(int width, int height);

    ~ColorImage();

    float getR(Vector2 position) const;
    float getG(Vector2 position) const;
    float getB(Vector2 position) const;
    float getA(Vector2 position) const;
    Color get(Vector2 position) const;

    void setR(Vector2 position, float value);
    void setG(Vector2 position, float value);
    void setB(Vector2 position, float value);
    void setA(Vector2 position, float value);
    void set(Vector2 position, Color value);

    virtual void setSize(int width, int height);
    void freeMemory();
protected:

private:
    Color* image;



};

}

#endif //__COLOR_IMAGE_H__
