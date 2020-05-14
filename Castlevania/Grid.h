#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Define.h"
#include "Camera.h"
#include <vector>
#include <unordered_map>
typedef vector<LPGAMEOBJECT>* Cell;
class Grid
{
	int screen_width,map_height;
	int column, row;
	int cell_width, cell_height;
	vector<LPGAMEOBJECT> object_list;
	vector<vector< Cell>> grid_cells;
public:
	Grid(int mapWidth,int mapHeight);
	void Update();
	void AddObject(LPGAMEOBJECT obj);
	void GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object, unordered_map<int,bool>* dup_checker );
	void GetCellObjects(int row,int col,vector<LPGAMEOBJECT>* cell_object, unordered_map<int, bool>* dup_checker);
	void Unload();
	void ClearCells();
	int GetColumn(float x);
	int GetRow(float y);
};

