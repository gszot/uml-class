
// Compile with: g++ main.cpp -lncurses -o example

#include <iostream>
#include "Uml-class.hpp"
#include "VimBackend.hpp"
#include "include/Logic.hpp"

int main()
{
    tools::UmlClass tool;
    backends::VimBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
    std::cout<<"Koniec\n";
}
