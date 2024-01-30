#pragma once
#include <concepts>

template <typename T> requires std::is_floating_point<T>::value
inline std::pair<T, T>	modf2(T val)
{
	T int_part, fract_part;
	fract_part = modf(val, &int_part);
	return { int_part, fract_part };
}
