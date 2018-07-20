#pragma once

#include "pch.hpp"
#include "MapManager.hpp"
#include "ResourceManager.hpp"
#include "ApplicationEx.hpp"
#include "CommandArea.hpp"
#include "RenderObjectArea.hpp"

class MapDesigner {
private:
	const std::string name = "MapDesigner";
public:
	ApplicationEx* application;
	PixelWorld* world;
	
	MapManager* mapManager;
	ResourceManager* resourceManager;
	TextureManager* textureManager;

	AutoMergeTexture* textMergeTexture;
	AutoMergeTexture* defaultMergeTexture;

	Camera* mainCamera;
	float mainCameraTimes;

	CommandArea* commandArea;
	RenderObjectArea* renderObjectArea;

	float ssaa;

	void CreateObject();

	void CreateTextureManager();

	void DestoryObject();

	void DestoryTextureManager();

	void LoadConfig();
	
	void SetMap();

	void SetResourceConfig();

	static void OnUpdate(void* sender);

	static void OnKeyDownEvent(void* sender, Events::KeyClickEvent* eventArg);

	static void OnMouseDownEvent(void* sender, Events::MouseClickEvent* eventArg);

	static void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg);
public:
	MapDesigner();

	~MapDesigner();

	void Initialize();

	void RunLoop();
};