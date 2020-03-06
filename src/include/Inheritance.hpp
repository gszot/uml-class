#ifndef _INHERITANCE_HPP_
#define _INHERITANCE_HPP_

#include "IActor.hpp"

class Inheritance : public IActor, public IXmlAble
{
    public:

    Inheritance() = default;
};

#endif