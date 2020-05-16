#include "StopWatch.h"
#include "Define.h"
#define STOP_WATCH_ACTIVATION_TIME 5000
StopWatch::StopWatch()
{
	tag = Weapon_Type::STOPWATCH;
	cost = 5;
}


void StopWatch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount() - activateTime >= STOP_WATCH_ACTIVATION_TIME)
		this->isEnabled = false;
}

void StopWatch::Attack(LPGAMEOBJECT user)
{
	isEnabled = true;
	activateTime = GetTickCount();
}

void StopWatch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}
