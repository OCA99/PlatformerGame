#include "GuiManager.h"

#include "App.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Textures.h"

GuiManager::GuiManager() : Module()
{
	name.Create("guimanager");
}

GuiManager::~GuiManager()
{}

bool GuiManager::Awake(pugi::xml_node& config)
{
	pugi::xml_node guimanagerPathN = config.child("guimanager");

	checkBoxPath = guimanagerPathN.attribute("checkbox").as_string();
	arrowMenuPath = guimanagerPathN.attribute("menuArrow").as_string();

	return true;
}

bool GuiManager::Start()
{

	checkBoxTex = app->tex->Load(checkBoxPath);
	arrowMenuTex = app->tex->Load(arrowMenuPath);

	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::PostUpdate()
{

	DrawAll();

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int x, int y, SDL_Rect bounds, int id)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		case GuiControlType::BUTTON: 
			control = new GuiButton(id, bounds, arrowMenuTex);
			control->SetObserver(app->scene);
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckBox(id, bounds, checkBoxTex);
			control->SetObserver(app->scene);
			break;
		case GuiControlType::SLIDER:
			/*control = new GuiSlider(id, bounds, "EXIT");
			control->SetObserver(app->scene);*/
			break;

		default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int i = controls.Find(entity);
	ListItem<GuiControl*>* c = controls.At(i);
	controls.Del(c);
}

void GuiManager::DestroyAllGuiControls()
{
	int u = controls.Count();

	for (int i = 0; i < u; i++)
	{
		controls.Del(controls.At(0));
	}
}

void GuiManager::AddGuiControl(GuiControl* entity)
{
	if (entity != nullptr) controls.Add(entity);
}

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		for (int i = 0; i < controls.Count(); i++)
		{
			controls[i]->Update(app->input, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		controls[i]->Draw(app->render);
	}
}