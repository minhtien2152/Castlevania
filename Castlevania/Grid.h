#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Define.h"
#include "Camera.h"
#include <vector>
#include <unordered_map>

class Grid
{
	int screen_width, screen_height;
	int map_height;
	int column, row;
	int cell_width, cell_height;
	vector<LPGAMEOBJECT> object_list;
	unordered_map<int, vector<LPGAMEOBJECT>> grid_cell;
public:
	Grid(int mapWidth,int mapHeight);
	void Update();
	void AddObject(LPGAMEOBJECT obj);
	void GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object);
	void Unload();
	void ClearCells();
	int GetColumn(float x);
	int GetRow(float y);
	int GetCellId(int col, int row);
};

