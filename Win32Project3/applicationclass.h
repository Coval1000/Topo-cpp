#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////


#ifndef _APPLICATIONCLASS_H_

#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100000.0f;
const float SCREEN_NEAR = 0.1f;

#include <string>
#include <algorithm>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "cameraclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"
#include "axle.h"
//Shader
#include "textureshaderclass.h"
#include "fontshaderclass.h"
#include "colorshaderclass.h"
//Render object
#include "terrainclass.h"
#include "textclass.h"
#include "axisrosette.h"
#include "VectorLineClass.h"
#include "RectangleClass.h"
#include "TriangleClass.h"

class ApplicationClass;
typedef void(ApplicationClass::*On_Click)();
struct int_RECT;
#include "button.h"
////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
private: 
public:
	typedef struct tagRGBTriplet
	{
		BYTE red;
		BYTE green;
		BYTE blue;
	} RGBTriplet;

	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int, LPWSTR);
	void Shutdown();
	void ShutdownButtons();
	bool Frame();
	bool IsOPressed();

	void Button_Grid();
	void Button_OverdriveP();
	void Button_OverdriveN();
	void Button_CameraDebug();
	void Button_MiniAxisBG();
	void Button_LoadComMap();
	void Button_Mode();
	On_Click fPTR;

	void Screen(int&, int&);
private:
	bool RenderToTexture();
	bool RenderScene();

private:
	void TestIntersection(int, int);
	bool TestIntersection_Check(int, int, D3DXVECTOR3, D3DXVECTOR3);
	bool RaySphereIntersect(D3DXVECTOR3, D3DXVECTOR3, float);
	bool HandleInput(float);
	bool RenderGraphics();

	bool InitializeButtons(HWND);
	bool InitializeText(HWND);
	bool InitializeAxis(HWND);

private:
	float overdrivez_;
	InputClass* _Input;
	D3DClass* _Direct3D;
	CameraClass* _Camera;
	ColorShaderClass* _ColorShader;
	TimerClass* _Timer;
	PositionClass* _Position;
	FpsClass* _Fps;
	CpuClass* _Cpu;
	FontShaderClass* FontShader_;

	string** _Data;
	string** _DataHeader;
	bool _gridOn;
	bool _gpressed, _RenderMiniAxis;
	bool _saveTexture;
	TerrainClass::DataMode mode_;
	int screenwidth_, screenheight_;
	Plain::ColorMode COLOR_MODE_;

	D3DXMATRIX _baseViewMatrix;

	RenderTextureClass* _RenderTexture;
	DebugWindowClass* _DebugWindow;
	TextureShaderClass* _TextureShader;

	//Obejcts to render
	TerrainClass* _Terrain;

	Text* CpuUsage_;
	Text* FramesPerSecond_;
	Text* IntersectionDebug_;
	Text* SensivityZ_;
	Text* GridDensity_;

	Axle *AxleX_, *AxleY_, *AxleZ_;
	AxisRosette* _AxisMini;

	Button *GridButton_;
	Button *OverdrivePButton_;
	Button *OverdriveNButton_;
	Button *CameraDebugButton_;
	Button *MiniAxisBgButton_;
	Button *LoadComMapButton_;
	Button *ModeButton_;
	VectorLineClass *_DebugLine;

};

struct int_RECT 
{
	int top;	
	int left;
	int height;
	int width;

	int_RECT(int left, int top, int w, int h) 
	{
		this->top = top;
		this->left = left;
		height = h;
		width = w;
	}

	int_RECT(int_RECT &rect)
	{
		this->top = rect.top;
		this->left = rect.left;
		this->height = rect.height;
		this->width = rect.width;
	}
};
#endif