#ifndef __ROTATED_IMAGE_H__
#define __ROTATED_IMAGE_H__

#include "Image.h"
#include "Rotation.h"

namespace ImgProc {

class RotatedImage {
public:
    RotatedImage(Rotation rot, const char* file) : Image(file) {
        this->rot = rot;
    }

    RotatedImage(Rotation rot, const Image& image) : Image(image) {
        this->rot = rot;
    }

    // IDK what I would use this for...
    // maybe wierdly shaped buffers...
    // strange cropping may occur...
    RotatedImage(Rotation rot, int width, int height) : Image(width, height) {
        this->rot = rot;
    }

    Rotation getRot() const {
        return rot;
    }

    void setRotation(Rotation rot) {
        this->rot = rot;
    }

    virtual int I(Vector2 p) const {
        Vector2 l = rot * p;
        return Image::I(l);
    }

private:
    Rotation rot;
}

}

#endif //__ROTATED_IMAGE_H__
