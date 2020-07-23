#include "ItemSpawner.h"
#include "Define.h"
#include"Item.h"
ItemSpawner::ItemSpawner()
{
	state = -1;
	isSolid = false;
	isDestructable = false;
}
void ItemSpawner::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+ TILE_WIDTH / 2;
	top = y;
	right = left + TILE_WIDTH/2;
	bottom = top + TILE_HEIGHT;
}

void ItemSpawner::Render()
{
	RenderBoundingBox();
}

void ItemSpawner::SetItemList(vector<LPGAMEOBJECT>* list)
{
	itemList = list;
}

void ItemSpawner::SetItemSpawnPoint(float x, float y)
{
	spawn_posX = x;
	spawn_posY = y;
}

void ItemSpawner::Activate()
{
	isActivated = true;
	Item* item = new Item();
	item->SetState(itemSpawn);
	item->SetPosition(spawn_posX, spawn_posY);
	itemList->push_back(item);
}
