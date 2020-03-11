#ifndef LOGIC_HPP_
#define LOGIC_HPP_

#include "Arrow.hpp"
#include "Renderer.hpp"
#include "Class.hpp"
#include "Inheritance.hpp"
#include <memory>
#include <cassert>
#include <fstream>
#include <typeinfo>

class Logic
{
    std::vector<std::shared_ptr<Class>> classes;
    std::vector<std::shared_ptr<Inheritance>> inheritances;

    std::weak_ptr<IActor> Selected;
    std::weak_ptr<IActor> SelectedField; //for field cycling

    static constexpr int minSpace = 12;
    static constexpr int minYPosition = 2;
    int actPosition{0};
    int actLevel{ 12 };
    
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

    void refitClasses()
    {
        actPosition = 0;

        for (auto& i : classes)
        {
            Point p{ actPosition += minSpace, minYPosition };
            i->setPosition(p);
        }
    }
    
    void checkInheritances()
    {
        inheritances.erase(std::remove_if(inheritances.begin(), inheritances.end(),
        [](const std::shared_ptr<Inheritance>& i)
        {
            return !i->start.lock() || !i->end.lock();
        }), inheritances.end());
    }

    void refitInheritances()
    {
        actLevel = 12;
        for (auto& i : inheritances)
        {
            i->level = actLevel++;
            i->refitLines();
        }
    }
    
    public:

    Logic() = default;

    void setRenderer(IRenderer* r)
    {
        rend = r;
    }

    void center(bool hard = false)
    {
        /*if (auto strongfield = SelectedField.lock())rend->setCenter(strongfield->getCenter());
        else if (auto strong = Selected.lock())rend->setCenter(strong->getCenter());
        if (hard)render();*/
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
            if (auto strongfield = SelectedField.lock())
                strongfield->selected = false;
            SelectedField.reset();
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

        //deselect(true);
    }

    void addClass(std::string name)
    {
        //if (currentMode == editionMode::Class)
        if(true)
        {
            Point p{ actPosition += minSpace, minYPosition };
            classes.push_back(std::move(std::make_shared<Class>(name, p)));

            render();
        }
    }

    void rmClass(std::string name)
    {
        if (true)
        {
            auto it = findByName(name, classes);

            if (it == classes.end()) return;

            classes.erase(it);

            render();
        }
    }

    void addFieldToClass(std::string className, std::string fieldName)
    {
        //if (currentMode == editionMode::Field)
        if (true)
        {
            auto it = findByName(className, classes);

            if (it == classes.end()) return;

            (*it)->addField(fieldName);

            render();
        }
    }

    void rmFieldFromClass(std::string className, std::string fieldName)
    {
        //if (currentMode == editionMode::Field)
        if (true)
        {
            auto it = findByName(className, classes);

            if (it == classes.end()) return;

            (*it)->removeField(fieldName);

            render();
        }
    }

    void addInheritance(std::string beginName, std::string endName)
    {
        auto it1 = findByName(beginName, classes);
        auto it2 = findByName(endName, classes);

        if (it1 == classes.end() || it2 == classes.end())
        {
            return;
        }

        auto newinh = std::make_shared<Inheritance>();
        newinh->start = *it1;
        newinh->end = *it2;
        inheritances.push_back(std::move(newinh));
        render();
    }

    void rmInheritance(std::string beginName, std::string endName)
    {
        auto it = inheritances.begin();
        bool found = false;
        for (auto i = inheritances.begin(); i != inheritances.end(); i++)
        {
            if ((*i)->start.lock()->name == beginName && (*i)->end.lock()->name == endName)
            {
                found = true;
                it = i;
                break;
            }
        }
        if(found) inheritances.erase(it);
        render();
    }

    void selectByName(std::string name)
    {
        if (currentMode == editionMode::Class)
        {
            auto it = findByName(name, classes);

            if (it != classes.end())
            {
                deselect();
                (*it)->selected = true;
                Selected = *it;

                rend->setCenter((*it)->getCenter());

                render();
            }
        }
        if (currentMode == editionMode::Field)
        {
            auto strong = Selected.lock();
            if (!strong) return;
            auto strongCast = std::dynamic_pointer_cast<Class>(strong);//class
            auto it = findByName(name, strongCast->fields);

            if (it != strongCast->fields.end())
            {
                if (auto strongfield = SelectedField.lock())
                    strongfield->selected = false;
                (*it)->selected = true;
                SelectedField = *it;
                strongCast->selected = false;

                render();
            }
        }
    }

    void editSelected(std::string name)
    {
        auto strongfield = SelectedField.lock();
        auto strong = Selected.lock();
        if (strongfield)
        {
            (std::dynamic_pointer_cast<Field>(strongfield))->setName(name);
            (std::dynamic_pointer_cast<Class>(strong))->boxUpdate();
        }
        else if (strong)
        {
            (std::dynamic_pointer_cast<Class>(strong))->setName(name);
        }
        render();
    }

