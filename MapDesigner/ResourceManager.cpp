#include "ResourceManager.hpp"

#define READ_RECT(rect) rect.left >> rect.top >> rect.right >> rect.bottom
#define WRITE_RECT(rect) rect.left << " " << rect.top << " " << rect.right << " " << rect.bottom

auto ResourceManager::CreateMergeTextureFromResource(ResourceManager* manager) -> MergeTexture *
{
	int maxHeight = 0;
	int needWidth = 0;

	std::map<int, int> finder;

	for (auto it = manager->resources.begin(); it != manager->resources.end(); it++) {
		maxHeight = Utility::Max(maxHeight, it->rect.bottom);
		needWidth += it->rect.right;

		if (finder.count(it->renderObjectID) != 0) ERROR_RETURN_WITH_VALUE("重复的渲染编号。", nullptr);

		finder.insert(std::pair<int, int>(it->renderObjectID, it->renderObjectID));
	}

	auto mergeTexture = new MergeTexture(manager->application, needWidth, maxHeight, PixelFormat::R8G8B8A8);

	needWidth = 0;

	for (auto it = manager->resources.begin(); it != manager->resources.end(); it++) {
		auto texture = manager->dataManager->RegisterTexture(it->fileName);

		auto subTexture = new Texture2D(texture, it->rect);

		mergeTexture->AddTexture(it->renderObjectID, needWidth, 0, subTexture);
		
		needWidth += subTexture->GetWidth();

		Utility::Delete(subTexture);
	}

	return mergeTexture;
}

ResourceManager::ResourceManager(Application * app)
{
	application = app;

	dataManager = new DataManager(app);

	mergeTexture = nullptr;
}

ResourceManager::~ResourceManager()
{
	Utility::Delete(mergeTexture);
	Utility::Delete(dataManager);
}

void ResourceManager::CreateResourceConfigFromFile(std::string fileName, int widthCount, int heightCount, int baseRenderObjectID)
{
	CloseResourceConfig();

	auto texture = dataManager->RegisterTexture(fileName);

	resources.resize(widthCount * heightCount);

	int count = 0;
	int width = texture->GetWidth() / widthCount;
	int height = texture->GetHeight() / heightCount;

	for (int x = 0; x < widthCount; x++) {
		for (int y = 0; y < heightCount; y++) {
			resources[count].fileName = fileName;
			resources[count].renderObjectID = baseRenderObjectID + count;
			resources[count].rect = PixelWorldEngine::Rectangle(x * width, y * height, x * width + width, y * height + height);

			count++;
		}
	}

	mergeTexture = CreateMergeTextureFromResource(this);
}

void ResourceManager::OpenResourceConfig(std::string fileName)
{
	CloseResourceConfig();

	std::ifstream file;

	file.open(fileName);

	if (!file) ERROR_RETURN("打开的资源配置文件路径不存在。");

	Resource readData;

	while (file >> readData.fileName) {
		file >> readData.renderObjectID;

		if (readData.renderObjectID >= TEXT_RENDEROBJECTID_BASE)
			ERROR_RETURN("配置资源中使用的渲染编号是文本渲染编号范围，请缩小渲染编号的值。");

		file >> READ_RECT(readData.rect);

		resources.push_back(readData);
	}

	file.close();

	mergeTexture = CreateMergeTextureFromResource(this);
}

void ResourceManager::SaveResourceConfig(std::string fileName)
{
	if (resources.size() == 0) ERROR_RETURN("没有可以保存的资源配置。");

	std::ofstream file;

	file.open(fileName);

	for (auto it = resources.begin(); it != resources.end(); it++) {
		file << it->fileName << " ";

		file << it->renderObjectID << " ";

		file << WRITE_RECT(it->rect) << std::endl;
	}

	file.close();
}

void ResourceManager::MergeResourceConfig(std::string fileName)
{
	auto currentResources = resources;

	CloseResourceConfig();

	for (auto it = currentResources.begin(); it != currentResources.end(); it++)
		resources.push_back(*it);

	std::ifstream file;

	file.open(fileName);

	if (!file) ERROR_RETURN("打开的资源配置文件路径不存在。");

	Resource readData;

	while (file >> readData.fileName) {
		file >> readData.renderObjectID;

		if (readData.renderObjectID >= TEXT_RENDEROBJECTID_BASE)
			ERROR_RETURN("配置资源中使用的渲染编号是文本渲染编号范围，请缩小渲染编号的值。");

		file >> READ_RECT(readData.rect);

		resources.push_back(readData);
	}

	file.close();

	mergeTexture = CreateMergeTextureFromResource(this);
}

void ResourceManager::CloseResourceConfig()
{
	if (resources.size() == 0) return;

	resources.clear();
	Utility::Delete(mergeTexture);
}

auto ResourceManager::GetBigFont() -> Font *
{
	return dataManager->RegisterFont(DEFAULT_FONT, DEFAULT_FONT_BIGNAME, DEFAULT_FONT_BIGSIZE);
}

auto ResourceManager::GetNormalFont() -> Font *
{
	return dataManager->RegisterFont(DEFAULT_FONT, DEFAULT_FONT_NORMALNAME, DEFAULT_FONT_NORMALSIZE);
}

auto ResourceManager::GetMergeTexture() -> MergeTexture *
{
	return mergeTexture;
}
