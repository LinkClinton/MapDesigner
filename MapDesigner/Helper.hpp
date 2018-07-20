#pragma once

#include "pch.hpp"

class Helper {
public:
	static auto ToUpperString(std::string str) -> std::string;

	static auto SpiltString(std::string str) -> std::vector<std::string>;

	static auto IsNumber(std::string str) -> bool;

	static auto StringToInt(std::string str) -> int;
};