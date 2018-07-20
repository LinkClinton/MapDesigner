#include "CommandArea.hpp"
#include "MapDesigner.hpp"
#include "Helper.hpp"

#define TEXT_OFFSET 10

#define ERROR_COMMAND ERROR_RETURN("错误的指令格式。")

auto CommandArea::IsLegalChar(char code) -> bool
{
	if (code >= '0' && code <= '9') return true;
	if (code >= 'A' && code <= 'Z') return true;
	if (code == ' ') return true;
	if (code == (char)KeyCode::OemPeriod) return true;

	return false;
}

auto CommandArea::GetLegalChar(char code) -> char
{
	bool need = Input::IsCapsLock() ^ Input::GetKeyCodeDown(KeyCode::ShiftKey);

	if (code >= '0' && code <= '9') return code;
	if (code >= 'A' && code <= 'Z' && need == true) return code;
	if (code >= 'A' && code <= 'Z' && need == false) return code + 'a' - 'A';
	if (code == ' ') return code;
	if (code == (char)KeyCode::OemPeriod) return '.';

	return ' ';
}

CommandArea::CommandArea(std::string name, MapDesigner* MapDesigner)
	: UIObject(name)
{
	isEnableRead = false;

	SetRenderObjectID(DEFAULT_BACK_GROUND_ID);
	SetOpacity(DEFAULT_BACK_GROUND_OPACITY);

	mapDesigner = MapDesigner;

	textObject = new TextObject("Command");
	textObject->SetPositionX(TEXT_OFFSET);
	textObject->SetEffectColor(0, 0, 0);

	RegisterUIObject(textObject);
}

void CommandArea::SendCommand(std::string command)
{
	SetText("");

	auto parameters = Helper::SpiltString(command);

	if (parameters.size() == 0) ERROR_COMMAND;

	parameters[0] = Helper::ToUpperString(parameters[0]);

	if (parameters[0] == COMMAND_CREATE_MAP) {

		if (parameters.size() != 4) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;
		if (Helper::IsNumber(parameters[2]) == false) ERROR_COMMAND;
		if (Helper::IsNumber(parameters[3]) == false) ERROR_COMMAND;

		mapDesigner->mapManager->CreateMap(parameters[1], Helper::StringToInt(parameters[2]),
			Helper::StringToInt(parameters[3]));

		mapDesigner->SetMap();
	}

	if (parameters[0] == COMMAND_OPEN_MAP) {

		if (parameters.size() != 2) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;

		mapDesigner->mapManager->OpenMap(parameters[1]);

		mapDesigner->SetMap();
	}

	if (parameters[0] == COMMAND_SAVE_MAP) {

		if (parameters.size() != 2) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;

		mapDesigner->mapManager->SaveMap(parameters[1]);
	}

	if (parameters[0] == COMMAND_CREATE_CONFIG) {

		if (parameters.size() != 5) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;
		if (Helper::IsNumber(parameters[2]) == false) ERROR_COMMAND;
		if (Helper::IsNumber(parameters[3]) == false) ERROR_COMMAND;
		if (Helper::IsNumber(parameters[4]) == false) ERROR_COMMAND;

		auto resourceManager = mapDesigner->resourceManager;

		resourceManager->CreateResourceConfigFromFile(parameters[1],
			Helper::StringToInt(parameters[2]), Helper::StringToInt(parameters[3]), Helper::StringToInt(parameters[4]));

		mapDesigner->SetResourceConfig();
	}

	if (parameters[0] == COMMAND_OPEN_CONFIG) {

		if (parameters.size() != 2) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;

		auto resourceManager = mapDesigner->resourceManager;

		resourceManager->OpenResourceConfig(parameters[1]);

		mapDesigner->SetResourceConfig();
	}

	if (parameters[0] == COMMAND_SAVE_CONFIG) {

		if (parameters.size() != 2) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;

		mapDesigner->resourceManager->SaveResourceConfig(parameters[1]);
	}

	if (parameters[0] == COMMAND_MERGE_CONFIG) {

		if (parameters.size() != 2) ERROR_COMMAND;
		if (parameters[1].length() == 0) ERROR_COMMAND;

		auto resourceManager = mapDesigner->resourceManager;

		resourceManager->MergeResourceConfig(parameters[1]);

		mapDesigner->SetResourceConfig();
	}
}

void CommandArea::OnReadCommand(void * sender, Events::KeyClickEvent * eventArg)
{
	if (eventArg->isDown == false || isEnableRead == false) return;

	if (eventArg->keyCode == KeyCode::Enter) {
		SendCommand(text);
		
		return;
	}

	if (eventArg->keyCode == KeyCode::Back) {
		if (text.size() == 0) return;

		text.pop_back();
	}

	if (text.size() == COMMAND_LENGTH_LIMIT) return;

	if (IsLegalChar((char)eventArg->keyCode) == true)
		text.push_back(GetLegalChar((char)eventArg->keyCode));

	SetText(text);
}

void CommandArea::SetText(std::string Text)
{
	text = Text;

	textObject->SetWidth(width);
	textObject->SetText(text + COMMAND_SUFFIX, mapDesigner->resourceManager->GetBigFont());
	textObject->SetPositionY((height - textObject->GetHeight())* 0.5f);
}

void CommandArea::EnableRead(bool state)
{
	isEnableRead = state;
}

auto CommandArea::IsEnableRead() -> bool
{
	return isEnableRead;
}
