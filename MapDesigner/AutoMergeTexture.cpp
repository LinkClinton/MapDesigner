#include "AutoMergeTexture.hpp"

AutoMergeTexture::AutoMergeTexture(Application * application, int WidthLimit, int HeightLimit, int defaultCount, PixelFormat pixelFormat)
{
	app = application;

	widthLimit = WidthLimit;
	heightLimit = HeightLimit;
	count = defaultCount;

	mergeTexture = new MergeTexture(app, widthLimit, heightLimit * count, pixelFormat);

	availableID.resize(count);

	for (int i = 0; i < count; i++)
		availableID[i] = i;
}

AutoMergeTexture::~AutoMergeTexture()
{
	Utility::Delete(mergeTexture);
}

void AutoMergeTexture::AddTexture(int id, Texture2D * texture)
{
	if (texture->GetWidth() > widthLimit || texture->GetHeight() > heightLimit)
		ERROR_RETURN("纹理的高度和宽度太大，无法管理纹理。");

	if (availableID.size() == 0)
		ERROR_RETURN("纹理过多，自动管理器无法加入。");

	int positionID = availableID[availableID.size() - 1];
	availableID.pop_back();

	mapUsedID.insert(std::pair<int, int>(id, positionID));

	mergeTexture->AddTexture(id, 0, positionID * heightLimit, texture);
}

void AutoMergeTexture::RemoveTexture(int id)
{
	availableID.push_back(mapUsedID[id]);

	mapUsedID.erase(id);
	mergeTexture->RemoveTexture(id);
}

auto AutoMergeTexture::GetFirstAvailableID() -> int
{
	if (availableID.size() == 0) return -1;

	return availableID[availableID.size() - 1];
}

auto AutoMergeTexture::GetMergeTexture() -> MergeTexture *
{
	return mergeTexture;
}
