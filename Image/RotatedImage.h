#ifndef __ROTATED_IMAGE_H__
#define __ROTATED_IMAGE_H__

#include "Image.h"
#include "Rotation.h"

#include <sstream>

namespace ImgProc {

class RotatedImage : public Image {
public:
    RotatedImage(Rotation r, const char* file) : Image(file), rot(r) {
    }

    RotatedImage(Rotation r, const Image& image) : Image(image), rot(r) {
    }

    // IDK what I would use this for...
    // maybe wierdly shaped buffers...
    // strange cropping may occur...
    RotatedImage(Rotation r, int width, int height) : Image(width, height), rot(r) {
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

    virtual std::string toString() {
        std::stringstream stream("Rotated Image (");

        stream << width << "x" << height << " px)" << endl;
        return stream.str() + rot.toString();
    }

private:
    Rotation rot;
};

}

#endif //__ROTATED_IMAGE_H__
