#pragma once

#include "pch.hpp"
#include "TextObject.hpp"

#define COMMAND_CREATE_MAP "CREATEMAP"
#define COMMAND_OPEN_MAP "OPENMAP"
#define COMMAND_SAVE_MAP "SAVEMAP"

#define COMMAND_CREATE_CONFIG "CREATECONFIG"
#define COMMAND_OPEN_CONFIG "OPENCONFIG"
#define COMMAND_SAVE_CONFIG "SAVECONFIG"
#define COMMAND_MERGE_CONFIG "MERGECONFIG"

#define COMMAND_AREA_HEIGHT WINDOW_HEIGHT * 0.05f

class MapDesigner;

class CommandArea : public UIObject {
private:
	MapDesigner * mapDesigner;

	std::string text;

	TextObject* textObject;

	bool isEnableRead;

	auto IsLegalChar(char code) -> bool;

	auto GetLegalChar(char code) -> char;
public:
	CommandArea(std::string name, MapDesigner* mapDesigner);

	void SendCommand(std::string command);

	void OnReadCommand(void* sender, Events::KeyClickEvent* eventArg);

	void SetText(std::string text);

	void EnableRead(bool state);

	auto IsEnableRead() -> bool;
};