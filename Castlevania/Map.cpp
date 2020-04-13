#include "Map.h"
#include "Define.h"
#include "Sprite.h"


Map::Map(int tileSetID, int rowMap, int columnMap, int rowTileSet, int columnTileSet, int totalTile)
{
	tileSet =CTextures::GetInstance()->Get(tileSetID);
	this->rowMap = rowMap;
	this->columnMap = columnMap;
	this->rowTileSet = rowTileSet;
	this->columnTileSet = columnTileSet;
	this->totalTile = totalTile;
}

Map::~Map()
{
}

void Map::Render()
{
	for (int i = 0; i < rowMap; i++)
		for (int j = 0; j < columnMap; j++)
			DrawTile(tileMap[i][j]-1,i,j);
}

void Map::SetTileMapData(int** tileMapData)
{
	tileMap = tileMapData;
}



void Map::DrawTile(int id,int currentRow,int currentColumn)
{

	int left = id % columnTileSet * TILE_WIDTH;
	int top = id / columnTileSet * TILE_HEIGHT;
	int right = left + TILE_WIDTH;
	int bottom = top + TILE_HEIGHT;
	LPSPRITE tile = new CSprite(id, left, top, right, bottom, tileSet);
	tile->Draw(currentColumn * TILE_WIDTH, currentRow * TILE_HEIGHT+ STATUS_BOARD_HEIGHT);
}
