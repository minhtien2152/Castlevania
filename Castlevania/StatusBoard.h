#pragma once
#include"Simon.h"
#include "HealthBar.h"
#include <string>
#include <d3dx9.h>
#include <d3d9.h>
using namespace std;

class StatusBoard
{
	Simon* simon;

	string info;
	ID3DXFont* font;
	DWORD time;

	HealthBar* simonbar;
	HealthBar* bossbar;

	LPANIMATION_SET animation_set;
	LPANIMATION_SET item_animation_set;
	int subWeaponType;
	int subWeaponCap;
	int sceneId;
	
public:
	bool hasBoss;
	StatusBoard(Simon* simon, LPGAMEOBJECT boss);
	~StatusBoard();
	void Update(DWORD dt);
	void Render();
	void SetFont(ID3DXFont* font);
	string FillStringWithZeros(string str, int totalChar);
	
	void SetSupweapon(int type);
	void Reset();
	DWORD GetTime();
	void SetTime(DWORD time);
	void SetSceneId(int id);
};

