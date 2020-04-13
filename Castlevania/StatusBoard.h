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
	//HealthBar* bossbar;
	LPSPRITE supweaponRectSprite;
	LPSPRITE subweaponSprite;
public:
	StatusBoard(Simon * simon);
	~StatusBoard();
	void Update(DWORD dt);
	void Render();
	void SetFont(ID3DXFont* font);
	string FillStringWithZeros(string str, int totalChar);
	
	void SetSupweaponSprite(LPSPRITE sprite);
};

