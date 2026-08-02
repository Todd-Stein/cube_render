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
    private:  
        T container[SIZE];   
};