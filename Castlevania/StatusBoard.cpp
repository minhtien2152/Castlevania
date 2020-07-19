#include "StatusBoard.h"
#include"Define.h"
#include"time.h"
#include "Game.h"
#include "Sprites.h"
#define STATUS_BOARD_ID	101
#define BOSS_HEALTH 16
#define SCORE_PER_SEC 10
#define SCORE_PER_HEART 100


StatusBoard::StatusBoard(Simon* simon, LPGAMEOBJECT boss)
{
	Reset();
	this->simon = simon;
	simonbar = new HealthBar(100, 30, simon, SIMON_HEALTH);
	if(boss != NULL)
		bossbar = new HealthBar(100, 50, boss, ENEMY_HEALTH);
	animation_set = CAnimationSets::GetInstance()->Get(STATUS_BOARD_ID);
	item_animation_set = CAnimationSets::GetInstance()->Get(ITEM_ANI_SET_ID);
	subWeaponType = -1;
	timeAccumulated = 0;
}

StatusBoard::~StatusBoard()
{
}

void StatusBoard::Update(DWORD dt)
{
	
	simonbar->Update();
	if (bossbar != NULL)
		bossbar->Update();
	subWeaponCap = simon->GetSubWeaponCap();
	subWeaponType = simon->GetSubWeaponType();
	if (IsCalculatingHighScore)
	{
		int health = simon->GetHP();
		int heart = simon->GetHeartsCollected();
		int remain_time = GetRemainingTime();
		int score = simon->GetScore();
		if (health < SIMON_DEFAULT_HEALTH)
		{
			simon->SetHP(++health);
		}
		else if (remain_time > 0)
		{
			time += CLOCKS_PER_SEC;
			simon->SetScore(simon->GetScore() + SCORE_PER_SEC);
		}
		else {
			if (heart > 0)
			{
				simon->SetHeartsCollected(--heart);
				simon->SetScore(score + SCORE_PER_HEART);
			}
			else if (timeAccumulated > DEATH_TIMER_DELAY)
				CGame::GetInstance()->SwitchScene(INTRO_SCENE_ID);
			else {
				timeAccumulated += dt;
			}
		}

	}
	else
		time += dt;
}

void StatusBoard::Render()
{
	string score = FillStringWithZeros(to_string(simon->GetScore()),6);
	string hearts= FillStringWithZeros(to_string(simon->GetHeartsCollected()),2);
	string life = FillStringWithZeros(to_string(simon->GetLife()),2);
	string sceneID = FillStringWithZeros(to_string(sceneId), 2);

	int remainTime = GetRemainingTime();
	string time = FillStringWithZeros(to_string(remainTime), 3);

	info = "SCORE-" + score + " TIME " + time + " SCENE "+ sceneID  + "\n";
	info+= "PLAYER                  -" + hearts + "\n";
	
	info+= "ENEMY                   -" + life + "\n";

	RECT rect;
	SetRect(&rect, 0, 15, SCREEN_WIDTH, STATUS_BOARD_HEIGHT);

	if (font != NULL)
		font->DrawTextA(NULL, info.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	simonbar->Render();
	if (bossbar != NULL)
		bossbar->Render();
	animation_set->at(SUB_WEAPON_RECT_TEX_ID)->Render(290, 32,1, 1, false);
	if (subWeaponType !=-1)
		item_animation_set->at(subWeaponType)->Render(305, 38,1, 1, false);
	if(subWeaponCap ==2)
		item_animation_set->at(Item_Type::DOUBLESHOT)->Render(450, 38, 1, 1, false);
	else if(subWeaponCap ==3)
		item_animation_set->at(Item_Type::TRIPLESHOT)->Render(450, 38, 1, 1, false);

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

int StatusBoard::GetRemainingTime()
{
	return DEFAULT_TIME_PLAY - time / CLOCKS_PER_SEC;
}

void StatusBoard::SetSupweapon(int type)
{
	subWeaponType = type;
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

