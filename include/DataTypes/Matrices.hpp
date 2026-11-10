#ifndef MATRICES_HPP_
#define MATRICES_HPP_
#include <iostream>
#include <immintrin.h>

struct alignas(16) Matrix2x2 {
    using data_type = float;
    float data[2][2];
};
struct alignas(32) Matrix3x3 {
    using data_type = float;
    float data[3][3];
};
struct alignas(32) Matrix4x4 {
    using data_type = float;
    float data[4][4];
    Matrix4x4& operator=(const Matrix4x4& rhs) {
        if(this == &rhs) {
            return *this;
        }
        for(int i = 0; i<4; i++) {
            for(int j = 0; j<4; j++) {
                this->data[i][j] = rhs.data[i][j];
            }
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Matrix4x4& mat) {
        //print in college math order
        for(int i = 0; i<4; i++) {
            stream << "[";
            for (int j = 0; j<4; j++) {
                stream << mat.data[j][i] << " ";
            }
            stream << "]\n";
        }
        return stream;
    }
};
constexpr static Matrix2x2 Identity2x2 {
    {
        1.0f, 0.0f,
        0.0f, 1.0f
    }
};
constexpr static Matrix3x3 Identity3x3 {
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    }
};
constexpr static Matrix4x4 Identity4x4 {
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }
};

#endif