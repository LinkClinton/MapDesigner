#include "ApplicationEx.hpp"

ApplicationEx::ApplicationEx(std::string applicationName, MapDesigner* MapDesigner)
	: Application(applicationName)
{
	mapDesigner = MapDesigner;
}
