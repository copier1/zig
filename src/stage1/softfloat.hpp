/*
 * Copyright (c) 2017 Andrew Kelley
 *
 * This file is part of zig, which is MIT licensed.
 * See http://opensource.org/licenses/MIT
 */

#ifndef ZIG_SOFTFLOAT_HPP
#define ZIG_SOFTFLOAT_HPP

extern "C" {
#include "softfloat.h"
}

#include "zigendian.h"

static inline float16_t zig_double_to_f16(double x) {
    float64_t y;
    static_assert(sizeof(x) == sizeof(y), "");
    memcpy(&y, &x, sizeof(x));
    return f64_to_f16(y);
}

static inline void zig_double_to_extF80M(double x, extFloat80_t *result) {
    float64_t y;
    static_assert(sizeof(x) == sizeof(y), "");
    memcpy(&y, &x, sizeof(x));
    f64_to_extF80M(y, result);
}

static inline void zig_double_to_f128M(double x, float128_t *result) {
    float64_t y;
    static_assert(sizeof(x) == sizeof(y), "");
    memcpy(&y, &x, sizeof(x));
    f64_to_f128M(y, result);
}


// Return value is safe to coerce to float even when |x| is NaN or Infinity.
static inline double zig_f16_to_double(float16_t x) {
    float64_t y = f16_to_f64(x);
    double z;
    static_assert(sizeof(y) == sizeof(z), "");
    memcpy(&z, &y, sizeof(y));
    return z;
}

static inline bool zig_f16_isNaN(float16_t a) {
    union { uint16_t ui; float16_t f; } uA;
    uA.f = a;
    return 0x7C00 < (uA.ui & 0x7FFF);
}

static inline bool zig_f128_isNaN(float128_t *aPtr) {
    uint64_t hi, lo;

    #if defined(ZIG_BYTE_ORDER) && ZIG_BYTE_ORDER == ZIG_LITTLE_ENDIAN
        hi = aPtr->v[1];
        lo = aPtr->v[0];
    #elif defined(ZIG_BYTE_ORDER) && ZIG_BYTE_ORDER == ZIG_BIG_ENDIAN
        hi = aPtr->v[0];
        lo = aPtr->v[1];
    #else
        #error Unsupported endian
    #endif

    uint64_t absA64 = hi & UINT64_C(0x7FFFFFFFFFFFFFFF);
    return
        (UINT64_C(0x7FFF000000000000) < absA64)
            || ((absA64 == UINT64_C(0x7FFF000000000000)) && lo);
}

static inline bool zig_extF80_isNaN(extFloat80_t *aPtr) {
    return (aPtr->signExp & 0x7FFF) == 0x7FFF && aPtr->signif & UINT64_C(0x7FFFFFFFFFFFFFFF);
}

#endif
