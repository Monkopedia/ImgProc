#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Variable.h"
#include "Vector2.h"
#include <string>

#include <wand/MagickWand.h>

#include "Color.h"

namespace ImgProc {

typedef enum {
    GRAY,
    RED,
    GREEN,
    BLUE,
    ALPHA,
} Channel;

/**
 * This will hold the data for all basic images.
 *
 * Extends Variable for the future.
 */
class Image : public Variable {
public:
    Image(const char* file);
    Image(const ImgProc::Image& base);
    Image(int width, int height);

    virtual ~Image();

    virtual float getGrey(Vector2 position) const;
    virtual float getR(Vector2 position) const;
    virtual float getG(Vector2 position) const;
    virtual float getB(Vector2 position) const;
    virtual float getA(Vector2 position) const;
    virtual float get(Vector2 position, int channel) const;
    virtual Color get(Vector2 position) const;

    virtual void setGrey(Vector2 position, float value);
    virtual void setR(Vector2 position, float value);
    virtual void setG(Vector2 position, float value);
    virtual void setB(Vector2 position, float value);
    virtual void setA(Vector2 position, float value);
    virtual void set(Vector2 position, int channel, float value);
    virtual void set(Vector2 position, Color value);

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    void save(const char* file);

    Image operator+(Image other);
    Image operator-(Image other);
    Image &operator=(Image other);
    Image operator+(Color other);
    Image operator-(Color other);
    Image &operator=(Color other);

protected:
    void showError(MagickWand* wand);

    void freeMemory();
    void setSize(int width, int height);

private:
    int width;
    int height;
    float* red;
    float* blue;
    float* green;
    float* alpha;

};

}

#endif //__IMAGE_H__
