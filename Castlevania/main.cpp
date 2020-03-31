/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Animations.h"
#include "Sprites.h"
#include "Simon.h"
#include "Zombie.h"
#include "Brick.h"

CGame* game;
Simon* simon;
CZombie* zombie;
vector<LPGAMEOBJECT> objects;
class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->isJumping||simon->IsAttacking()||simon->GetState()==SIMON_SIT)
			return;
		simon->SetState(SIMON_JUMP);
		DebugOut(L"[INFO] JUMP ");
		break;
	case DIK_X:
		if (simon->IsAttacking())
			return;
		if (simon->GetState() == SIMON_WALK)
			simon->SetState(SIMON_STAND);
		if (simon->GetState() == SIMON_STAND || simon->GetState() == SIMON_JUMP|| simon->GetState()==SIMON_WALK)
		{
			simon->SetState(SIMON_STAND_ATTACK);
		}
		else if (simon->GetState() == SIMON_SIT)
		{
			simon->SetState(SIMON_SIT_ATTACK);
		}
		DebugOut(L"[INFO] Attack");
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	if (simon->GetState() == SIMON_STAND_ATTACK && simon->animations[SIMON_STAND_ATTACK]->IsOver() == false)
		return;//chi can else 1 lan 
	if (simon->GetState() == SIMON_SIT_ATTACK && simon->animations[SIMON_SIT_ATTACK]->IsOver() == false)
		return;//
	
	if (simon->isJumping)
		return;
	
	if (game->IsKeyDown(DIK_DOWN))
	{
		simon->SetState(SIMON_SIT);
		if (game->IsKeyDown(DIK_RIGHT))
		{
			simon->SetDirection(-1);

		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			simon->SetDirection(1);
		}
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetDirection(-1);
		simon->SetState(SIMON_WALK);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetDirection(1);
		simon->SetState(SIMON_WALK);
	}
	else simon->SetState(SIMON_STAND);
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(1, L"Resources\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(2, L"Resources\\Ground.png", D3DCOLOR_XRGB(255,0,255));
	textures->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(1);

	// readline => id, left, top, right 

	//walk
	sprites->Add(10001, 0, 0, 60, 64, texSimon); //idle
	sprites->Add(10002, 60,	0, 120, 64, texSimon);
	sprites->Add(10003, 120, 0, 180, 64, texSimon);
	sprites->Add(10004, 180, 0, 240, 64, texSimon);

	//sit
	sprites->Add(20001, 302, 198, 360, 264, texSimon);
	//jump
	sprites->Add(30001, 240, 0, 300, 64, texSimon);
	//sit_atk
	sprites->Add(40001, 420	,66	,480,	130, texSimon);
	sprites->Add(40002, 0	,132,	60	,196, texSimon);
	sprites->Add(40003, 60,	132	,120	,196, texSimon);
	//stand_atk
	sprites->Add(50001, 300,	0,	360,	64, texSimon);
	sprites->Add(50002, 360,	0,	420,	64, texSimon);
	sprites->Add(50003, 420,	0,	480,	64, texSimon);
		
	LPDIRECT3DTEXTURE9 texGround = textures->Get(2);
	sprites->Add(60001, 32	,0	,64	,32, texGround);

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(10001);
	animations->Add(500, ani);

	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(10004);
	animations->Add(501, ani);

	ani = new CAnimation(100);
	ani->Add(20001);
	animations->Add(502, ani);

	ani = new CAnimation(100);
	ani->Add(30001);
	animations->Add(503, ani);

	ani = new CAnimation(100);
	ani->Add(40001);
	ani->Add(40002);
	ani->Add(40003);
	animations->Add(504, ani);

	ani = new CAnimation(100);
	ani->Add(50001);
	ani->Add(50002);
	ani->Add(50003);
	animations->Add(505, ani);

	ani = new CAnimation(100);		// brick
	ani->Add(60001);
	animations->Add(601, ani);

	simon = new Simon();
	simon->AddAnimation(500);
	simon->AddAnimation(501);
	simon->AddAnimation(502);
	simon->AddAnimation(503);
	simon->AddAnimation(504);
	simon->AddAnimation(505);
	simon->SetPosition(50.0f, 0);
	objects.push_back(simon);

	for (int i = 0; i < 5; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(100.0f + i * 32.0f, 74.0f);
		objects.push_back(brick);

		/*brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(100.0f + i * 32.0f, 90.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(84.0f + i * 32.0f, 90.0f);
		objects.push_back(brick);*/
	}


	for (int i = 0; i < 30; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(0 + i * 32.0f, 150);
		objects.push_back(brick);
	}

	// and Goombas 

		//zombie = new CZombie();
		//zombie->AddAnimation(701);
		//zombie->SetPosition(200 , 135);
		//objects.push_back(zombie);
	

}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow mario
	float cx, cy;
	simon->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	CGame::GetInstance()->SetCamPos(cx,cy);
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		//
		// TEST SPRITE DRAW
		//

		
	/*	CTextures *textures = CTextures::GetInstance();

		D3DXVECTOR3 p(20, 20, 0);
		RECT r;
		r.left = 0;	
		r.top = 0;
		r.right = 32;
		r.bottom = 32;
		spriteHandler->Draw(textures->Get(2), &r, NULL, &p, D3DCOLOR_XRGB(0, 0, 0));
		*/

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();
	Run();

	return 0;
}