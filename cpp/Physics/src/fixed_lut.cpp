#include "math/fixed_lut.h"

#include "sin_const.h"
#include "cos_const.h"
#include "sqrt_const.h"

extern int sqrt_table[];
extern int sin_table[];
extern int cos_table[];

namespace neko
{
template<>
Fixed16 Sqrt(Fixed16 value)
{
    constexpr Fixed16 minValue = Fixed16::fromUnderlyingValue(sqrt_min);
    constexpr Fixed16 maxValue = Fixed16::fromUnderlyingValue(sqrt_max);
    constexpr Fixed16 delta = maxValue-minValue;
    const Fixed16 ratio = value-minValue;
    constexpr Fixed16 resolution = Fixed16::fromUnderlyingValue(sqrt_resolution);
    const auto index = (value-minValue)/(maxValue-minValue); // v = a + (b-a)*t; <=> (v-a)/(b-a)
    return Fixed16::fromUnderlyingValue(sqrt_table[(int)index]);
}

template<>
Fixed16 Sin(Fixed16 value)
{
    constexpr Fixed16 minValue = Fixed16::fromUnderlyingValue(0);
    constexpr Fixed16 maxValue = Pi<Fixed16>() * Fixed16{2};
    constexpr Fixed16 resolution = Fixed16::fromUnderlyingValue(sqrt_resolution);
    const auto index = (value-minValue)/(maxValue-minValue); // v = a + (b-a)*t; <=> (v-a)/(b-a)
    return Fixed16::fromUnderlyingValue(sin_table[(int)index]);
}

template<>
Fixed16 Cos(Fixed16 value)
{
    constexpr Fixed16 minValue = Fixed16::fromUnderlyingValue(0);
    constexpr Fixed16 maxValue = Pi<Fixed16>() * Fixed16{2};
    constexpr Fixed16 resolution = Fixed16::fromUnderlyingValue(sqrt_resolution);
    const auto index = (value-minValue)/(maxValue-minValue); // v = a + (b-a)*t; <=> (v-a)/(b-a)
    return Fixed16::fromUnderlyingValue(cos_table[(int)index]);
}
}