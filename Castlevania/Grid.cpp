#include "Grid.h"

Grid::Grid(int mapWidth, int mapHeight)
{
	
	screen_width = CGame::GetInstance()->GetScreenWidth();
	screen_height = CGame::GetInstance()->GetScreenHeight();
	map_height = mapHeight;
	cell_width = screen_width / 2 ;
	cell_height = screen_height/ 2 ;
	this->column = mapWidth / cell_width;
	this->row = mapHeight / cell_height;

}

void Grid::Update()
{
	ClearCells();
	
	for(UINT i = 0; i < object_list.size(); i++)
		if (!object_list[i]->isEnabled)
		{
			DebugOut(L"deleted obj type %d\n", object_list[i]->tag);
			free(object_list[i]);
			object_list[i] = NULL;
			object_list.erase(object_list.begin() + i);
		}
	for (auto obj : object_list)
	{
		float l, t, r, b;
		obj->GetBoundingBox(l, t, r, b);
		int start_row = GetRow(t);
		int end_row = GetRow(b);
		int start_col = GetColumn(l);
		int end_col = GetColumn(r);
		for (int row = start_row; row <= end_row; row++)
			for (int col = start_col; col <= end_col; col++)
			{
				grid_cell[GetCellId(col, row)].push_back(obj);
				if (obj->tag == Object_Type::STAIR_OBJECT)
					DebugOut(L"Added Stair to row %d, col %d,id %d\n", row, col, GetCellId(col, row));
			}

	}
}

void Grid::AddObject(LPGAMEOBJECT obj)
{
	object_list.push_back(obj);
	DebugOut(L"size %d\n", object_list.size());
}

void Grid::GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object)
{
	float cam_x, cam_y;
	cam->GetCamPosition(cam_x, cam_y);
	int start_row = GetRow(cam_y);
	int end_row = GetRow(cam_y + screen_height);
	int start_col = GetColumn(cam_x);
	int end_col = GetColumn(cam_x + screen_width - 2);
	unordered_map<int, LPGAMEOBJECT> temp_list;
	
	
	//DebugOut(L"start row %d,end row %d, start col %d, end col %d\n", start_row, end_row, start_col, end_col);
	for (int row = start_row; row <= end_row; row++)
		for (int col = start_col; col <= end_col; col++)
		{
			for (auto obj : grid_cell[GetCellId(col,row)])
			{
				if (temp_list.find(obj->id) == temp_list.end())
				{
					temp_list[obj->id] = obj;
					//DebugOut(L"Get cell %d\n", GetCellId(col, row));
				}
			}
		}
	for (auto obj : temp_list)
		cell_object->push_back(obj.second);
	//DebugOut(L"total object %d, total got %d\n", object_list.size(), temp_list.size());
}




void Grid::Unload()
{
	ClearCells();


	for (UINT i = 0; i < object_list.size(); i++)
	{
		free(object_list[i]);
		object_list[i] = NULL;
		object_list.erase(object_list.begin() + i);
	}
}

void Grid::ClearCells()
{
	for (auto cell : grid_cell)
		cell.second.clear();
	grid_cell.clear();
}



int Grid::GetColumn(float x)
{
	return x / cell_width ;
}

int Grid::GetRow(float y)
{
	return y / cell_height;
}

int Grid::GetCellId(int col, int row)
{
	return col + row * column;
}