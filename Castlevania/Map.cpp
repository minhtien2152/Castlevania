#include "Map.h"
#include "Define.h"
#include "Sprite.h"
#include "Utils.h"
#include "Game.h"
Map::Map(int tileSetID, int rowMap, int columnMap, int rowTileSet, int columnTileSet, int totalTile)
{
	tileSet =CTextures::GetInstance()->Get(tileSetID);
	this->rowMap = rowMap;
	this->columnMap = columnMap;
	this->rowTileSet = rowTileSet;
	this->columnTileSet = columnTileSet;
	this->totalTile = totalTile;
	screenWidth = CGame::GetInstance()->GetScreenWidth();
}

Map::~Map()
{
}

void Map::Render()
{
	float camX,  camY;
	camera->GetCamPosition(camX, camY);
	float startingColumn = floor(camX / TILE_WIDTH);
	float maxColumn = ceil( (screenWidth + camX)/ TILE_WIDTH);
	DebugOut(L" start = %f, max = %f\n", startingColumn, maxColumn);

	if (maxColumn >= columnMap) maxColumn = columnMap;
	for (int currentRow =0; currentRow < rowMap; currentRow++)
		for (int currentColumn = startingColumn; currentColumn < maxColumn; currentColumn++)
			tiles.at(tileMap[currentRow][currentColumn] - 1)->Draw(currentColumn * TILE_WIDTH, currentRow * TILE_HEIGHT + STATUS_BOARD_HEIGHT);
}

void Map::SetTileMapData(int** tileMapData)
{
	tileMap = tileMapData;
}

void Map::SetCamera(Camera* cam)
{
	this->camera = cam;
}


void Map::ExtractTileFromTileSet()
{
	for (int tileNum = 0; tileNum < totalTile; tileNum++)
	{
		int left =  tileNum	% columnTileSet * TILE_WIDTH;
		int top = tileNum / columnTileSet * TILE_HEIGHT;
		int right = left + TILE_WIDTH;
		int bottom = top + TILE_HEIGHT;
		LPSPRITE newTile = new CSprite(tileNum, left, top, right, bottom, tileSet);
		this->tiles.push_back(newTile);
	}
}

int Map::GetMapWidth()
{
	return columnMap * TILE_WIDTH;
}
