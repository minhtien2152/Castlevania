#include "Grid.h"

Grid::Grid(int mapWidth, int mapHeight)
{
	
	screen_width = CGame::GetInstance()->GetScreenWidth();
	map_height = mapHeight;
	cell_width = screen_width / 2 ;
	cell_height = mapHeight / 2 ;
	this->column = mapWidth / cell_width;
	this->row = mapHeight / cell_height;
	grid_cells.resize(row);
	for (UINT i = 0; i < grid_cells.size(); i++)
		grid_cells[i].resize(column);
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			grid_cells[i][j] = new vector<LPGAMEOBJECT>;
}

void Grid::Update()
{
	ClearCells();
	
	for(UINT i = 0; i < object_list.size(); i++)
		if (!object_list[i]->isEnabled)
		{
			free(object_list[i]);
			object_list[i] = NULL;
			object_list.erase(object_list.begin() + i);
		}
	for (auto obj : object_list)
	{
		float l, t, r, b;
		obj->GetBoundingBox(l, t, r, b);
		t -= STATUS_BOARD_HEIGHT;
		b -= STATUS_BOARD_HEIGHT;
		int start_row = GetRow(t);
		int end_row = GetRow(b);
		int start_col = GetColumn(l);
		int end_col = GetColumn(r);
		for (int row = start_row; row <= end_row; row++)
			for (int col = start_col; col <= end_col; col++)
				grid_cells.at(row).at(col)->push_back(obj);

	}
}

void Grid::AddObject(LPGAMEOBJECT obj)
{
	object_list.push_back(obj);
	DebugOut(L"size %d\n", object_list.size());
}

void Grid::GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object, unordered_map<int, bool>* dup_checker)
{
	float cam_x, cam_y;
	cam->GetCamPosition(cam_x, cam_y);
	int start_row = GetRow(cam_y);
	int end_row = GetRow(cam_y + map_height - 2);
	int start_col = GetColumn(cam_x);
	int end_col = GetColumn(cam_x + screen_width - 2);

	for (int row = start_row; row <= end_row; row++)
		for (int col = start_col; col <= end_col; col++)
			GetCellObjects(row,col, cell_object, dup_checker);
}

void Grid::GetCellObjects(int row,int col, vector<LPGAMEOBJECT>* cell_object, unordered_map<int, bool>* dup_checker)
{
	for (UINT i =0; i<grid_cells.at(row).at(col)->size();i++)
	{
		if (dup_checker->find(grid_cells.at(row).at(col)->at(i)->id) == dup_checker->end())
		{
			cell_object->push_back(grid_cells.at(row).at(col)->at(i));
			dup_checker->emplace(grid_cells.at(row).at(col)->at(i)->id,1);
			
		}
	}
}

void Grid::Unload()
{
	ClearCells();
	for (auto row : grid_cells)
		row.clear();
	grid_cells.clear();

	for (UINT i = 0; i < object_list.size(); i++)
	{
		free(object_list[i]);
		object_list[i] = NULL;
		object_list.erase(object_list.begin() + i);
	}
}

void Grid::ClearCells()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			grid_cells[i][j]->clear();
}



int Grid::GetColumn(float x)
{
	return x / (cell_width + 1);
}

int Grid::GetRow(float y)
{
	return y / (cell_height+1);
}

