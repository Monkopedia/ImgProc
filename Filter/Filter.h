#ifndef __FILTER_H__
#define __FILTER_H__

#include "Image.h"
#include "Variable.h"

namespace ImgProc {

class Filter : public Variable {
public:
    virtual ~Filter() {
    }

    Image* process(Image* image) {
        Image* ret = new Image(image->getWidth(), image->getHeight());
        process(image, ret);
        return ret;
    }
    virtual void process(Image* image, Image* dest) = 0;

    virtual std::string toString() {
        return "Filter: Needs to be overloaded...";
    }
protected:

private:

};

}

#endif //__FILTER_H__
