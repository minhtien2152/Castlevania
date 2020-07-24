#include "Grid.h"
#include <algorithm>
Grid::Grid(int mapWidth, int mapHeight)
{
	
	screen_width = CGame::GetInstance()->GetScreenWidth();
	screen_height = CGame::GetInstance()->GetScreenHeight();
	map_height = mapHeight;
	cell_width = screen_width / 2 ;
	cell_height = screen_height/ 2 ;
	this->column = mapWidth / cell_width;
	

}

void Grid::Update(Camera* cam)
{
	vector<LPGAMEOBJECT> temp_list;
	SetCamCell(cam);
	GetUpdateObjects(&temp_list);
	ClearCells();
	for (auto obj : temp_list)
	{
		if (obj->isEnabled == false)
		{
			delete(obj);
			obj = NULL;
		}
		else
		PlaceObjectInGrid(obj);
	}
}

void Grid::PlaceObjectInGrid(LPGAMEOBJECT obj)
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
			//if (obj->tag == Object_Type::STAIR_OBJECT)
			//	DebugOut(L"Added Stair to row %d, col %d,id %d\n", row, col, GetCellId(col, row));
		}

}

void Grid::PlaceObjectInGrid(LPGAMEOBJECT obj, int id)
{
	grid_cell[id].push_back(obj);
}




void Grid::CleanUp()
{
}

void Grid::GetObjectsAccordingCam(Camera* cam, vector<LPGAMEOBJECT>* cell_object)
{
	float cam_x, cam_y;
	cam->GetCamPosition(cam_x, cam_y);
	int start_row = GetRow(cam_y);
	int end_row = GetRow(cam_y + screen_height);
	int start_col = GetColumn(cam_x);
	int end_col = GetColumn(cam_x + screen_width -2);
	unordered_map<int, LPGAMEOBJECT> temp_list;
	//DebugOut(L"start row %d,end row %d, start col %d, end col %d\n", start_row, end_row, start_col, end_col);
	for (int row = start_row; row <= end_row; row++)
		for (int col = start_col; col <= end_col; col++)
		{
			int cell = GetCellId(col, row);
			cam_cell.push_back(cell);
			for (auto obj : grid_cell[cell])
			{
				if (temp_list[obj->id]==NULL)
				{
					temp_list[obj->id] = obj;
				//	
				}
			}
			//DebugOut(L"Get cell %d\n", GetCellId(col, row));
		}
	for (auto obj : temp_list)
		cell_object->push_back(obj.second);
	//DebugOut(L"total object %d, total got %d\n", object_list.size(), temp_list.size());
}

void Grid::GetUpdateObjects(vector<LPGAMEOBJECT>* cell_object)
{

	unordered_map<int, LPGAMEOBJECT> temp_list;
	//DebugOut(L"start row %d,end row %d, start col %d, end col %d\n", start_row, end_row, start_col, end_col);
	for (auto cell_num : update_cell)
	{
		if (grid_cell[cell_num].size() > 0)
			for (auto obj : grid_cell[cell_num])
			{
				if (temp_list.find(obj->id) == temp_list.end())
				{
					temp_list[obj->id] = obj;
					//	
				}
			}
	}
	for (auto obj : temp_list)
		cell_object->push_back(obj.second);
}

void Grid::GetCamObjects(vector<LPGAMEOBJECT>* cell_object)
{
	unordered_map<int, LPGAMEOBJECT> temp_list;
	//DebugOut(L"start row %d,end row %d, start col %d, end col %d\n", start_row, end_row, start_col, end_col);
	for (auto cell_num : cam_cell)
	{
		for (auto obj : grid_cell[cell_num])
		{
			if (temp_list.find(obj->id) == temp_list.end())
			{
				temp_list[obj->id] = obj;
				//	
			}
		}
	}
	for (auto obj : temp_list)
		cell_object->push_back(obj.second);
}

void Grid::SetCamCell(Camera* cam)
{
	cam_cell.clear();
	update_cell.clear();
	float cam_x, cam_y;
	cam->GetCamPosition(cam_x, cam_y);
	int start_row = GetRow(cam_y);
	int end_row = GetRow(cam_y + screen_height);
	int start_col = GetColumn(cam_x);
	int end_col = GetColumn(cam_x + screen_width - 2);
	for (int row = start_row; row <= end_row; row++)
		for (int col = start_col; col <= end_col; col++)
		{
			int cell = GetCellId(col, row);
			cam_cell.push_back(cell);
		}

	if (start_col > 0)
		start_col--;
	if (end_col < column - 1)
		end_col++;
	for (int row = start_row; row <= end_row; row++)
		for (int col = start_col; col <= end_col; col++)
		{
			int cell = GetCellId(col, row);
			update_cell.push_back(cell);
		}

}




void Grid::Unload()
{
	DeleteAllObjects();
	ClearAll();
}

void Grid::ClearCells()
{
	for (auto cell_num : update_cell)
	{
		grid_cell[cell_num].clear();
	}
}

void Grid::ClearAll()
{
	for (auto cell : grid_cell)
	{
		cell.second.clear();
	}
}

void Grid::DeleteAllObjects()
{
	vector<LPGAMEOBJECT> tempList;
	for (auto cell : grid_cell)
		for (auto obj : cell.second)
			tempList.push_back(obj);
	std::sort(tempList.begin(), tempList.end());
	tempList.erase(std::unique(tempList.begin(), tempList.end()), tempList.end());
	for (auto obj : tempList)
	{
		delete(obj);
		obj = NULL;
	}
}


void Grid::DeleteDisabledObj(vector<LPGAMEOBJECT>* list)
{
	for (auto obj = list->begin(); obj != list->end();)
	{ 
		if ((*obj)->isEnabled == false)
			obj = list->erase(obj);
		else
			obj++;
	}
}



int Grid::GetColumn(float x)
{
	if (x < 0)
		return 0;
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