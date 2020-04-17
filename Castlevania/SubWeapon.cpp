#include "SubWeapon.h"
#include "Define.h"
#include "Utils.h"
SubWeapon::SubWeapon()
{
	isEnabled = false;
	animation_set = CAnimationSets::GetInstance()->Get(12);
}

SubWeapon::~SubWeapon()
{
}

void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* staticCoObjects , vector<LPGAMEOBJECT>* dynamicCoObjects )
{
	CGameObject::Update(dt, staticCoObjects, dynamicCoObjects);
	x += dx;
	y += dy;
	switch (state)
	{
	case Weapon_Type::AXE:
		vy += GRAVITY * dt;
		break;
	case Weapon_Type::HOLYWATER:
		vy += GRAVITY * dt;
		break;
	case Weapon_Type::BOOMERANG:
		if (nx > 0) vx -= BOOMERANG_TURNBACK_SPEED;
		else vx += BOOMERANG_TURNBACK_SPEED;
		break;
	default:
		break;
	}

	//DebugOut(L"nx = %f \n", nx);
}

void SubWeapon::Render()
{
	animation_set->at(state)->Render( x, y,nx);
	RenderBoundingBox();
}

void SubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;

	switch (state)
	{
	case Weapon_Type::DAGGER:
		right = left + DAGGER_BBOX_WIDTH;
		bottom = top + DAGGER_BBOX_HEIGHT;
		break;
	case Weapon_Type::AXE:
		right = left + AXE_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	case Weapon_Type::HOLYWATER:
		right = left + HOLY_WATER_BBOX_WIDTH;
		bottom = top + AXE_BBOX_HEIGHT;
		break;
	case Weapon_Type::BOOMERANG:
		right = left + BOOMERANG_BBOX_WIDTH;
		bottom = top + BOOMERANG_BBOX_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}

void SubWeapon::SetState(int state)
{
	CGameObject::SetState(state);
	
}

void SubWeapon::Attack()
{
	switch (state)
	{
	case Weapon_Type::DAGGER:
		vx = -nx * DAGGER_SPEED;
		vy = 0;
		break;
	case Weapon_Type::AXE:
		vx = nx * AXE_SPEED_X;
		vy = -AXE_SPEED_Y;
		break;
	case Weapon_Type::HOLYWATER:
		vx = nx * HOLY_WATER_SPEED_X;
		vy = -HOLY_WATER_SPEED_Y;
		break;
		/*case WEAPONS_HOLY_WATER_SHATTERED:
			vx = 0;
			vy = 0;
			StartHolyWaterEffect();
			break;*/
	case Weapon_Type::BOOMERANG:
		vx = nx * BOOMERANG_SPEED;
		vy = 0;
		break;
	default:
		break;
	}
}
