#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <string>
#include "IActor.hpp"

class Field : public IActor, public IXmlAble
{
    std::string name{ "" };

    static constexpr unsigned maxNameSize{ 8 };

    public:

        Field() = default;

        Field(std::string name) : name(name) {}

        void setName(std::string nn)
        {
            name = nn;
        }
        std::string getName() const { return name; }

        Point getCenter() const override
        {

        }

        std::string dumpDataIntoXmlFormat() const override { return ""; }

        void draw(IRenderer& r) override {}

        friend class Class;
};

#endif
