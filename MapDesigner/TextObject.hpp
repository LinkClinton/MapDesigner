#pragma once

#include "pch.hpp"
#include "AutoMergeTexture.hpp"

class TextObject : public UIObject {
private:
	Font* font;

	std::string text;
	Text* textTexture;

	static AutoMergeTexture* autoMergeTexture;

	void FreeTextTexture();
public:
	TextObject(std::string name);

	~TextObject();

	void SetText(std::string text, Font* font);

	static void Create(AutoMergeTexture* autoMergeTexture);
};