#ifndef MY_HEADERS_H
#define MY_HEADERS_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <format>
#include <cmath>
#include <type_traits>

#include "Utils.h"
#include "Const.h"

#if !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif

#if !defined(M_PI_F)
#define M_PI_F static_cast<float>(M_PI)
#endif

#if !defined(M_PI_2)
#define M_PI_2 static_cast<float>(M_PI / 2.0)
#endif

#if !defined(M_PI_4)
#define M_PI_4 static_cast<float>(M_PI / 4.0)
#endif

#if !defined(M_E)
#define M_E 2.71828182845904523536
#endif

#if !defined(M_E_F)
#define M_E_F static_cast<float>(M_E)
#endif

#if !defined(M_SQRT2)
#define M_SQRT2 1.41421356237309504880
#endif

#if !defined(M_SQRT2_F)
#define M_SQRT2_F static_cast<float>(M_SQRT2)
#endif

#if !defined(M_SQRT1_2)
#define M_SQRT1_2 0.70710678118654752440
#endif

#if !defined(M_SQRT1_2_F)
#define M_SQRT1_2_F static_cast<float>(M_SQRT1_2)
#endif

#if !defined(M_LN2)
#define M_LN2 0.69314718055994530942
#endif

#if !defined(M_LN2_F)
#define M_LN2_F static_cast<float>(M_LN2)
#endif

#if !defined(M_LN10)
#define M_LN10 2.30258509299434308761
#endif

#if !defined(M_LN10_F)
#define M_LN10_F static_cast<float>(M_LN10)
#endif

#if !defined(M_LOG2E)
#define M_LOG2E 1.44269504088896340736
#endif

#if !defined(M_LOG2E_F)
#define M_LOG2E_F static_cast<float>(M_LOG2E)
#endif

#if !defined(M_LOG10E)
#define M_LOG10E 0.43429448190325182765
#endif

#if !defined(M_LOG10E_F)
#define M_LOG10E_F static_cast<float>(M_LOG10E)
#endif

#if !defined(M_SQRT1_2)
#define M_SQRT1_2 0.70710678118654752440
#endif

#if !defined(M_SQRT1_2_F)
#define M_SQRT1_2_F static_cast<float>(M_SQRT1_2)
#endif

#if !defined(M_2PI)
#define M_2PI 6.28318530717958647692
#endif

#if !defined(M_2PI_F)
#define M_2PI_F static_cast<float>(M_2PI)
#endif

#if !defined(M_2PI_4)
#define M_2PI_4 static_cast<float>(M_2PI / 4.0)
#endif

#if !defined(M_2PI_4_F)
#define M_2PI_4_F static_cast<float>(M_2PI / 4.0)
#endif

#if !defined(M_2PI_INV)
#define M_2PI_INV 0.15931422
#endif

#if !defined(M_2PI_INV_F)
#define M_2PI_INV_F static_cast<float>(M_2PI_INV)
#endif

#if !defined(M_TWOPI)
#define M_TWOPI 6.28318530717958647692
#endif

#if !defined(M_TWOPI_F)
#define M_TWOPI_F static_cast<float>(M_TWOPI)
#endif

#if !defined(M_TWOPI_4)
#define M_TWOPI_4 static_cast<float>(M_TWOPI / 4.0)
#endif

#if !defined(M_TWOPI_4_F)
#define M_TWOPI_4_F static_cast<float>(M_TWOPI / 4.0)
#endif

#if !defined(M_TWOPI_INV)
#define M_TWOPI_INV 0.15931422
#endif

#if !defined(M_TWOPI_INV_F)
#define M_TWOPI_INV_F static_cast<float>(M_TWOPI_INV)
#endif

#if !defined(M_HALFPI)
#define M_HALFPI 1.57079632679489661923
#endif

#if !defined(M_HALFPI_F)
#define M_HALFPI_F static_cast<float>(M_HALFPI)
#endif

#if !defined(M_HALFPI_2)
#define M_HALFPI_2 0.78539816339744830962
#endif

#if !defined(M_HALFPI_2_F)
#define M_HALFPI_2_F static_cast<float>(M_HALFPI_2)
#endif

#if !defined(M_HALFPI_4)
#define M_HALFPI_4 0.39269908169872415481
#endif

#if !defined(M_HALFPI_4_F)
#define M_HALFPI_4_F static_cast<float>(M_HALFPI_4)
#endif

#if !defined(M_HALFPI_INV)
#define M_HALFPI_INV 0.63661977236758134308
#endif

#if !defined(M_HALFPI_INV_F)
#define M_HALFPI_INV_F static_cast<float>(M_HALFPI_INV)
#endif

#if !defined(M_QUARTERPI)
#define M_QUARTERPI 0.78539816339744830962
#endif

#if !defined(M_QUARTERPI_F)
#define M_QUARTERPI_F static_cast<float>(M_QUARTERPI)
#endif

#if !defined(M_QUARTERPI_2)
#define M_QUARTERPI_2 0.39269908169872415481
#endif

#if !defined(M_QUARTERPI_2_F)
#define M_QUARTERPI_2_F static_cast<float>(M_QUARTERPI_2)
#endif

#if !defined(M_QUARTERPI_4)
#define M_QUARTERPI_4 0.19634954084936206923
#endif

#if !defined(M_QUARTERPI_4_F)
#define M_QUARTERPI_4_F static_cast<float>(M_QUARTERPI_4)
#endif

#if !defined(M_QUARTERPI_INV)
#define M_QUARTERPI_INV 2.6179938779914943654
#endif

#if !defined(M_QUARTERPI_INV_F)
#define M_QUARTERPI_INV_F static_cast<float>(M_QUARTERPI_INV)
#endif

#if !defined(M_EPSILON)
#define M_EPSILON 1e-6
#endif

#if !defined(M_EPSILON_F)
#define M_EPSILON_F static_cast<float>(M_EPSILON)
#endif

#if !defined(M_EPSILON_S)
#define M_EPSILON_S 1e-7
#endif

#if !defined(M_EPSILON_S_F)
#define M_EPSILON_S_F static_cast<float>(M_EPSILON_S)
#endif

#if !defined(M_EPSILON_L)
#define M_EPSILON_L 1e-9
#endif

#if !defined(M_EPSILON_L_F)
#define M_EPSILON_L_F static_cast<float>(M_EPSILON_L)
#endif

#if !defined(M_EPSILON_UL)
#define M_EPSILON_UL 1e-11
#endif

#if !defined(M_EPSILON_UL_F)
#define M_EPSILON_UL_F static_cast<float>(M_EPSILON_UL)
#endif

#if !defined(M_EPSILON_DBL)
#define M_EPSILON_DBL 1e-15
#endif

#if !defined(M_EPSILON_DBL_F)
#define M_EPSILON_DBL_F static_cast<float>(M_EPSILON_DBL)
#endif

#if !defined(M_EPSILON_LDBL)
#define M_EPSILON_LDBL 1e-17
#endif

#if !defined(M_EPSILON_LDBL_F)
#define M_EPSILON_LDBL_F static_cast<float>(M_
