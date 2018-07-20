#include "TextObject.hpp"

AutoMergeTexture* TextObject::autoMergeTexture = nullptr;

void TextObject::FreeTextTexture()
{
	if (textTexture == nullptr) return;

	autoMergeTexture->RemoveTexture(renderObjectID);

	Utility::Delete(textTexture);
}

TextObject::TextObject(std::string name)
	: UIObject(name)
{
	text = "";
	textTexture = nullptr;

	font = nullptr;
}

TextObject::~TextObject()
{
	Utility::Delete(textTexture);
}

void TextObject::SetText(std::string localText, Font * localFont)
{
	if (text == localText) return;

	text = localText;

	if (localFont != nullptr) font = localFont;

	FreeTextTexture();
	
	textTexture = new Text(text, font, (int)width, 0);

	SetHeight((float)textTexture->GetHeight());

	renderObjectID = TEXT_RENDEROBJECTID_BASE + autoMergeTexture->GetFirstAvailableID();

	autoMergeTexture->AddTexture(renderObjectID, textTexture->GetTexture());
}

void TextObject::Create(AutoMergeTexture* mergeTexture)
{
	autoMergeTexture = mergeTexture;
}



