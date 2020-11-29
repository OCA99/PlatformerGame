#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "ModuleUI.h"
#include "Player.h"
#include "Scene.h"
#include "Input.h"

#include "Optick/include/optick.h"


ModuleUI::ModuleUI() : Module()
{
	name.Create("ui");
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Awake(pugi::xml_node& config)
{
	bool ret = true;

	pugi::xml_node uiPathN = config.child("ui");

	fontPath = uiPathN.attribute("fontPath").as_string();

	teleportMapPath1 = uiPathN.attribute("teleportMapPath1").as_string();
	teleportMapPath2 = uiPathN.attribute("teleportMapPath2").as_string();

	score = 0;

	currentLevel = 1;

	return ret;
}

bool ModuleUI::Start()
{
	bool ret = true;

	char lookupTable[] = { "0123456789.,\"!'-�ABCDEFGHIJKLMNOPQRSTUVWXYZ.    " };

	font = Load(fontPath, lookupTable, 3);

	box = SDL_Rect({ 0, 0, app->render->camera.w, 30 });

	drawTeleportText = false;

	teleportMapLevel1 = app->tex->Load(teleportMapPath1);
	teleportMapLevel2 = app->tex->Load(teleportMapPath2);

	destinationCheckpoint = 0;

	return ret;
}

// Update: draw background
bool ModuleUI::Update(float dt)
{
	if (canDrawMap)
	{
		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			if (drawTeleportMap)
			{
				drawTeleportMap = false;
			}
			else
			{
				drawTeleportMap = true;
			}
		}
		if (drawTeleportMap)
		{
			if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			{
				destinationCheckpoint++;
				if (destinationCheckpoint > 2)
				{
					destinationCheckpoint = 2;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			{
				destinationCheckpoint--;
				if (destinationCheckpoint < 0)
				{
					destinationCheckpoint = 0;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				switch (destinationCheckpoint)
				{
				case 0:
					app->player->position = app->player->initialPosition;
					break;

				case 1:
					if (!app->player->unlockedChekpoint1)
						break;
					app->player->position = app->player->checkpoint1Position;
					break;

				case 2:
					if (!app->player->unlockedChekpoint2)
						break;
					app->player->position = app->player->checkpoint2Position;
					break;

				default:
					break;
				}
			}
		}
	}

	if (!drawTeleportText)
		drawTeleportMap = false;

	canDrawMap = false;
	canDrawSecret = false;
	drawTeleportText = false;

	return true;
}

bool ModuleUI::PostUpdate()
{
	OPTICK_EVENT("UIPostUpdate", Optick::Category::UI);

	app->render->DrawRectangle(box, 33, 31, 48, 255, true, false);

	int uiposx = 10;
	BlitText(uiposx, 5, font, "LEVEL", false);
	IntToString(shortNumberText, currentLevel, 2);
	BlitText(uiposx + 55, 5, font, shortNumberText, false);

	BlitText(uiposx + 90, 5, font, "HEALTH", false);
	IntToString(shortNumberText, app->player->health, 2);
	BlitText(uiposx + 155, 5, font, shortNumberText, false);

	BlitText(uiposx + 320, 5, font, "SCORE", false);
	IntToDynamicString(scoreText, score);
	BlitText(uiposx + 375, 5, font, scoreText, false);

	if (drawTeleportText && !drawTeleportMap)
		BlitText(app->player->position.x - 70, app->player->position.y + 30, font, "PRESS T TO TELEPORT", true);

	if (canDrawSecret)
	{
		BlitText(app->player->position.x - 70, app->player->position.y + 30, font, "ONLY A GOD CAN REACH", true);
		BlitText(app->player->position.x - 70, app->player->position.y + 40, font, "MIDA'S HALL", true);
	}


	if (drawTeleportMap)
	{
		switch (currentLevel)
		{
		case 1:
			app->render->DrawTexture(teleportMapLevel1, 0, 0, NULL, 0, 0, 0, 0, false);
			break;

		case 2:
			app->render->DrawTexture(teleportMapLevel2, 0, 0, NULL, 0, 0, 0, 0, false);
			break;

		default:
			break;
		}
	}

	return true;
}

bool ModuleUI::CleanUp()
{

	bool ret = true;

	UnLoad(font);

	return ret;
}


// Load new texture from file path
int ModuleUI::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// TODO 1: Finish storing font data

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;


	k++;

	return id;
}

void ModuleUI::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		app->tex->UnLoad(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
	}
	k--;
}

void ModuleUI::BlitText(int x, int y, int font_id, const char* text, bool useCamera) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr)
	{
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 1.0f, 0.0f, INT_MAX, INT_MAX, useCamera);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}

void ModuleUI::IntToDynamicString(char* buffer, int k) {

	for (int i = 0; i < DYNAMIC_TEXT_LEN; i++) {
		buffer[i] = '0';
	}

	buffer[DYNAMIC_TEXT_LEN] = 0;

	int i = DYNAMIC_TEXT_LEN - 1;
	while (k != 0) {
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}

void ModuleUI::IntToString(char* buffer, int k, int length) {

	for (int i = 0; i < length; i++) {
		buffer[i] = '0';
	}

	buffer[length] = 0;

	int i = length - 1;
	while (k != 0) {
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}