#include "pch.hpp"

#include "MapDesigner.hpp"

MapDesigner mapDesigner = MapDesigner();

int main() {
	mapDesigner.Initialize();

	mapDesigner.RunLoop();
}