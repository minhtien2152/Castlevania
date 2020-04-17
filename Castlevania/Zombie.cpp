#include "Zombie.h"

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y+1;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;

}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* staticCoObjects , vector<LPGAMEOBJECT>* dynamicCoObjects )
{
	
	CGameObject::Update(dt, staticCoObjects,  dynamicCoObjects);

	
	// Check collision between zombie and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEventsResult;


	if(state!= ZOMBIE_STATE_DIE)
		CalcPotentialCollisions(staticCoObjects, staticCoEvents);
	

	if (staticCoEvents.size() == 0)
	{
		x += dx;
		y += dy;
		vy += ZOMBIE_GRAVITY * dt;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		
		FilterCollision(staticCoEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);



		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		if (nx != 0 && ny == 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}
		else if (ny == -1.0f)
		{
			vy = 0;
		}
	}

	// clean up collision events
	for (int i = 0; i < staticCoEvents.size(); i++) delete  staticCoEvents[i];
}

void CZombie::Render()
{
	if (state != ZOMBIE_STATE_DIE)
	{
		animation_set->at(state)->Render(x, y, nx);
		RenderBoundingBox();
	}
	
}

CZombie::CZombie()
{
	SetState(ZOMBIE_STATE_WALKING);
	nx = -1;
}
