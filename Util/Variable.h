#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>

/*
 * This is simply a base class that will be used.
 * in some future plans as the semester progresses.
 */
class Variable {
public:

    virtual ~Variable() {

    }

    virtual std::string toString() = 0;

};

#endif //__VARIABLE_H__
