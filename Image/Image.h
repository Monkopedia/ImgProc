#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Variable.h"
#include "Vector2.h"
#include <string>

#include <wand/MagickWand.h>

#include "Color.h"


namespace ImgProc {

typedef enum {
    GRAY = 1,
    RED = 2,
    GREEN = 3,
    BLUE = 4,
    ALPHA = 5,
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

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    void save(const char* file);

    Image operator+(Image& other);
    Image operator-(Image& other);
    Image &operator=(Image& other);
    Image operator+(Color& other);
    Image operator-(Color& other);
    Image &operator=(Color& other);

    virtual void freeMemory();
    virtual void setSize(int width, int height);
    virtual int I(Vector2 p) const;
protected:
    
    void loadFile(const char* file);
    void showError(MagickWand* wand);


    int width;
    int height;

    float* red;
    float* blue;
    float* green;
    float* alpha;
private:

};

}

#endif //__IMAGE_H__
