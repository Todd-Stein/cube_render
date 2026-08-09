#ifndef MATRIX_MATH_HPP_
#define MATRIX_MATH_HPP_
#include "DataTypes/Matrices.hpp"
#include "DataTypes/Vectors.hpp"
#include "Math/MathConstants.hpp"

//with transpose
inline Matrix4x4 Dot(const Matrix4x4& lhs, const Matrix4x4& rhs) {
    Matrix4x4 result;
    __m128 r0, r1, r2, r3;
    __m128 tmp0;//, tmp1;
            
    __m128 columns[4];
    columns[0] = _mm_load_ps(&rhs.data[0][0]);
    columns[1] = _mm_load_ps(&rhs.data[1][0]);
    columns[2] = _mm_load_ps(&rhs.data[2][0]);
    columns[3] = _mm_load_ps(&rhs.data[3][0]);
    __m128 res[4];

    for(int i = 0; i<4; i++) {
        res[i] = _mm_setzero_ps();
        tmp0 = _mm_set1_ps(lhs.data[0][i]);
        r0 = _mm_mul_ps(tmp0, columns[0]);
        res[i] = _mm_add_ps(res[i], r0);
        tmp0 = _mm_set1_ps(lhs.data[1][i]);
        r1 = _mm_mul_ps(tmp0, columns[1]);
        res[i] = _mm_add_ps(res[i], r1);
        tmp0 = _mm_set1_ps(lhs.data[2][i]);
        r2 = _mm_mul_ps(tmp0, columns[2]);
        res[i] = _mm_add_ps(res[i], r2);
        tmp0 = _mm_set1_ps(lhs.data[3][i]);
        r3 = _mm_mul_ps(tmp0, columns[3]);
        res[i] = _mm_add_ps(res[i], r3);
    }
    for(int i = 0; i<4; i++) {
        _mm_store_ps(&result.data[i][0], res[i]);
    }
    return result;
}
inline Matrix4x4& Transpose(Matrix4x4& in) {
    Matrix4x4 result;
    __m128 r0 = _mm_load_ps(&in.data[0][0]);
    __m128 r1 = _mm_load_ps(&in.data[1][0]);
    __m128 r2 = _mm_load_ps(&in.data[2][0]);
    __m128 r3 = _mm_load_ps(&in.data[3][0]);
    #if defined(_MM_TRANSPOSE4_PS)
        __m128 tmp0, tmp1, res[4];

        res[0] = _mm_unpacklo_ps(r0, r1);
        res[1] = _mm_unpacklo_ps(r2, r3);
        res[2] = _mm_unpackhi_ps(r0, r1);
        res[3] = _mm_unpackhi_ps(r2, r3);

        tmp0 = _mm_unpacklo_ps(res[0], res[1]);
        tmp1 = _mm_unpackhi_ps(res[0], res[1]);
        res[0] = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE(3, 1, 2, 0));
        res[1] = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(3, 1, 2, 0));

        tmp0 = _mm_unpacklo_ps(res[2], res[3]);
        tmp1 = _mm_unpackhi_ps(res[2], res[3]);
        res[2] = _mm_shuffle_ps(tmp0, tmp0, _MM_SHUFFLE(3, 1, 2, 0));
        res[3] = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(3, 1, 2, 0));

        for(int i = 0; i<4; i++) {
            _mm_store_ps(&result.data[i][0], res[i]);
        }
    #else
        _MM_TRANSPOSE4_PS(r0, r1, r2, r3);
        _mm_store_ps(&result.data[0][0], r0);
        _mm_store_ps(&result.data[1][0], r1);
        _mm_store_ps(&result.data[2][0], r2);
        _mm_store_ps(&result.data[3][0], r3);
    #endif
    return result;
}
inline constexpr Matrix4x4 CreateTranslationMatrix(Vector3 in) {
    Matrix4x4 result = Identity4x4;
    result.data[3][0] = in.x;
    result.data[3][1] = in.y;
    result.data[3][2] = in.z;
    return result;
}
inline constexpr Matrix4x4 CreateRotationMatrix(Quaternion inRot) {
    //TODO: Implement in SIMD
    Matrix4x4 result = Identity4x4;
    float i = inRot.x;
    float i2 = inRot.x*inRot.x;
    float j = inRot.y;
    float j2 = inRot.y*inRot.y;
    float k = inRot.z;
    float k2 = inRot.z*inRot.z;
    float r = inRot.w;
    float r2 = inRot.w*inRot.w;
    result.data[0][0] = 1.f - 2.f * (j2 + k2);
    result.data[0][1] = 2.f*(i*j) - 2.f*(r*k);
    result.data[0][2] = 2.f*(i*k)+2.f*(r*j);

    result.data[1][0] = 2.f*(i*j) + 2.f*(r*k);
    result.data[1][1] = 1.f - 2.f * (i2 + k2);
    result.data[1][2] = 2.f*(j*k) - 2.f*(r*i);

    result.data[2][0] = 2.f*(i*k) - 2.f*(r*j);
    result.data[2][1] = 2.f*(j*k) + 2.f*(r*i);
    result.data[2][2] = 1.f - 2.f * (i2 + j2);
    return result;
}


inline Matrix4x4 CreatePerspectiveProjectionMatrix(const float fov, const float near, const float far, float aspectRatio) {
    //put into row major order
    Matrix4x4 projMat = Identity4x4;
    float projScale = 1/(std::tan(fov*DEG2RAD*0.5f));
    projMat.data[0][0] = projScale/aspectRatio;
    projMat.data[1][1] = projScale;
    projMat.data[2][2] = (far+near)/(near-far);
    projMat.data[2][3] = (2*far*near)/(near-far);
    projMat.data[3][2] = -1.0f;
    projMat.data[3][3] = 0.0f;
    return projMat;
}

#endif