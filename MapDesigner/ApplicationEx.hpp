#pragma once

#include "pch.hpp"

class MapDesigner;

class ApplicationEx : public Application {
private:
	MapDesigner* mapDesigner;

	friend class MapDesigner;
public:
	ApplicationEx(std::string applicationName, MapDesigner* mapDesigner);
};