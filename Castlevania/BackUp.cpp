#include "BackUp.h"


BackUp* BackUp::__instance = NULL;
BackUp* BackUp::GetInstance()
{
	if (__instance == NULL) __instance = new BackUp();
	return __instance;
}

BackUp::BackUp()
{
	ResetData();
}

BackUp::~BackUp()
{
}

void BackUp::BackUpSimon(Simon* simon)
{
	health = simon->GetHP();
	life = simon->GetLife();
	score = simon->GetScore();
	heart = simon->GetHeartsCollected();
	whipLv = simon->GetMainWeapon()->GetState();
	subWeaponCap = simon->GetSubWeaponCap();
	subWeaponItem = simon->GetSubWeaponType();
	currentStairDirection = simon->GetCurrentStairDirection();
	currentStairType = simon->GetCurrentStairType();
	stairState = simon->stairState;
	nx = simon->nx;
}

void BackUp::LoadBackUp(Simon* simon)
{
	simon->SetHeartsCollected(heart);
	simon->SetHP(health);
	simon->SetLife(life);
	simon->SetScore(score);
	if(simon->GetMainWeapon()!= NULL)
	simon->GetMainWeapon()->SetState(whipLv);
	simon->SetSupWeaponCap(subWeaponCap);
	simon->SetSubWeaponItem(subWeaponItem);
	simon->SetCurrentStairDirection(currentStairDirection);
	simon->SetCurrentStairType(currentStairType);
	simon->stairState = stairState;
	simon->SetDirection(nx);
	loseLife = false;
}

void BackUp::SetTime(DWORD time)
{
	this->time = time;
}

void BackUp::SetLife(int life)
{
	this->life = life;
}

void BackUp::LoseLife()
{
	int life_backup = life-1;
	ResetData();
	SetLife(life_backup);
	loseLife = true;
}

DWORD BackUp::GetTime()
{
	return time;
}

void BackUp::SetSupWPItem(int type)
{
	subWeaponItem = type;
}

int BackUp::GetSupWPItem()
{
	return subWeaponItem;
}

void BackUp::ResetData()
{
	health = SIMON_DEFAULT_HEALTH;
	score = 0;
	life = SIMON_DEFAULT_LIFE;
	heart = SIMON_DEFAULT_HEART;
	time = 0;
	subWeaponItem = -1;
	whipLv = 0;
	subWeaponCap = 1;
	stairState = STAIR_STATE_NONE;
	nx = 1;
}

bool BackUp::HaveJustLostLife()
{
	return loseLife;
}
