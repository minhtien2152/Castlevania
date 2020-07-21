#include "Holywater.h"
#include "Define.h"
#include "AnimationSets.h"
#define HOLY_WATER_BROKE 1
#define HOLY_WATER_INTACT 0
#define HOLY_WATER_FIRE_DELAY 1200
Holywater::Holywater()
{
	brokeTime = 0;
	animation_set = CAnimationSets::GetInstance()->Get(Weapon_Type::HOLYWATER);
	tag = Weapon_Type::HOLYWATER;
	cost = 1;
	isPhysicEnabled = true;
}
Holywater::~Holywater()
{
}
void Holywater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += GRAVITY * dt;
	if (isTouchingGround)
	{
		
		if (brokeTime == 0)
		{
			Break();
		}
		if (animation_set->at(state)->CheckAniTime(HOLY_WATER_FIRE_DELAY))
		{
			this->isEnabled = false;
			CleanUpCoEvents();
		}
	}
	
	for (UINT i = 0; i < nonSolidObjects.size(); i++)
	{
		if(IsColidingAABB(nonSolidObjects[i]))
		if (nonSolidObjects[i]->isDestructable)
		{
			nonSolidObjects[i]->GetHit(this->damage);
		}

	}
	//DebugOut(L"CoEvent size : %d\n", nonSolidObjCoEvents.size());
	
	CleanUpCoEvents();
}

void Holywater::Break()
{
	SetState(HOLY_WATER_BROKE);
	ResetAni();
	brokeTime = GetTickCount();
	vx = vy = 0;
}

void Holywater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + HOLY_WATER_BBOX_WIDTH;
	bottom = top + HOLY_WATER_BBOX_WIDTH;
}

void Holywater::Attack(LPGAMEOBJECT user)
{
	CWeapon::Attack(user);
	vx = -nx * HOLY_WATER_SPEED_X;
	vy = -HOLY_WATER_SPEED_Y;
	SetState(HOLY_WATER_INTACT);
	brokeTime = 0;
}
