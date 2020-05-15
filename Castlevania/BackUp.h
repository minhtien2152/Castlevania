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
public:
	static BackUp* GetInstance();
	BackUp();
	~BackUp();
	void BackUpSimon(Simon* simon);
	void LoadBackUp(Simon* simon);
	void SetTime(DWORD time);
	DWORD GetTime();
	void SetSupWPItem(int type);
	int GetSupWPItem();
	void ResetData();
	
};

