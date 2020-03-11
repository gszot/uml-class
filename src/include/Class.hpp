#ifndef _CLASS_HPP_
#define _CLASS_HPP_

#include <string>
#include <memory>
#include "IActor.hpp"
#include "ClassBox.hpp"
#include "Box.hpp"
#include "Line.hpp"
#include "Field.hpp"
#include "Utility.hpp"

class Class : public IActor, public IXmlAble
{
    std::string name;
    std::vector<std::shared_ptr<Field>> fields;
    ClassBox box;

    std::string fieldName;
    unsigned int pos;

    void boxUpdate()
    {
        if (name.size() > box.maxContentSize)box.nameBox.name = name.substr(0, box.maxContentSize);
        else 
            box.nameBox.name = name;
        box.fieldBoxes.clear();
        pos = 2;
        for (auto &it : fields)
        {
            pos++;
            if (it->name.size() > box.maxContentSize)
                fieldName = it->name.substr(0, box.maxContentSize);
            else fieldName = it->name;

            box.fieldBoxes.push_back(Box(fieldName, box.p + Point(0, pos)));
        }
    }

    public:

        Class() = default;

        Class(std::string name, Point pos = { 0,0 }) : name(name)
        {
            box.p = pos;
            box.nameBox.p = pos + Point(0,1);
            boxUpdate();
        }

        void setName(std::string nn)
        {
            name = nn;
            boxUpdate();
        }
        std::string getName() const { return name; }

        std::string dumpDataIntoXmlFormat() const override
        {
            std::string ret;

            ret += XML::makeTag("name", name);

            return ret;
        }

        void setPosition(Point p)
        {
            box.p = p;
            box.nameBox.p = p + Point(0, 1);
            boxUpdate();
        }

        Point getCenter() const override
        {
            auto blc = box.leftUpperCorner();
            auto brc = box.rightLowerCorner();

            return { blc.x + ((brc.x - blc.x) / 2), (blc.y - brc.y) / 2 + blc.y };
        }

        void addField(std::string fieldName)
        {
            //fields.push_back(Field(fieldName));
            fields.push_back(std::move(std::make_shared<Field>(fieldName)));
            boxUpdate();
        }

        void removeField(std::string fieldName)
        {
            fields.erase(std::remove_if(fields.begin(), fields.end(),
                [&fieldName](const std::shared_ptr<Field> &f)
            {
                return f->getName() == fieldName;
            }), fields.end());
            boxUpdate();
        }

        virtual void draw(IRenderer& r) override
        {
            if (selected)
            {
                r.setAttribute(A_REVERSE);
                box.draw(r);
                r.unsetAttribute(A_REVERSE);
            }
            else
            {
                fieldName = "";
                for (auto &f : fields)
                {
                    if (f->selected)
                    {
                        fieldName = f->getName();
                        break;
                    }
                }
                box.draw(r, fieldName);
            }
        }

        friend class Logic;
        friend class Inheritance;
};

#endif
