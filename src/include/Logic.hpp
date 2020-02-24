#ifndef LOGIC_HPP_
#define LOGIC_HPP_

#include "Arrow.hpp"
#include "Renderer.hpp"
#include "Class.hpp"
#include <memory>
#include <cassert>
#include <fstream>

class Logic
{
    std::vector<std::shared_ptr<Class>> classes;
    std::vector<std::shared_ptr<Inheritance>> inheritances;

    std::weak_ptr<IActor> Selected;

    static constexpr int minSpace = 12;
    static constexpr int minYPosition = 2;
    int actPosition{0};
    
    IRenderer *rend{nullptr};

    enum class editionMode
    {
        Class,
        Field,
        Inheritance,
        None = 0xF,
    };

    editionMode currentMode{editionMode::None};

    template <typename T>
    auto getIteratorFromPointerActual(const std::shared_ptr<T> &sp, const std::vector<std::shared_ptr<T>> &ct) const //pointer jest wazny, bo inne funkcje dbają o zresetowanie go jesli nie jest
    {
        return (std::find_if(ct.begin(), ct.end(), 
            [&sp](const std::shared_ptr<T> &p)
            {
                return p.get() == sp.get();
        }));
    }

    template <typename T>
    auto getIteratorFromPointerActualReversed(const std::shared_ptr<T> &sp, const std::vector<std::shared_ptr<T>> &ct) const //pointer jest wazny, bo inne funkcje dbają o zresetowanie go jesli nie jest
    {
        return (std::find_if(ct.crbegin(), ct.crend(), 
            [&sp](const std::shared_ptr<T> &p)
            {
                return p.get() == sp.get();
        }));
    }

    template <typename T>
    auto findByName(std::string name, const std::vector<std::shared_ptr<T>> &ct) const
    {
        return (std::find_if(ct.begin(), ct.end(), 
        [&name](const std::shared_ptr<T> &p)
            {
                return p->getName() == name;
        }));
    }

    public:

    Logic() = default;

    void setRenderer(IRenderer* r)
    {
        rend = r;
    }

    void center(bool hard = false)
    {
        if (auto strong = Selected.lock())rend->setCenter(strong->getCenter());
        if (hard)render();
    }

    void setEditionMode(std::string mode)
    {
        std::for_each(mode.begin(), mode.begin(), [](char& c)
        {
            c = tolower(c);
        });

        if (mode == "c")
        {
            currentMode = editionMode::Class;
        }
        else if (mode == "f")
        {
            currentMode = editionMode::Field;
        }
        else if (mode == "i")
        {
            currentMode = editionMode::Inheritance;
        }
        else
        {
            currentMode = editionMode::None;
        }

        deselect(true);
    }

    void addClass(std::string name)
    {
        if (currentMode == editionMode::Class)
        {
            Point p{ actPosition += minSpace, minYPosition };
            classes.push_back(std::move(std::make_shared<Class>(name, p)));

            render();
        }
    }

    void deselect(bool reset = false)
    {
        if (auto strong = Selected.lock())
        {
            strong->selected = false;
            Selected.reset();
        }
        if (reset)
        {
            rend->resetCenter();
            render();
        }
    }

    void render()
    {
        assert(rend != nullptr);

        rend->clearWindow();

        for (auto &i : classes)
        {
            i->draw(*rend);
        }

        /*for (auto &i : inheritances)
        {
            i->lateConstructor();
            i->draw(*rend);
        }*/

        rend->update();
    }
};

#endif
