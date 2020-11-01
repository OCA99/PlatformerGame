#ifndef __PARALLAX_H__
#define __PARALLAX_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class Parallax : public Module
{
public:
	Parallax();

	// Destructor
	virtual ~Parallax();

	//Called at the beginning of the application execution
	bool Awake(pugi::xml_node& config);

	//Called when the module is activated
	//By now we will consider all modules to be permanently active
	bool Start();

	//Called at the beginning of each application loop
	bool PreUpdate();

	//Called at the middle of each application loop
	bool Update(float dt);

	//Called at the end of each application loop
	bool PostUpdate();

	//Called at the end of the application
	bool CleanUp();

	void ParallaxBackground0();
	void ParallaxBackground1();
	void ParallaxBackground2();

	int xstart = 112;
	int ystart = 144;
	int parallaxw = 719;

	float x0 = 0;
	float x1 = 0;
	float x2 = 0;

	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* bgTexture2 = nullptr;
	SDL_Texture* bgTexture3 = nullptr;

	const char* bgTexturePath;
	const char* bgTexturePath2;
	const char* bgTexturePath3;

	bool enabled = false;

};

#endif //__PARALLAX_H__