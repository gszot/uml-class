#ifndef _INHERITANCE_HPP_
#define _INHERITANCE_HPP_

#include "IActor.hpp"

class Inheritance : public IActor, public IXmlAble
{
    public:

    std::weak_ptr<Class> start, end;
    std::unique_ptr<Line> lineVstart, lineH, lineVend;
    Point s, e;
    unsigned int level{ 10 };

    Inheritance() = default;

    virtual void draw(IRenderer& r) override
    {
        lineVstart->draw(r);
        lineH->draw(r);
        lineVend->draw(r);
    }
    virtual std::string dumpDataIntoXmlFormat() const override
    {

    }
    Point getCenter() const override
    {

    }


    void refitLines()
    {
        auto start_strong = start.lock();
        auto end_strong = end.lock();
        s = start_strong->box.rightLowerCorner() + Point(0, 1);
        e = end_strong->box.rightLowerCorner() + Point(0, 1);
        lineVstart = std::make_unique<Line>(s, Point(s.x, level));
        lineH = std::make_unique<Line>(Point(s.x, level), Point(e.x, level));
        lineVend = std::make_unique<Line>(e, Point(e.x, level));
    }
};

#endif
