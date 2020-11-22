#include "..\Header\StringHelpers.h"

using namespace std;

vector<string> StringHelpers::Split(const std::string& str, const std::string& delim)
{
	size_t last = 0;
	size_t next = 0;

	string token;
	vector<string> tokens;

	while ((next = str.find(delim, last)) != std::string::npos)
	{
		token = str.substr(last, next - last);
		tokens.push_back(token);
		last = next + delim.size();
	}

	tokens.push_back(str.substr(last));

	return tokens;
}

std::string StringHelpers::Trim(const std::string& str)
{
	const size_t strBegin = str.find_first_not_of(" \t");
	if (strBegin == std::string::npos) { return ""; }

	const size_t strEnd = str.find_last_not_of(" \t");

	return str.substr(strBegin, strEnd - strBegin + 1);
}
