#pragma once

#include "pch.hpp"

class AutoMergeTexture {
private:
	Application* app;
	MergeTexture * mergeTexture;

	int count;
	int widthLimit;
	int heightLimit;

	std::vector<int> availableID;
	std::map<int, int> mapUsedID;
public:
	AutoMergeTexture(Application* application, int widthLimit, int heightLimit, int defaultCount, PixelFormat pixelFormat);

	~AutoMergeTexture();

	void AddTexture(int id, Texture2D* texture);

	void RemoveTexture(int id);

	auto GetFirstAvailableID() -> int;

	auto GetMergeTexture() -> MergeTexture*;
};