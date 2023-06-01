#pragma once

#include <iostream>
//#include <SDL3/SDL.h>

namespace anvil {


    class Test
    {
    public:
        Test();
        void print() { std::cout << "finally, we are here" << std::endl; }
    };

    void print1() { std::cout << "finally, we are here" << std::endl; };
}
