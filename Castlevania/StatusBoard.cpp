#include "StatusBoard.h"
#include"Define.h"
#include"time.h"
#include "Sprites.h"
#define STATUS_BOARD_ID	101

StatusBoard::StatusBoard(Simon* simon)
{
	Reset();
	this->simon = simon;
	simonbar = new HealthBar(100, 30, simon, SIMON_HEALTH);
	animation_set = CAnimationSets::GetInstance()->Get(STATUS_BOARD_ID);
	item_animation_set = CAnimationSets::GetInstance()->Get(ITEM_ANI_SET_ID);
	subWP = -1;
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
	string sceneID = FillStringWithZeros(to_string(sceneId), 2);

	int remainTime = DEFAULT_TIME_PLAY - time / CLOCKS_PER_SEC;
	string time = FillStringWithZeros(to_string(remainTime), 3);

	info = "SCORE-" + score + " TIME " + time + " SCENE "+ sceneID  + "\n";
	info+= "PLAYER                  -" + hearts + "\n";
	info+= "ENEMY                   -" + life + "\n";

	RECT rect;
	SetRect(&rect, 0, 15, SCREEN_WIDTH, STATUS_BOARD_HEIGHT);

	if (font != NULL)
		font->DrawTextA(NULL, info.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	simonbar->Render();
	animation_set->at(SUB_WEAPON_RECT_TEX_ID)->Render(290, 32,1,false);
	if (subWP!=-1)
		item_animation_set->at(subWP)->Render(305, 38,1,false);

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

void StatusBoard::SetSupweapon(int type)
{
	subWP = type;
}




void StatusBoard::Reset()
{
	time = 0;
}

DWORD StatusBoard::GetTime()
{
	return time;
}

void StatusBoard::SetTime(DWORD time)
{
	this->time = time;
}

void StatusBoard::SetSceneId(int id)
{
	sceneId = id;
}
