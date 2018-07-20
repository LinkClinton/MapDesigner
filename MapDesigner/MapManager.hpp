#pragma once

#include "pch.hpp"

class MapManager {
public:
	WorldMap * map;
public:
	MapManager();
	
	~MapManager();

	void CreateMap(std::string name, int width, int height);

	void OpenMap(std::string fileName);

	void SaveMap(std::string fileName);

	void CloseMap();
};
