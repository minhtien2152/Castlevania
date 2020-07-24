#include "Zombie.h"

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;

}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	CGameObject::Update(dt,coObjects);
	vx = -nx*ZOMBIE_WALKING_SPEED;
	vy += ZOMBIE_GRAVITY * dt;


}

void CZombie::Render()
{

	animation_set->at(state)->Render(x, y, nx,isUpdatingFrame);
	RenderBoundingBox();

}

CZombie::CZombie()
{
	SetState(ZOMBIE_STATE_WALKING);
	isDestructable = true;
}
