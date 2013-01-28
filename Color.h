#ifndef __COLOR_H__
#define __COLOR_H__

#define MAX_A 1

namespace ImgProc {

class Color {
public:
    Color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = MAX_A;
    }

    Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color(const Color& base) {
        this->r = base.r;
        this->g = base.g;
        this->b = base.b;
        this->a = base.a;
    }

    Color() {
        a = r = g = b = MAX_A;
    }

    Color operator+(Color other) {
       Color ret;
       ret.r = alphaBlend(r, other.r, a, other.a);
       ret.g = alphaBlend(g, other.g, a, other.a);
       ret.b = alphaBlend(b, other.b, a, other.a);
       ret.a = other.a + a;
       if (ret.a > MAX_A) ret.a = MAX_A;
       return ret;
    }
    
    Color operator-(Color color) {
        Color ret;
        ret.r = r - color.r;
        ret.g = g - color.g;
        ret.b = b - color.b;
        ret.a = a - color.a;
        return ret;
    }

    Color operator=(Color base) {
        this->r = base.r;
        this->g = base.g;
        this->b = base.b;
        this->a = base.a;
        return *this;
    }

    float r;
    float b;
    float g;
    float a;

protected:
    inline float alphaBlend(float c1, float c2, float a1, float a2) {
        return (a1*c1 + a2*c2) / (a1 + a2);
    }

private:
};

}

#endif //__COLOR_H__
