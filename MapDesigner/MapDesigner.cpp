#include "MapDesigner.hpp"

#include "TextObject.hpp"
#include "Helper.hpp"

#define CAMERA_MOVE_SPEED 200.0f
#define CAMERA_TIMES_LIMIT 2.0f
#define CAMERA_WHEEL_SPEED 0.1f

#define MAX_CHAR_COUNT 1000

void MapDesigner::CreateObject()
{
	commandArea = new CommandArea("CommandArea", this);

	commandArea->SetSize(WINDOW_WIDTH - RENDER_OBJECT_AREA_WIDTH + 1, COMMAND_AREA_HEIGHT);
	commandArea->SetPosition(0, WINDOW_HEIGHT - commandArea->GetHeight());
	commandArea->SetBorderWidth(1);
	commandArea->SetBorderColor(0, 0, 0);
	commandArea->SetText("");

	world->RegisterUIObject(commandArea);

	renderObjectArea = new RenderObjectArea("RenderObjectArea", this);
	
	renderObjectArea->SetSize(RENDER_OBJECT_AREA_WIDTH, WINDOW_HEIGHT);
	renderObjectArea->SetPosition(WINDOW_WIDTH - renderObjectArea->GetWidth(), 0);
	renderObjectArea->SetBorderWidth(1);
	renderObjectArea->SetBorderColor(0, 0, 0);

	world->RegisterUIObject(renderObjectArea);
}

void MapDesigner::CreateTextureManager()
{
	textureManager = new TextureManager(application);

	textMergeTexture = new AutoMergeTexture(application,
		TEXT_AUTO_MERGETEXTURE_WIDTHLIMIT, TEXT_AUTO_MERGETEXTURE_HEIGHTLIMIT, TEXT_AUTO_MERGETEXTURE_COUNT,
		PixelFormat::A8);

	defaultMergeTexture = new AutoMergeTexture(application,
		100, 100, 5, PixelFormat::R8G8B8A8);

	defaultMergeTexture->AddTexture(DEFAULT_MAP_RENDER_OBJECT_ID,
		resourceManager->dataManager->RegisterTexture(DEFAULT_MAP_RENDER_OBJECT_FILE));
	defaultMergeTexture->AddTexture(DEFAULT_BACK_GROUND_ID,
		resourceManager->dataManager->RegisterTexture(DEFAULT_BACK_GROUND_FILE));

	textureManager->AddMergeTexture(0, resourceManager->GetMergeTexture());
	textureManager->AddMergeTexture(1, textMergeTexture->GetMergeTexture());
	textureManager->AddMergeTexture(2, defaultMergeTexture->GetMergeTexture());

	world->SetTextureManager(textureManager);
}

void MapDesigner::DestoryObject()
{
	Utility::Delete(commandArea);
	Utility::Delete(renderObjectArea);
}

void MapDesigner::DestoryTextureManager()
{
	Utility::Delete(textMergeTexture);
	Utility::Delete(defaultMergeTexture);

	Utility::Delete(textureManager);
}

void MapDesigner::LoadConfig()
{
	std::ifstream file;

	file.open(GLOBAL_CONFIG_FILE);

	if (!file) return;

	char data[MAX_CHAR_COUNT];

	while (file.getline(data, MAX_CHAR_COUNT)) {
		auto parameters = Helper::SpiltString(data);

		if (parameters.size() != 2) continue;

		parameters[0] = Helper::ToUpperString(parameters[0]);

		if (parameters[0] == MAP_FILE) 
			mapManager->OpenMap(parameters[1]);

		if (parameters[0] == RESOURCE_CONFIG_FILE)
			resourceManager->OpenResourceConfig(parameters[1]);

		if (parameters[0] == SSAA_FILE) {
			if (Helper::IsNumber(parameters[1]) == false) ERROR_RETURN("配置文件的SSAA抗锯齿倍数错误，不是整数。");

			ssaa = (float)Helper::StringToInt(parameters[1]);
		}
	}

	file.close();
}

