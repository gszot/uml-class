#ifndef _CLASSBOX_HPP
#define _CLASSBOX_HPP

#include "IDrawable.hpp"
#include "IRenderer.hpp"
#include "Box.hpp"
#include <string>


class ClassBox : public IDrawable
{
    Box nameBox;
    std::vector<Box> fieldBoxes;
    Point p; //lewy górny róg

    static constexpr unsigned maxContentSize{ 8 };
    unsigned int Spaces;


    ClassBox() = default;
    public:

        ClassBox(std::string s, Point p) : nameBox(s,p), p(p){
        }

        void draw(IRenderer& r) override
        {
            std::string Line = std::string(maxContentSize, '-');

            r.renderString("+" + Line + "+", p);
            nameBox.draw(r);
            if (fieldBoxes.size() > 0)
            {
                r.renderString("*" + Line + "*", { p.x, p.y + 2 });
                for (Box b : fieldBoxes)
                {
                    b.draw(r);
                }
                r.renderString("+" + Line + "+", { p.x, p.y + 2 + fieldBoxes.size() + 1 });
            }
            else r.renderString("+" + Line + "+", { p.x, p.y + 2 });
        }

        void draw(IRenderer& r, std::string selFieldName)
        {
            std::string Line = std::string(maxContentSize, '-');

            r.renderString("+" + Line + "+", p);
            nameBox.draw(r);
            if (fieldBoxes.size() > 0)
            {
                r.renderString("*" + Line + "*", { p.x, p.y + 2 });
                for (Box b : fieldBoxes)
                {
                    if (b.name == selFieldName)
                    {
                        r.setAttribute(A_REVERSE);
                        b.draw(r);
                        r.unsetAttribute(A_REVERSE);
                    }
                    else
                        b.draw(r);
                }
                r.renderString("+" + Line + "+", { p.x, p.y + 2 + fieldBoxes.size() + 1 });
            }
            else r.renderString("+" + Line + "+", { p.x, p.y + 2 });
        }
        
        Point leftUpperCorner() const
        {
            return { p.x, p.y };
        }

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#endif

        Point rightLowerCorner() const
        {
            if (fieldBoxes.size() > 0)
                return { p.x + 1 + maxContentSize, p.y + 2 + fieldBoxes.size() + 1 };
            else
                return { p.x + 1 + maxContentSize, p.y + 2 };
        }

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

    friend class Class;
};

#endif
