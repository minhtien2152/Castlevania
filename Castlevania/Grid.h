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
	vector<int> cam_cell;
	vector<int>	update_cell;
	unordered_map<int, vector<LPGAMEOBJECT>> grid_cell;
public:
	Grid(int mapWidth,int mapHeight);
	void Update(Camera*cam);
	void PlaceObjectInGrid(LPGAMEOBJECT obj);
	void PlaceObjectInGrid(LPGAMEOBJECT obj,int row, int col);
	void CleanUp();
	void GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object);
	void GetUpdateObjects(vector<LPGAMEOBJECT>* cell_object);
	void GetCamObjects(vector<LPGAMEOBJECT>* cell_object);
	void SetCamCell(Camera* cam);
	void Unload();
	void ClearCells();
	void ClearAll();
	void DeleteAllObjects();
	void DeleteDisabledObj(vector<LPGAMEOBJECT>* list);
	int GetColumn(float x);
	int GetRow(float y);
	int GetCellId(int col, int row);
};

