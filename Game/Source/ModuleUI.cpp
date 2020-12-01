#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "ModuleUI.h"
#include "Player.h"
#include "Scene.h"
#include "Input.h"
#include "Collisions.h"

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

	teleportArrowPath = uiPathN.attribute("teleportArrow").as_string();

	teleportCrossPath = uiPathN.attribute("teleportCross").as_string();

	livesTexturePath= uiPathN.attribute("livesTexturePath").as_string();

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
	boxGodMode = SDL_Rect({ 0, 510, 38, 30 });
	boxShowColl = SDL_Rect({ 38, 510, 38, 30 });

	drawTeleportText = false;

	teleportMapLevel1 = app->tex->Load(teleportMapPath1);
	teleportMapLevel2 = app->tex->Load(teleportMapPath2);

	teleportArrowTex = app->tex->Load(teleportArrowPath);
	
	teleportCrossTex = app->tex->Load(teleportCrossPath);

	livesTexture = app->tex->Load(livesTexturePath);
	livesRect = SDL_Rect({ 0,0,12,10 });
	extraLivesRect = SDL_Rect({ 12,0,12,10 });


	destinationCheckpoint = 0;	

	return ret;
}

// Update: draw background
bool ModuleUI::Update(float dt)
{
	switch (currentLevel)
	{
	case 1:
		arrow1.x = 92;
		arrow1.y = 190;

		arrow2.x = 211;
		arrow2.y = 106;

		arrow3.x = 325;
		arrow3.y = 106;

		crossPos1.x = 203;
		crossPos1.y = 116;

		crossPos2.x = 317;
		crossPos2.y = 116;

		renderedMap = teleportMapLevel1;

		break;

	case 2:
		arrow1.x = 92;
		arrow1.y = 165;

		arrow2.x = 224;
		arrow2.y = 114;

		arrow3.x = 252;
		arrow3.y = 188;

		crossPos1.x = 216;
		crossPos1.y = 124;

		crossPos2.x = 245;
		crossPos2.y = 197;

		renderedMap = teleportMapLevel2;

		break;

	default:
		break;
	}

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

			switch (destinationCheckpoint)
			{
			case 0:
				renderedArrowPos = arrow1;
				break;

			case 1:
				renderedArrowPos = arrow2;
				break;

			case 2:
				renderedArrowPos = arrow3;
				break;

			default:
				break;
			}

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				switch (destinationCheckpoint)
				{
				case 0:
					app->player->position = app->player->initialPosition;
					app->player->collider->SetPos(app->player->position.x, app->player->position.y);
					break;

				case 1:
					if (!app->player->unlockedChekpoint1)
						break;

					app->player->position = app->player->checkpoint1Position;
					app->player->collider->SetPos(app->player->position.x, app->player->position.y);
					drawTeleportMap = false;
					break;

				case 2:
					if (!app->player->unlockedChekpoint2)
						break;

					app->player->position = app->player->checkpoint2Position;
					app->player->collider->SetPos(app->player->position.x, app->player->position.y);
					drawTeleportMap = false;
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

	toLoadBar = 24 * app->player->cooldown / app->player->maxCooldown;

	boxCooldown = SDL_Rect({ (int)app->player->position.x, (int)app->player->position.y - 10, toLoadBar, 1 });
	boxOuterCooldown = SDL_Rect({ (int)app->player->position.x - 1, (int)app->player->position.y - 11, 26, 3 });

	return true;
}

bool ModuleUI::PostUpdate()
{
	OPTICK_EVENT("UIPostUpdate", Optick::Category::UI);

	app->render->DrawRectangle(box, 33, 31, 48, 255, true, false);

	if (app->player->cooldown < app->player->maxCooldown)
	{
		app->render->DrawRectangle(boxOuterCooldown, 33, 31, 48, 255, true, true);
		app->render->DrawRectangle(boxCooldown, 71, 174, 72, 255, false, true);
	}

	int uiposx = 10;
	BlitText(uiposx, 5, font, "LEVEL", false);
	IntToString(shortNumberText, currentLevel, 2);
	BlitText(uiposx + 55, 5, font, shortNumberText, false);

	BlitText(uiposx + 90, 5, font, "HEALTH", false);
	
	for (int i = 0; i < app->player->health; i++)
	{
		if (i < 3)
		{
			app->render->DrawTexture(livesTexture, uiposx + 150 + (i * 15), 3, &livesRect, 0, 0, 0, 0, false);
		}
		else if (i < 6)
		{
			app->render->DrawTexture(livesTexture, uiposx + 150 + (i * 15), 3, &extraLivesRect, 0, 0, 0, 0, false);
		}
	}

	if (app->player->godMode)
	{
		app->render->DrawRectangle(boxGodMode, 33, 31, 48, 255, true, false);
		BlitText(5, 259, font, "G", false);
	}

	if (app->collisions->showColliders)
	{
		app->render->DrawRectangle(boxShowColl, 33, 31, 48, 255, true, false);
		BlitText(24, 259, font, "C", false);
	}

	BlitText(uiposx + 320, 5, font, "SCORE", false);
	IntToDynamicString(scoreText, score);
	BlitText(uiposx + 375, 5, font, scoreText, false);

	if (drawTeleportText && !drawTeleportMap)
		BlitText(app->player->position.x - 70, app->player->position.y + 30, font, "PRESS T TO TELEPORT", true);

	if (canDrawSecret)
	{
		BlitText(app->player->position.x - 70, app->player->position.y + 30, font, "ONLY A GOD CAN REACH", true);
		BlitText(app->player->position.x - 70, app->player->position.y + 40, font, "MIDAS' HALL", true);
	}


	if (drawTeleportMap)
	{
		app->render->DrawTexture(renderedMap, 0, 0, NULL, 0, 0, 0, 0, false);
		app->render->DrawTexture(teleportArrowTex, renderedArrowPos.x, renderedArrowPos.y, NULL, 0, 0, 0, 0, false);

		if (!app->player->unlockedChekpoint1)
			app->render->DrawTexture(teleportCrossTex, crossPos1.x, crossPos1.y, NULL, 0, 0, 0, 0, false);
		
		if (!app->player->unlockedChekpoint2)
			app->render->DrawTexture(teleportCrossTex, crossPos2.x, crossPos2.y, NULL, 0, 0, 0, 0, false);
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