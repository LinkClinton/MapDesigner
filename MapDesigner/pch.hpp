#pragma once

#include <fstream>

#include <Application.hpp>
#include <Input.hpp>
#include <Text.hpp>

using namespace PixelWorldEngine;
using namespace PixelWorldEngine::Graphics;

#define ERROR_RETURN(x) { std::wcout << x << std::endl; return; }
#define ERROR_RETURN_WITH_VALUE(x, value)  { std::wcout << x << std::endl; return value; }

#define DEFAULT_MAP_BLOCK_SIZE 64

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define SSAA 2

#define AUTO_MERGETEXTURE_ADD 20

#define TEXT_RENDEROBJECTID_BASE 1000

#define TEXT_AUTO_MERGETEXTURE_WIDTHLIMIT 1280
#define TEXT_AUTO_MERGETEXTURE_HEIGHTLIMIT 50
#define TEXT_AUTO_MERGETEXTURE_COUNT 60

#define COMMAND_LENGTH_LIMIT 100
#define COMMAND_SUFFIX "_"

#define DEFAULT_FONT "Consola.ttf"

#define DEFAULT_FONT_BIGNAME "BigFont"
#define DEFAULT_FONT_NORMALNAME "NormalFont"

#define DEFAULT_FONT_BIGSIZE 20
#define DEFAULT_FONT_NORMALSIZE 17

#define DEFAULT_MAP_RENDER_OBJECT_ID -1
#define DEFAULT_BACK_GROUND_ID -2

#define DEFAULT_MAP_RENDER_OBJECT_FILE "DefaultBlock.jpg"
#define DEFAULT_BACK_GROUND_FILE "BackGround.jpg"
#define DEFAULT_BACK_GROUND_OPACITY 0.7f

#define GLOBAL_CONFIG_FILE "config"

#define MAP_FILE "MAP"
#define RESOURCE_CONFIG_FILE "RESOURCECONFIG"
#define SSAA_FILE "SSAA"