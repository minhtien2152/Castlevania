#include "FireBall.h"
#include "Define.h"

#define FIREBALL_BBOX_WIDTH 16
#define FIREBALL_BBOX_HEIGHT 16
#define FIREBALL_SPEED 0.2f
FireBall::FireBall()
{
	animation_set = CAnimationSets::GetInstance()->Get(Weapon_Type::FIREBALL);
	tag = Weapon_Type::FIREBALL;
	isPhysicEnabled = false;
	damage = 2;
}

void FireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

}

void FireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

void FireBall::SetSpeed(float desX, float desY)
{
	D3DXVECTOR2* v1 = new D3DXVECTOR2(desX - x, desY - y);
	D3DXVECTOR2* v2 = new D3DXVECTOR2(desX - x, 0);
	/*D3DXVec2Normalize(v1, v1);
	D3DXVec2Normalize(v2, v2);*/
	float angle = acos(D3DXVec2Dot(v1, v2) / (D3DXVec2Length(v1) * D3DXVec2Length(v2)));
	float temp_vx = FIREBALL_SPEED * cos(angle);
	float temp_vy = FIREBALL_SPEED * sin(angle);

	if (desX - x < 0)
	{
		vx = -temp_vx;
		nx = -1;
	}
	else {
		vx = temp_vx;
		nx = 1;
	}
	if (desY - y < 0)
		vy = -temp_vy;
	else
		vy = temp_vy;
}
