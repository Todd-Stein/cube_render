#ifndef ROTATION_MATH_HPP_
#define ROTATION_MATH_HPP_ 

#if __cplusplus == 202604L
    #define CPP_26_
#endif

#include "DataTypes/Vectors.hpp"
#include "DataTypes/Matrices.hpp"
#include "Math/MathConstants.hpp"
#include <cmath>

static constexpr Quaternion IdentityQuaternion {
    0.f,
    0.f,
    0.f,
    1.f
};

inline 
#ifdef CPP_26_
constexpr
#endif
Quaternion MakeQuaternion(Vector3 inRot) {
    //TODO: Implement in SIMD
    float halfCosX = std::cos(DEG2RAD*inRot.x*.5f);
    float halfSinX = std::sin(DEG2RAD*inRot.x*.5f);
    float halfCosY = std::cos(DEG2RAD*inRot.y*.5f);
    float halfSinY = std::sin(DEG2RAD*inRot.y*.5f);
    float halfCosZ = std::cos(DEG2RAD*inRot.z*.5f);
    float halfSinZ = std::sin(DEG2RAD*inRot.z*.5f);
    Quaternion result;
    result.w = (halfCosX * halfCosY * halfCosZ) + (halfSinX * halfSinY * halfSinZ);
    result.x = (halfSinX * halfCosY * halfCosZ) - (halfCosX * halfSinY * halfSinZ);
    result.y = (halfCosX * halfSinY * halfCosZ) + (halfSinX * halfCosY * halfSinZ);
    result.z = (halfCosX * halfCosY * halfSinZ) - (halfSinX * halfSinY * halfCosZ);
    return result;
}

inline constexpr Vector3 GetForwardVector(Matrix4x4& inMat) {
    Vector3 res;
    res.x = inMat.data[2][0];
    res.y = inMat.data[2][1];
    res.z = inMat.data[2][2];
    return res;
}
inline constexpr Vector3 GetUpVector(Matrix4x4& inMat) {
    Vector3 res;
    res.x = inMat.data[1][0];
    res.y = inMat.data[1][1];
    res.z = inMat.data[1][2];
    return res;
}
inline constexpr Vector3 GetRightVector(Matrix4x4& inMat) {
    Vector3 res;
    res.x = inMat.data[0][0];
    res.y = inMat.data[0][1];
    res.z = inMat.data[0][2];
    return res;
}

#endif