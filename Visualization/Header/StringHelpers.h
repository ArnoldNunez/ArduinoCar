#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <string>
#include <vector>

namespace StringHelpers
{
	/**
	 * Splits a string by the provided delimeter string
	 */
	std::vector<std::string> Split(const std::string& str, const std::string& delim);


}

#endif // !STRING_HELPERS_H
