#pragma once
#include"Simon.h"
#include "Define.h"
class BackUp
{
	static BackUp* __instance;
	int health;
	int life;
	int subWeaponItem;
	DWORD time;
	int score;
	int heart;
	int whipLv;
	int subWeaponCap;
	bool loseLife;

	int currentStairDirection;
	int currentStairType;
	int stairState;
	int nx;
public:
	static BackUp* GetInstance();
	BackUp();
	~BackUp();
	void BackUpSimon(Simon* simon);
	void LoadBackUp(Simon* simon);
	void SetTime(DWORD time);
	void SetLife(int life);
	void LoseLife();
	DWORD GetTime();
	void SetSupWPItem(int type);
	int GetSupWPItem();
	void ResetData();
	bool HaveJustLostLife();
	
};

