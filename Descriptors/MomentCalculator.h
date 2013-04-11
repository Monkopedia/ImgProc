#ifndef __MOMENT_CALCULATOR_H__
#define __MOMENT_CALCULATOR_H__

#include <vector>

namespace ImgProc {

class Image;

class MomentCalculator {
public:
    MomentCalculator(Image* image) {
        this->image = image;
    }

    ~MomentCalculator() {

    }

    float getMoment(int p, int q);
    float getCentralMoment(int p, int q);
    float getNormalMoment(int p, int q);

protected:
    Image* image;

    std::vector<int> momentXs;
    std::vector<int> momentYs;
    std::vector<float> moments;

    std::vector<int> centralMomentXs;
    std::vector<int> centralMomentYs;
    std::vector<float> centralMoments;

    std::vector<int> normalMomentXs;
    std::vector<int> normalMomentYs;
    std::vector<float> normalMoments;
};

}   

#endif //__MOMENT_CALCULATOR_H__
