#ifndef __COLOR_H__
#define __COLOR_H__

#define MAX_A 65535
#define MAX_VAL 65535

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
        if (this->a == 0) {
            this->a = MAX_A;
        }
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

    void addTo(const Color& other) {
        r += other.r;
        if (r > MAX_VAL) r = MAX_VAL;
        g += other.g;
        if (g > MAX_VAL) g = MAX_VAL;
        b += other.b;
        if (b > MAX_VAL) b = MAX_VAL;
        a += other.a;
        if (a > MAX_A) a = MAX_A;
    }

    Color operator+(const Color& other) {
       Color ret;
       ret.r = alphaBlend(r, other.r, a, other.a);
       ret.g = alphaBlend(g, other.g, a, other.a);
       ret.b = alphaBlend(b, other.b, a, other.a);
       ret.a = other.a + a;
       if (ret.a > MAX_A) ret.a = MAX_A;
       return ret;
    }

    Color operator*(float amount) {
        Color ret;
        ret.r = amount * r;
        ret.g = amount * g;
        ret.b = amount * b;
        ret.a = a;
        return ret;
    }
    
    Color operator-(const Color& color) {
        Color ret;
        ret.r = r - color.r;
        ret.g = g - color.g;
        ret.b = b - color.b;
        ret.a = a - color.a;
        return ret;
    }

    Color operator=(const Color& base) {
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

    inline float alphaBlend(float c1, float c2, float a1, float a2) {
        return (a1*c1 + a2*c2) / (a1 + a2);
    }
protected:

private:
};

}

#endif //__COLOR_H__
