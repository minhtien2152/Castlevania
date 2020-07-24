#include "SmartEnemy.h"
#include "Simon.h"
#include "Define.h"
SmartEnemy::SmartEnemy()
{
	hp = 1;
	isDestructable = true;
	timeAccumulated = 0;
	player = NULL;
}

void SmartEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (player == NULL)
		for (UINT i = 0; i < nonSolidObjects.size(); i++)
		{
			LPGAMEOBJECT obj = nonSolidObjects[i];
			if (dynamic_cast<Simon*>(obj))
				player = obj;
		}
	timeAccumulated += dt;
}

void SmartEnemy::Render()
{
	animation_set->at(state)->Render(x, y, nx,isUpdatingFrame);
	RenderBoundingBox();
}

void SmartEnemy::AdjustDirectionToFollowSimon()
{
	if (player != NULL)
	{
		float plr_x, plr_y;
		player->GetPosition(plr_x, plr_y);
		if (x - (plr_x + TILE_WIDTH) > 0)
			SetDirection(1);
		else
			SetDirection(-1);
	}
}