void MapDesigner::SetMap()
{
	if (mapManager->map == nullptr) return;

	world->SetWorldMap(mapManager->map);

	for (auto it = operations.begin(); it != operations.end(); it++) 
		Utility::Delete(it->nextMapData);

	operations.clear();
}

void MapDesigner::SetResourceConfig()
{
	if (resourceManager->resources.size() == 0) return;

	renderObjectArea->ClearRenderObject();

	for (auto it = resourceManager->resources.begin(); it != resourceManager->resources.end(); it++)
		renderObjectArea->AddRenderObject(it->renderObjectID);

	textureManager->AddMergeTexture(0, resourceManager->mergeTexture);
}

void MapDesigner::SetMapData(int mousePositionX, int mousePositionY)
{

	if (mapManager->map == nullptr) return;

	auto cameraRect = mainCamera->GetRectangle();

	float x = ((float)mousePositionX / WINDOW_WIDTH) * (cameraRect.right - cameraRect.left) + cameraRect.left;
	float y = ((float)mousePositionY / WINDOW_HEIGHT) * (cameraRect.bottom - cameraRect.top) + cameraRect.top;

	auto index = mapManager->map->GetWorldMapDataIndex(x, y);

	if (index == mapManager->map->InvalidLocation()) return;
		
	Operation record;

	record.x = index.first;
	record.y = index.second;
	record.lastMapData = mapManager->map->GetMapData(index.first, index.second);
	record.nextMapData = new MapData();

	record.nextMapData->RenderObjectID = RenderObjectItem::GetFocusRenderObjectID();
	
	mapManager->map->SetMapData(index.first, index.second, record.nextMapData);

	operations.push_back(record);
}

void MapDesigner::BackSetMapData()
{
	if (operations.size() == 0) return;

	auto record = operations[operations.size() - 1];

	mapManager->map->SetMapData(record.x, record.y, record.lastMapData);

	Utility::Delete(record.nextMapData);

	operations.pop_back();
}

void MapDesigner::OnUpdate(void * sender)
{
	auto application = (ApplicationEx*)sender;
	auto mapDesigner = application->mapDesigner;

	application->SetWindow(mapDesigner->name + " FPS: " + Utility::ToString(application->GetFramePerSecond()), WINDOW_WIDTH, WINDOW_HEIGHT);

	auto deltaTime = application->GetDeltaTime();

	glm::vec2 translate = glm::vec2(0, 0);

	if (Input::GetKeyCodeDown(KeyCode::A) == true)
		translate.x -= 1;
	if (Input::GetKeyCodeDown(KeyCode::D) == true)
		translate.x += 1;
	if (Input::GetKeyCodeDown(KeyCode::S) == true)
		translate.y += 1;
	if (Input::GetKeyCodeDown(KeyCode::W) == true)
		translate.y -= 1;

	if (translate != glm::vec2(0, 0) && mapDesigner->commandArea->IsEnableRead() == false) {

		translate = glm::normalize(translate) * CAMERA_MOVE_SPEED * deltaTime * mapDesigner->mainCameraTimes;

		mapDesigner->mainCamera->Move(translate);
	}
}

void MapDesigner::OnKeyDownEvent(void * sender, Events::KeyClickEvent * eventArg)
{
	if (eventArg->isDown == false) return;

	auto application = (ApplicationEx*)sender;
	auto mapDesigner = application->mapDesigner;

	if (eventArg->keyCode == KeyCode::Tab)
		mapDesigner->commandArea->EnableRead(!mapDesigner->commandArea->IsEnableRead());

	if (eventArg->keyCode == KeyCode::Z && Input::GetKeyCodeDown(KeyCode::ControlKey) == true)
		mapDesigner->BackSetMapData();

	mapDesigner->commandArea->OnReadCommand(mapDesigner->commandArea, eventArg);
}

