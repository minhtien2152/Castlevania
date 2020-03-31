#pragma once

#include "Game.h"
#include "Simon.h"
class Input : public CKeyEventHandler
{
	Simon* simon;
	CGame* game;
public:
	Input(Simon* s,CGame *g);
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

