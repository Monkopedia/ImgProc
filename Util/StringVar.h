#ifndef __STRING_VAR_H__
#define __STRING_VAR_H__

#include "Variable.h"
#include <string>

namespace ImgProc {

class StringVar : public Variable {
public:

    StringVar(std::string str) {
        this->str = str;
    }

    StringVar() {
        this->str = "";
    }

    ~StringVar() {

    }

    virtual std::string toString() {
        return str;
    }
    std::string str;
};

}

#endif //__STRING_VAR_H__