    void deleteSelected()
    {
        auto strongfield = SelectedField.lock();
        auto strong = Selected.lock();//class
        if (strongfield)
        {
            auto strongCast = std::dynamic_pointer_cast<Class>(strong);//class
            auto& container = strongCast->fields;
            auto oldsize = container.size();
            container.erase(std::remove_if(container.begin(), container.end(),
                [&strongfield](const std::shared_ptr<IActor>& p)
                {
                    return p.get() == strongfield.get();
            }), container.end());

            if (oldsize != container.size())
            {
                strongfield.reset(); //elimin. silnego dowiązania
                strongCast->boxUpdate();
                render();
            }
        }
        else if (strong)
        {
            auto& container = classes;
            auto oldsize = container.size();
            container.erase(std::remove_if(container.begin(), container.end(),
                [&strong](const std::shared_ptr<IActor>& p)
                {
                    return p.get() == strong.get();
            }), container.end());

            if (oldsize != container.size())
            {
                strong.reset(); //elimin. silnego dowiązania

                render();
            }
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

    void right(bool loop = true)
    {
        if (currentMode == editionMode::None) return;
        if (currentMode == editionMode::Class)
        {
            if (classes.size() == 0)return;

            auto strong = Selected.lock();

            deselect(false);

            if (!strong)
            {
                Selected = strong = *classes.begin();
                strong->selected = true;
            }
            else
            {
                auto it = getIteratorFromPointerActual<Class>(std::dynamic_pointer_cast<Class>(strong), classes);
                std::advance(it, 1);
                if (it != classes.end())
                {
                    Selected = strong = *it;
                    strong->selected = true;
                }
                else if (loop)
                {
                    Selected = strong = *classes.begin();
                    strong->selected = true;
                }
            }

            if (strong)
            {
                center();
            }

            render();
        }
        if (currentMode == editionMode::Field)
        {
            auto strong = Selected.lock();
            auto strongfield = SelectedField.lock();
            auto strongclass = std::dynamic_pointer_cast<Class>(strong);
            
            if (!strong) return;

            if (!strongfield)
            {
                if (strongclass->fields.size() == 0)return;
                SelectedField = strongfield = *strongclass->fields.begin();
                strongfield->selected = true;
                strong->selected = false;
            }
            
            else
            {
                strong->selected = false;
                strongfield->selected = false;
                auto it = getIteratorFromPointerActual<Field>(std::dynamic_pointer_cast<Field>(strongfield), strongclass->fields);
                std::advance(it, 1);
                if (it != strongclass->fields.end())
                {
                    SelectedField = strongfield = *it;
                    strongfield->selected = true;
                }
                else if (loop)
                {
                    SelectedField = strongfield = *strongclass->fields.begin();
                    strongfield->selected = true;
                }
            }
            
            if (strong)
            {
                center();
            }

            render();
        }
        if (currentMode == editionMode::Inheritance) return;
    }

    void left(bool loop = true)
    {
        if (currentMode == editionMode::None) return;
        if (currentMode == editionMode::Class)
        {
            if (classes.size() == 0)return;

            auto strong = Selected.lock();

            deselect(false);

            if (!strong)
            {
                Selected = strong = *classes.crbegin();
                strong->selected = true;
            }
            else
            {
                auto it = getIteratorFromPointerActualReversed<Class>(std::dynamic_pointer_cast<Class>(strong), classes);
                std::advance(it, 1);
                if (it != classes.crend())
                {
                    Selected = strong = *it;
                    strong->selected = true;
                }
                else if (loop)
                {
                    Selected = strong = *classes.crbegin();
                    strong->selected = true;
                }
            }

            if (strong)
            {
                center();
            }

            render();
        }
        if (currentMode == editionMode::Field)
        {
            auto strong = Selected.lock();
            auto strongfield = SelectedField.lock();
            auto strongclass = std::dynamic_pointer_cast<Class>(strong);

            if (!strong) return;

            if (!strongfield)
            {
                if (strongclass->fields.size() == 0)return;
                SelectedField = strongfield = *strongclass->fields.begin();
                strongfield->selected = true;
                strong->selected = false;
            }

            else
            {
                strong->selected = false;
                strongfield->selected = false;
                auto it = getIteratorFromPointerActualReversed<Field>(std::dynamic_pointer_cast<Field>(strongfield), strongclass->fields);
                std::advance(it, 1);
                if (it != strongclass->fields.crend())
                {
                    SelectedField = strongfield = *it;
                    strongfield->selected = true;
                }
                else if (loop)
                {
                    SelectedField = strongfield = *strongclass->fields.crbegin();
                    strongfield->selected = true;
                }
            }

            if (strong)
            {
                center();
            }

            render();
        }
        if (currentMode == editionMode::Inheritance) return;
    }

    void render()
    {
        assert(rend != nullptr);

        rend->clearWindow();
        refitClasses();
        checkInheritances();
        refitInheritances();

        for (auto &i : classes)
        {
            i->draw(*rend);
        }

        for (auto &i : inheritances)
        {
            i->draw(*rend);
        }

        rend->update();
    }
};

#endif
