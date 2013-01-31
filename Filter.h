#ifndef __FILTER_H__
#define __FILTER_H__

namespace ImgProc {

class Filter {
public:
    virtual ~Filter() {
    }

    Image* process(Image* image) {
        Image* ret = new Image(image->getWidth(), image->getHeight());
        process(image, ret);
        return ret;
    }
    virtual void process(Image* image, Image* dest) = 0;

protected:

private:

};

}

#endif //__FILTER_H__
