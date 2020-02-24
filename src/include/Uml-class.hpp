#ifndef _UML_CLASS_HPP_
#define _UML_CLASS_HPP_

#include <map>
#include "ToolBase.hpp"
#include "BackendBase.hpp"
#include "Logic.hpp"

namespace tools
{

class UmlClass : public ToolBase
{

std::map<std::string, std::string> entries;
Logic logix;
Renderer rend;

public:
    UmlClass()
    {
        setEntry("IS_SAVED", "NO");
        logix.setRenderer(&rend);
        rend.resetCenter();
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        return entries[key];
    }
    void setCoordinates(int width, int height, int startx, int starty) noexcept override
    {
        rend.setCoordinates(width, height, startx, starty);
    }
    void setBackend(backends::BackendBase &bk) override
    {
        ToolBase::setBackend(bk);

        setEntry("IS_SAVED", "YES");

        bk.bind(":c ${ARG}", [this](void){
            logix.addClass(getEntry("ARG"));
            setEntry("IS_SAVED", "NO");
        }, "Creates a new class with specified name");
       
    }

};

}

#endif
