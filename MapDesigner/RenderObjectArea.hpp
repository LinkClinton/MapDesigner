#pragma once

#include "pch.hpp"
#include "TextObject.hpp"

#define RENDER_OBJECT_AREA_WIDTH WINDOW_WIDTH * 0.05f

class MapDesigner;

class RenderObjectItem : public UIObject {
private:
	UIObject* viewObject;
	TextObject* textObject;

	int viewObjectRenderObjectID;

	static RenderObjectItem* focusObject;

	virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg)override;
public:
	RenderObjectItem(std::string name, float Width, float Height, int RenderObjectID, Font* font);

	static auto GetFocusRenderObjectID() -> int;
};

class RenderObjectArea : public UIObject {
private:
	MapDesigner* mapDesigner;

	std::vector<RenderObjectItem*> renderObjectItems;

	float yOffset;

	virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg)override;
public:
	RenderObjectArea(std::string name, MapDesigner* mapDesigner);

	void AddRenderObject(int renderObjectID);

	void ClearRenderObject();
};