#include <string>
#include <iostream>
#include <utility>
#include <cassert>
#include <concepts>
#include "InteropString.hpp"

template <class T>
concept string_iterator = requires (T t)
{
	std::contiguous_iterator<T>;
	std::same_as<typename T::value_type, char>;
};

static void	skip_whitespaces(string_iterator auto& it, string_iterator auto end)
{
	while (isspace(*it) && it < end)
		++it;
}

std::pair<std::string::const_iterator, std::string::const_iterator>
get_block_range(std::string::const_iterator begin, std::string::const_iterator end)
{
	int depth = 0;

	assert(*begin++ == '{');
	skip_whitespaces(begin, end);

	auto it = begin;
	for (; it != end && depth >= 0; ++it)
	{
		if (*it == '{')
			depth++;
		else if (*it == '}')
			depth--;
	}
	return { begin, it };
}

// nah, fuck that
// TODO: use sol2 and some sort of seralizing function.
// DCS Lua tables are always sorted in ASCII order!

// std::pair<std::string::iterator, std::string::iterator>
// get_value_location(std::string &file_contents, std::initializer_list<std::string_view> key_sequence)
// {
// 	for (auto key = key_sequence.begin(); key != key_sequence.end(); ++key)
// 	{
// 
// 	}
// }
