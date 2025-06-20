#include <iostream>

#include "DisplayService.hpp"
#include "SFML/Window/Window.hpp"

int main()
{
    std::cout << "Hello World!\n" << std::endl;
    DisplayService displayService;
    displayService.start();


    std::cout << "Goodbye World!\n" << std::endl;
    return 0;
}
