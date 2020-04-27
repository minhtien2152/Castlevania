#include "StatusBoard.h"
#include"Define.h"
#include"time.h"
#include "Sprites.h"
StatusBoard::StatusBoard(Simon* simon)
{
	time = 0;
	this->simon = simon;
	simonbar = new HealthBar(100, 30, simon,SIMON_HEALTH);
	supweaponRectSprite = CSprites::GetInstance()->Get(SUB_WEAPON_RECT_TEX_ID);
}

StatusBoard::~StatusBoard()
{
}

void StatusBoard::Update(DWORD dt)
{
	time += dt;
	simonbar->Update();
}

void StatusBoard::Render()
{
	string score = FillStringWithZeros(to_string(simon->GetScore()),6);
	string hearts= FillStringWithZeros(to_string(simon->GetHeartsCollected()),2);
	string life = FillStringWithZeros(to_string(simon->GetLife()),2);
	

	int remainTime = DEFAULT_TIME_PLAY - time / CLOCKS_PER_SEC;
	string time = FillStringWithZeros(to_string(remainTime), 3);

	info = "SCORE-" + score + " TIME " + time + " SCENE " + "\n";
	info+= "PLAYER                  -" + hearts + "\n";
	info+= "ENEMY                   -" + life + "\n";

	RECT rect;
	SetRect(&rect, 0, 15, SCREEN_WIDTH, STATUS_BOARD_HEIGHT);

	if (font != NULL)
		font->DrawTextA(NULL, info.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	simonbar->Render();
	supweaponRectSprite->Draw(290, 32,1,255,false);
	if (subweaponSprite!= NULL)
		subweaponSprite->Draw(305, 38,1,255 ,false);

}

void StatusBoard::SetFont(ID3DXFont* font)
{
	this->font = font;
}

string StatusBoard::FillStringWithZeros(string str, int totalChar)
{
	while (str.length() < totalChar) str = "0" + str;
	return str;
}

void StatusBoard::SetSupweaponSprite(LPSPRITE sprite)
{
	this->subweaponSprite = sprite;
}
