#ifndef _BOX_HPP
#define _BOX_HPP

#include "IDrawable.hpp"
#include "IRenderer.hpp"
#include <string>


class Box : public IDrawable
{
    std::string name{""};
    Point p; //lewy górny róg

    static constexpr unsigned maxContentSize{ 8 };
    unsigned int Spaces;


    Box() = default;
    public:

        Box(std::string s, Point p) : name(s), p(p) {
            
        }
        
        void draw(IRenderer &r) override
        {
            if (maxContentSize - name.size() >= 0)
                Spaces = maxContentSize - name.size();
            else
                Spaces = 0;
                
            r.renderString("|" + name + std::string(Spaces, ' ') + "|", {p.x, p.y} );
        }

        Point leftUpperCorner() const
        {
            return {p.x, p.y};
        }

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#endif

        Point rightLowerCorner() const //rightCorner bo jedna linia
        {
            return { p.x + maxContentSize + 1, p.y};
        }

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

    friend class Class;
    friend class ClassBox;
};

#endif
