#ifndef __TRANSFORM_IMAGE_H__
#define  __TRANSFORM_IMAGE_H__

#include "Image.h"
#include "Transform.h"

#include <sstream>

namespace ImgProc {

class TransformImage : public Image {
public:
    TransformImage(Transform r, const char* file) : Image(file), rot(r) {
    }

    TransformImage(Transform r, const Image& image) : Image(image), rot(r) {
    }

    // IDK what I would use this for...
    // maybe wierdly shaped buffers...
    // strange cropping may occur...
    TransformImage(Transform r, int width, int height) : Image(width, height), rot(r) {
    }

    Transform getRot() const {
        return rot;
    }

    void setTransform(Transform rot) {
        this->rot = rot;
    }

    virtual int I(Vector2 p) const {
        Vector2 l = rot * p;
        return Image::I(l);
    }

    virtual std::string toString() {
        std::stringstream stream("Transform Image (");

        stream << width << "x" << height << " px)" << endl;
        return stream.str() + rot.toString();
    }
private:
    Transform rot;
};

}

#endif //__TRANSFORM_IMAGE_H__
