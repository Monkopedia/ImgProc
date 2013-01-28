#ifndef __FILTER_H__
#define __FILTER_H__

namespace ImgProc {

class Filter {
public:
    virtual ~Filter() {
    }

    virtual Image* process(Image* image) = 0;

protected:

private:

};

}

#endif //__FILTER_H__
