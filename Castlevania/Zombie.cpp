#include "Zombie.h"

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y+1;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;

}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	if (!isEnabled) return;
	CGameObject::Update(dt,coObjects);
	vy += ZOMBIE_GRAVITY * dt;


}

void CZombie::Render()
{
	if (!isEnabled) return;
	
	animation_set->at(state)->Render(x, y, nx);
	RenderBoundingBox();
	
	
	
}

CZombie::CZombie()
{
	SetState(ZOMBIE_STATE_WALKING);
	nx = -1;
}
