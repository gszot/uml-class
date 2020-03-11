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

        bk.bind(":rmc ${ARG}", [this](void) {
            logix.rmClass(getEntry("ARG"));
            setEntry("IS_SAVED", "NO");
            }, "Removes a class with specified name");

        bk.bind(":f ${ARG} ${AARG}", [this](void) {
            logix.addFieldToClass(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
        }, "Creates a new field in class with name");

        bk.bind(":rmf ${ARG} ${AARG}", [this](void) {
            logix.rmFieldFromClass(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
        }, "Removes a field in class with name");

        bk.bind(":i ${ARG} ${AARG}", [this](void) {
            logix.addInheritance(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
            }, "Adds inheritance between classes");

        bk.bind(":rmi ${ARG} ${AARG}", [this](void) {
            logix.rmInheritance(getEntry("ARG"), getEntry("AARG"));
            setEntry("IS_SAVED", "NO");
            }, "Removes inheritance between classes");

        bk.bind(":edit ${ARG}", [this](void) {
            logix.editSelected(getEntry("ARG"));
            setEntry("IS_SAVED", "NO");
        }, "Edits selected");

        bk.bind(":del", [this](void) {
            logix.deleteSelected();
            setEntry("IS_SAVED", "NO");
        }, "Deletes selected");

        bk.bind("<DEL>", [this](void) {
            logix.deleteSelected();
            setEntry("IS_SAVED", "NO");
        }, "Deletes selected");
       
        bk.bind(":mode ${ARG}", [this](void){
            logix.setEditionMode(getEntry("ARG"));
        }, "Set edition mode (class/field/inheritance).");

        bk.bind(":sel ${ARG}", [this](void) {
            logix.selectByName(getEntry("ARG"));
        }, "Selects first thing with specified name");
        
        bk.bind(":unsel", [this](void) {
            logix.deselect(true);
        }, "Unselect");

        bk.bind("<RARROW>", [this](void) {
            logix.right();
        }, "Select right/next class");

        bk.bind("<LARROW>", [this](void) {
            logix.left();
        }, "Select left/previous class");

        bk.bind(":forget", [this](void){
            setEntry("IS_SAVED", "YES");
        }, "Forces backend to think, that file has been saved.");

    }

};

}

#endif
