#include "KeyHandler.h"

void Input::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_JUMP);
		break;
	}
}

void Input::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}



Input::Input(Simon* s, CGame* g)
{
	simon = s;
	game = g;
}

void Input::KeyState(BYTE* states)
{
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_WALK);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_WALK);
	else simon->SetState(SIMON_STAND);
}