#ifndef _CLASS_HPP_
#define _CLASS_HPP_

#include <string>
#include "IActor.hpp"
#include "Box.hpp"
#include "Line.hpp"
#include "Field.hpp"

class Class : public IActor, public IXmlAble
{
    std::string name;
    Box box;
    Line line;

    static constexpr unsigned maxNameSize{ 8 };

    std::vector<std::weak_ptr<Field>> fields;

    void boxUpdateName()
    {
        if (name.size() > maxNameSize)box.content = name.substr(0, maxNameSize - 3) + "...";
        else box.content = name;
    }

    public:

        int signalEndPositionVertical{ 0 };
        
        Class() = default;

        Class(std::string name, Point pos = { 0,0 }) : name(name)
        {
            box.p = pos;
            boxUpdateName();
        }

        void setName(std::string nn)
        {
            name = nn;
            boxUpdateName();
        }
        std::string getName() const { return name; }

        //std::string dumpDataIntoXmlFormat()

        //void setPosition(Point p)

        Point getCenter() const override
        {
            auto blc = box.leftUpperCorner();
            auto brc = box.rightLowerCorner();

            return { blc.x + ((brc.x - blc.x) / 2), (blc.y - brc.y) / 2 + blc.y };
        }

        //void pipeSignalFrom(std::weak_ptr<Signal> sig)
        //void pipeSignalTo(std::weak_ptr<Signal> sig)

        //void cullDeadSignals(int mode)

        //void findVerticalEnd()

        virtual void draw(IRenderer& r) override
        {
            if (selected)r.setAttribute(A_REVERSE);

            box.draw(r);

            auto blc = box.leftUpperCorner();
            auto brc = box.rightLowerCorner();

            auto lineX = blc.x + ((brc.x - blc.x) / 2);

            line.start = { lineX, brc.y + 1 };
            line.end = { lineX, std::max(brc.y + 1, signalEndPositionVertical + 1) };

            line.draw(r);

            if (selected)r.unsetAttribute(A_REVERSE);
        }
};

#endif
