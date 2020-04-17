#pragma once
#include <d3d9.h>
#include"Textures.h"
#include "Sprite.h"
#include "Camera.h"
class Map
{
	Camera* camera;
	int rowMap, columnMap;
	int rowTileSet, columnTileSet;

	int totalTile;	//total tiles of tile set
	
	LPDIRECT3DTEXTURE9 tileSet;
	vector<LPSPRITE> tiles;
	int** tileMap;
	int screenWidth;
public:
	Map(int tileSetID,int rowMap,int columnMap, int rowTileSet,int  columnTileSet, int totalTile);
	~Map();
	void Render();
	void SetTileMapData(int** tileMapData);
	void SetCamera(Camera* cam);
	void ExtractTileFromTileSet();
	int GetMapWidth();
};