void MapDesigner::OnMouseDownEvent(void * sender, Events::MouseClickEvent * eventArg)
{
	if (eventArg->isDown == false) return;

	auto application = (ApplicationEx*)sender;
	auto mapDesigner = application->mapDesigner;
	auto commandArea = mapDesigner->commandArea;

	if (eventArg->button == Events::MouseButton::Left) {

		auto rect = PixelWorldEngine::RectangleF(commandArea->GetPositionX(),
			commandArea->GetPositionY(),
			commandArea->GetPositionX() + commandArea->GetWidth(),
			commandArea->GetPositionY() + commandArea->GetHeight());

		if (eventArg->x >= rect.left && eventArg->x <= rect.right &&
			eventArg->y >= rect.top && eventArg->y <= rect.bottom)
			commandArea->EnableRead(true);
		else commandArea->EnableRead(false);


		if (eventArg->x >= 0 && eventArg->x <= WINDOW_WIDTH - RENDER_OBJECT_AREA_WIDTH &&
			eventArg->y >= 0 && eventArg->y <= WINDOW_HEIGHT - COMMAND_AREA_HEIGHT) {

			mapDesigner->SetMapData(eventArg->x, eventArg->y);
		}

	}
}

void MapDesigner::OnMouseWheel(void * sender, Events::MouseWheelEvent * eventArg)
{
	auto application = (ApplicationEx*)sender;
	auto mapDesigner = application->mapDesigner;

	if (eventArg->x >= 0 && eventArg->x <= WINDOW_WIDTH - RENDER_OBJECT_AREA_WIDTH &&
		eventArg->y >= 0 && eventArg->y <= WINDOW_HEIGHT - COMMAND_AREA_HEIGHT) {

		float offset = -((short)eventArg->offset) * CAMERA_WHEEL_SPEED * mapDesigner->application->GetDeltaTime();

		mapDesigner->mainCameraTimes = Utility::Limit(mapDesigner->mainCameraTimes + offset, 1.0f, CAMERA_TIMES_LIMIT);

		auto cameraRect = mapDesigner->mainCamera->GetRectangle();

		float positionX = (cameraRect.right - cameraRect.left) * 0.5f + cameraRect.left;
		float positionY = (cameraRect.bottom - cameraRect.top) * 0.5f + cameraRect.top;

		float halfWidth = WINDOW_WIDTH * 0.5f * mapDesigner->mainCameraTimes;
		float halfHeight = WINDOW_HEIGHT * 0.5f * mapDesigner->mainCameraTimes;

		mapDesigner->mainCamera->SetFocus(positionX, positionY,
			PixelWorldEngine::RectangleF(halfWidth, halfHeight, halfWidth, halfHeight));
	}
}

MapDesigner::MapDesigner()
{
	application = new ApplicationEx(name, this);

	world = new PixelWorld(name, application);

	mapManager = new MapManager();
	resourceManager = new ResourceManager(application);

	mainCamera = new Camera(PixelWorldEngine::RectangleF(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	mainCameraTimes = 1.0f;

	ssaa = SSAA;
}

MapDesigner::~MapDesigner()
{
	DestoryObject();

	DestoryTextureManager();

	Utility::Delete(mainCamera);
	Utility::Delete(world);
	Utility::Delete(mapManager);
	Utility::Delete(resourceManager);
	Utility::Delete(application);
}

void MapDesigner::Initialize()
{
	LoadConfig();

	CreateTextureManager();

	TextObject::Create(textMergeTexture);

	CreateObject();
	
	world->SetResolution(WINDOW_WIDTH, WINDOW_HEIGHT, ssaa);
	world->SetCamera(mainCamera);

	application->SetWorld(world);

	application->Update.push_back(OnUpdate);
	application->KeyClick.push_back(OnKeyDownEvent);
	application->MouseClick.push_back(OnMouseDownEvent);
	application->MouseWheel.push_back(OnMouseWheel);

	application->MakeWindow(name, WINDOW_WIDTH, WINDOW_HEIGHT);
	application->ShowWindow();

	SetMap();
	SetResourceConfig();
}

void MapDesigner::RunLoop()
{
	application->RunLoop();
}
