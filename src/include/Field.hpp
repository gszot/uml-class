#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <string>
#include "IActor.hpp"

class Field : public IActor, public IXmlAble
{
    std::string name;

    static constexpr unsigned maxNameSize{ 8 };
};

#endif
