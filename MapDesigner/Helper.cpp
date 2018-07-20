#include "Helper.hpp"

auto Helper::ToUpperString(std::string str) -> std::string
{
	std::string result;

	for (auto it = str.begin(); it != str.end(); it++) {
		if (*it >= 'a' && *it <= 'z') result.push_back(*it - 'a' + 'A');
		else result.push_back(*it);
	}

	return result;
}

auto Helper::SpiltString(std::string str) -> std::vector<std::string>
{
	std::vector<std::string> result;

	std::string tempValue = "";

	for (auto it = str.begin(); it != str.end(); it++) {
		if (*it == ' ') {

			result.push_back(tempValue);
			tempValue.clear();

			continue;
		}

		tempValue.push_back(*it);
	}

	result.push_back(tempValue);

	return result;
}

auto Helper::IsNumber(std::string str) -> bool
{
	if (str.length() == 0) return false;

	if (str[0] == '0') return false;

	for (auto it = str.begin(); it != str.end(); it++) 
		if (*it < '0' || *it >'9') return false;

	return true;
}

auto Helper::StringToInt(std::string str) -> int
{
	int result = 0;

	for (auto it = str.begin(); it != str.end(); it++) 
		result = result * 10 + *it - '0';

	return result;
}
