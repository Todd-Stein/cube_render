#ifndef SIMPLE_ARRAY_HPP_
#define SIMPLE_ARRAY_HPP_
#include <stdlib.h>
#include <stdexcept>

template <typename T, size_t SIZE>

class Simple_Array{
    public:
        T& operator[](int idx) {
            if(idx>SIZE || idx<0)
                throw std::out_of_range("Index out of bounds.");
            return container[idx];
        }
        //TODO: fix index operator implementation
    private:  
        T container[SIZE];   
};
#endif