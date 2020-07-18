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
	vector<LPGAMEOBJECT> objList;
	unordered_map<int, vector<LPGAMEOBJECT>> grid_cell;
public:
	Grid(int mapWidth,int mapHeight);
	void Update();
	void PlaceObjectInGrid(LPGAMEOBJECT obj);
	void AddObject(LPGAMEOBJECT obj);
	void CleanUp();
	void GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object);
	void Unload();
	void ClearCells();
	void DeleteDisabledObj();
	int GetColumn(float x);
	int GetRow(float y);
	int GetCellId(int col, int row);
};

