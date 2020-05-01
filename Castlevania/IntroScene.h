#pragma once
#include "Simon.h"
#include "Scene.h"
#include "Game.h"
#include "Textures.h"
#include "AnimationSets.h"
class CIntroScene:public CScene
{
	
	ID3DXFont* font;
	CGame* game;

	int state;

	LPANIMATION_SET animation_set;

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

