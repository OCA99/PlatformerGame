#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Animation.h"
#include "Textures.h"
#include "App.h"

#include "Log.h"

#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL.h"

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	~TileSet()
	{
		app->tex->UnLoad(texture);
	}

	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	// L04: TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L06: TODO 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		SString type;
		int value;
	};


	~Properties()
	{

	}

	List<Property*> propertyList;

	int GetProperty(const char* name, int defaultValue = 1) const;

	void SetProperty(const char* name, int value);

};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		//...
		return 0;
	}
};

// L03: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	int nextObjectId;
	int nextLayerId;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;
	List<MapLayer*> maplayers;
	Properties properties;

	// L04: TODO 2: Add a list/array of layers to the map
};

class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path, bool loadEntities = true);

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	bool IntersectsWithMap(Collider* c, int direction = 0);
private:

	// L03: Methods to load all required map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties* properties);
	bool StoreID(pugi::xml_node& node, MapLayer* layer, int ID);


	bool CreateColliders();
	bool CreateEntities();
	void CreateWalkabilityMap();
	void CreatePathfindingWalkabilityMap();

public:

	// L03: DONE 1: Add your struct for map info
	MapData data;

private:

	const char* flagPath;
	SDL_Texture* flagTex = nullptr;

	Animation flagAnimation;

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;
	uchar* walkabilityMap = nullptr;
	uchar* pathfindingWalkabilityMap = nullptr;
};

#endif // __MAP_H__