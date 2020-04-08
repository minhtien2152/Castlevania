#include "Zombie.h"

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y+1;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;

}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);

	
	// Check collision between zombie and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if(state!= ZOMBIE_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		vy += ZOMBIE_GRAVITY * dt;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

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
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
