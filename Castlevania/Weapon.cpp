#include "Weapon.h"
#include "Utils.h"
#include "Define.h"
CWeapon::CWeapon()
{
	damage = 1;
	isEnabled = false;
	isStatic = false;
	cost = 0;
	isPhysicEnabled = false;
}

CWeapon::~CWeapon()
{
}

void CWeapon::Attack(LPGAMEOBJECT user)
{
	float obj_x, obj_y;
	user->GetPosition(obj_x, obj_y);
	if(user->nx == 1)
		SetPosition(obj_x, obj_y);
	else
		SetPosition(obj_x+SIMON_BBOX_WIDTH, obj_y);
	isEnabled = true;
	nx = user->nx;
	ResetAni();
}

void CWeapon::SetPosition(float x, float y)
{
	CGameObject::SetPosition(x, y);
}

void CWeapon::Render()
{
	animation_set->at(state)->Render(x, y, nx);
	RenderBoundingBox();
}
