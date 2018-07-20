#include "RenderObjectArea.hpp"
#include "MapDesigner.hpp"
#include "Helper.hpp"

#define VIEW_OBJECT_WIDTH width * 0.8f
#define TEXT_OBJECT_WIDTH VIEW_OBJECT_WIDTH
#define VIEW_OBJECT_OFFSET height * 0.1f

#define RENDER_OBJECT_ITEM_WIDTH width * 0.9f
#define RENDER_OBJECT_ITEM_HEIGHT height * 0.10f

#define WHEEL_SPEED 75.0f

RenderObjectItem* RenderObjectItem::focusObject = nullptr;

void RenderObjectArea::OnMouseWheel(void * sender, Events::MouseWheelEvent * eventArg)
{
	float offset = - ((short)eventArg->offset) * WHEEL_SPEED * mapDesigner->application->GetDeltaTime();
	float oldYOffset = yOffset;

	float yLimit = renderObjectItems.size() * RENDER_OBJECT_ITEM_HEIGHT - WINDOW_HEIGHT;

	yOffset += offset;
	yOffset = Utility::Limit(yOffset, 0.0f, yLimit);

	for (auto it = renderObjectItems.begin(); it != renderObjectItems.end(); it++) {
		auto renderObject = *it;

		renderObject->SetPositionY(renderObject->GetPositionY() + oldYOffset - yOffset);
	}
}

RenderObjectArea::RenderObjectArea(std::string name, MapDesigner * MapDesigner)
	: UIObject(name)
{
	mapDesigner = MapDesigner;

	yOffset = 0;

	SetRenderObjectID(DEFAULT_BACK_GROUND_ID);
	SetOpacity(DEFAULT_BACK_GROUND_OPACITY);

	AddRenderObject(DEFAULT_MAP_RENDER_OBJECT_ID);
}

void RenderObjectArea::AddRenderObject(int renderObjectID)
{
	auto renderObject = new RenderObjectItem("RenderObjectItem" + Utility::ToString(renderObjectID),
		RENDER_OBJECT_ITEM_WIDTH, RENDER_OBJECT_ITEM_HEIGHT, renderObjectID, mapDesigner->resourceManager->GetNormalFont());

	renderObject->SetPosition((width - renderObject->GetWidth()) * 0.5f,
		renderObjectItems.size() * (RENDER_OBJECT_ITEM_HEIGHT) - yOffset);

	renderObjectItems.push_back(renderObject);

	RegisterUIObject(renderObject);
}

void RenderObjectArea::ClearRenderObject()
{
	for (auto it = renderObjectItems.begin(); it != renderObjectItems.end(); it++) {	
		auto renderObject = *it;

		UnRegisterUIObject(renderObject);

		Utility::Delete(renderObject);
	}

	renderObjectItems.clear();

	AddRenderObject(DEFAULT_MAP_RENDER_OBJECT_ID);
}

void RenderObjectItem::OnMouseClick(void * sender, Events::MouseClickEvent * eventArg)
{
	if (eventArg->isDown == true && eventArg->button == Events::MouseButton::Left) {
		if (focusObject != nullptr) 
			focusObject->SetBorderWidth(0);

		focusObject = this;

		SetBorderWidth(1);
		SetBorderColor(0, 0, 0);
	}
}

RenderObjectItem::RenderObjectItem(std::string name, float Width, float Height, int RenderObjectID, Font* font)
	: UIObject(name, 0, 0, Width, Height)
{
	viewObjectRenderObjectID = RenderObjectID;

	viewObject = new UIObject("ViewObject");

	viewObject->SetSize(VIEW_OBJECT_WIDTH, VIEW_OBJECT_WIDTH);
	viewObject->SetPosition((width - viewObject->GetWidth()) * 0.5f, VIEW_OBJECT_OFFSET);
	viewObject->SetRenderObjectID(RenderObjectID);

	textObject = new TextObject("TextObject");

	textObject->SetWidth(TEXT_OBJECT_WIDTH);
	textObject->SetPosition((width - textObject->GetWidth()) * 0.5f, viewObject->GetHeight() + viewObject->GetPositionY());
	textObject->SetText(Utility::ToString(RenderObjectID), font);
	textObject->SetEffectColor(0, 0, 0);

	RegisterUIObject(viewObject);
	RegisterUIObject(textObject);
}

auto RenderObjectItem::GetFocusRenderObjectID() -> int
{
	if (focusObject == nullptr) return DEFAULT_MAP_RENDER_OBJECT_ID;

	return focusObject->viewObjectRenderObjectID;
}
