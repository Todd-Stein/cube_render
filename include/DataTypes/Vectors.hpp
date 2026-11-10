#ifndef VECTOR_DATA_TYPES
#define VECTOR_DATA_TYPES

#include "Containers/Simple_Array.hpp"

struct alignas(8) Vector2 {
    using data_type = float;
    float x;
    float y;
};
struct alignas(16) Vector3 {
    using data_type = float;
    float x;
    float y;
    float z;
    constexpr Vector3() : x(0.f), y(0.f), z(0.f) {};
    constexpr Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {};
    Vector3& operator+=(const Vector3& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
    Vector3& operator-=(const Vector3& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }
    // Vector3& CalculateNormals(const Simple_Array<Vector3, 3>& triangle) {
    //     Vector3 edge1 = triangle[1] - triangle[0];
    //     Vector3 edge2 = triangle[1] - triangle[2];
    //     return edge1;
    // }
};
struct alignas(16) Vector4 {
    using data_type = float;
    constexpr Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {};
    constexpr Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {};
    float x;
    float y;
    float z;
    float w;
};
//just included quaternion here bc I didn't see a point in making another header file JUST for 1 data type
struct alignas(16) Quaternion {
    using data_type = float;
    constexpr Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {};
    constexpr Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {};
    float x;
    float y;
    float z;
    float w;
};
#endif