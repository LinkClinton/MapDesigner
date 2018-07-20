#pragma once

#include "pch.hpp"

struct Resource {
	std::string fileName;
	int renderObjectID;
	PixelWorldEngine::Rectangle rect; //x, y, width, height
};

class ResourceManager {
public:
	DataManager* dataManager;
	MergeTexture* mergeTexture;
	Application* application;

	std::vector<Resource> resources;

	static auto CreateMergeTextureFromResource(ResourceManager* manager) -> MergeTexture*;
public:
	ResourceManager(Application* application);

	~ResourceManager();

	void CreateResourceConfigFromFile(std::string fileName, int widthCount, int heightCount, int baseRenderObjectID);

	void OpenResourceConfig(std::string fileName);

	void SaveResourceConfig(std::string fileName);

	void MergeResourceConfig(std::string fileName);

	void CloseResourceConfig();

	auto GetBigFont() -> Font*;

	auto GetNormalFont() -> Font*;

	auto GetMergeTexture() -> MergeTexture*;
};