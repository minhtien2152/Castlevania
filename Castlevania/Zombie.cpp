#include "Zombie.h"

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;

	if (state == ZOMBIE_STATE_DIE)
		bottom = y + ZOMBIE_BBOX_HEIGHT_DIE;
	else
		bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CZombie::Render()
{
	int ani = ZOMBIE_ANI_WALKING;
	if (state == ZOMBIE_STATE_DIE) {
		ani = ZOMBIE_ANI_DIE;
	}

	animations[ani]->Render(x, y,1);
	//RenderBoundingBox();
}

void CZombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_DIE:
		y += ZOMBIE_BBOX_HEIGHT - ZOMBIE_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case ZOMBIE_STATE_WALKING:
		vx = -ZOMBIE_WALKING_SPEED;
	}

}
