#pragma once

#include <d3dx9.h>
#include "KeyHandler.h"
#include<string>
#include "Textures.h"
#include "Animations.h"
#include "StatusBoard.h"
#include "Camera.h"
#define SECTION_UNKNOWN					-1
#define SCENE_SECTION_RESOURCES			0	
#define SCENE_SECTION_OBJECTS			1
#define SCENE_SECTION_TILESET			2
#define SCENE_SECTION_TILEMAP_DATA		3

#define RESOURCES_SECTION_TEXTURES		4
#define RESOURCES_SECTION_SPRITES		5
#define RESOURCES_SECTION_ANIMATIONS	6
#define RESOURCES_SECTION_ANIMATION_SETS 7

#define MAX_SCENE_LINE 1024

using namespace std;
class CScene
{
protected:
	CKeyEventHandler* key_handler;
	int id;
	LPCWSTR sceneFilePath;
	Camera* camera;
	StatusBoard* statusboard;
public:
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler* GetKeyEventHandler() { return key_handler; }
	virtual void LoadScene()=0;
	
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void LoadObjectResourses(LPCWSTR filepath);
	virtual void _ParseSection_RESOURCES(string line);
	virtual void _ParseSection_TEXTURES(string line);
	virtual void _ParseSection_SPRITES(string line);
	virtual void _ParseSection_ANIMATIONS(string line);
	virtual void _ParseSection_ANIMATION_SETS(string line);
	virtual void _ParseSection_OBJECTS(string line)=0;
};
typedef CScene* LPSCENE;

class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScene* scence;

public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};