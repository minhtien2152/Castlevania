#pragma once
#include <d3d9.h>
#include"Textures.h"
class Map
{
	int rowMap, columnMap;
	int rowTileSet, columnTileSet;

	int totalTile;	//total tiles of tile set
	
	LPDIRECT3DTEXTURE9 tileSet;
	int** tileMap;
public:
	Map(int tileSetID,int rowMap,int columnMap, int rowTileSet,int  columnTileSet, int totalTile);
	~Map();
	void Render();
	void SetTileMapData(int** tileMapData);
	void DrawTile(int id,int currentRow,int currentColumn);
};

