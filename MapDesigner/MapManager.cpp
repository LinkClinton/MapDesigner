#include "MapManager.hpp"

MapManager::MapManager()
{
	map = nullptr;
}

MapManager::~MapManager()
{
	CloseMap();
}

void MapManager::CreateMap(std::string name, int width, int height)
{
	CloseMap();

	map = new WorldMap(name, width, height);

	map->SetMapBlockSize(DEFAULT_MAP_BLOCK_SIZE);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			auto mapData = new MapData();

			mapData->RenderObjectID = DEFAULT_MAP_RENDER_OBJECT_ID;

			map->SetMapData(x, y, mapData);
		}
	}
}

void MapManager::OpenMap(std::string fileName)
{
	CloseMap();
	
	std::ifstream file;

	file.open(fileName);

	if (!file) ERROR_RETURN("打开的地图路径不存在。");

	std::string worldName;
	int width;
	int height;

	file >> worldName >> width >> height;

	map = new WorldMap(worldName, width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int renderObjectID; int moveEnable;

			file >> renderObjectID >> moveEnable;

			auto mapData = new MapData();

			mapData->RenderObjectID = renderObjectID;
			mapData->MoveEnable = (bool)moveEnable;

			if (mapData->RenderObjectID == 0)
				mapData->RenderObjectID = DEFAULT_MAP_RENDER_OBJECT_ID;

			map->SetMapData(x, y, mapData);
		}
	}

	file.close();

	map->SetMapBlockSize(DEFAULT_MAP_BLOCK_SIZE);
}

void MapManager::SaveMap(std::string fileName)
{
	if (map == nullptr) ERROR_RETURN("没有需要保存的地图。");

	std::ofstream file;

	file.open(fileName);

	file << map->GetMapName() << " " << map->GetWidth() << " " << map->GetHeight() << std::endl;

	for (int y = 0; y < map->GetHeight(); y++) {
		for (int x = 0; x < map->GetWidth(); x++) {
			auto mapData = map->GetMapData(x, y);

			int renderObjectID = mapData->RenderObjectID;
			int moveEnable = (int)mapData->MoveEnable;

			if (renderObjectID == DEFAULT_MAP_RENDER_OBJECT_ID)
				renderObjectID = 0;

			file << renderObjectID << " " << moveEnable << " ";
		}
		file << std::endl;
	}

	file.close();
}

void MapManager::CloseMap()
{
	if (map == nullptr) return;

	for (int x = 0; x < map->GetWidth(); x++) {
		for (int y = 0; y < map->GetHeight(); y++) {
			auto mapData = map->GetMapData(x, y);

			Utility::Delete(mapData);
		}
	}

	Utility::Delete(map);
}