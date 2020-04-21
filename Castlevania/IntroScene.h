#pragma once
#include "Simon.h"
#include "Scene.h"
#include "Game.h"
#include "Textures.h"
#include "AnimationSets.h"
class CIntroScene:public CScene
{
	Simon* simon;
	ID3DXFont* font;
	CGame* game;

	int state;
	LPDIRECT3DTEXTURE9 menuScreen;
	LPDIRECT3DTEXTURE9 castleBackground;
	LPANIMATION_SET mainMenuAni;

	DWORD timeAccumulated = 0;
public:
	CIntroScene(int id, LPCWSTR filePath) ;
	virtual void LoadScene();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void _ParseSection_OBJECTS(string line);
	friend class CIntroScenceKeyHandler;
};

class CIntroScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

