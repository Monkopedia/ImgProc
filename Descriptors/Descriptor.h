#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "Image.h"
#include "Variable.h"
#include "StringVar.h"

namespace ImgProc {

class Descriptor : public Variable {
public:
    virtual ~Descriptor() {
    }

    virtual Variable* process(Image* image) {
        Variable* ret = new StringVar();
        process(image, ret);
        return ret;
    }
    virtual void process(Image* image, Variable* dest) = 0;

    virtual std::string toString() {
        return "Descriptor: Needs to be overloaded...";
    }
protected:

private:


};

}


#endif //__DESCRIPTOR_H__
