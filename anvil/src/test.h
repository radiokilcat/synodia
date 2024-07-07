#ifndef TEST_H
#define TEST_H

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
#endif // TEST_H
