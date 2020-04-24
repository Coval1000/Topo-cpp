#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <shobjidl.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "applicationclass.h"

//#####
//MACRO
//#####
typedef struct tagMYREC
{
	wchar_t  s1[80];
	wchar_t  s2[80];
	DWORD n;
} MYREC;
PCOPYDATASTRUCT pMyCDS;
////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void BasicFileOpen(LPWSTR);

	LPWSTR _filename;
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	WNDCLASSEX FillWndClass();
private:
	LPCWSTR _applicationName;
	int screenWidth_, screenHeight_;
	HINSTANCE _hinstance;
	HWND _hwnd;

	InputClass* _Input;
	FpsClass* _Fps;
	CpuClass* _Cpu;
	TimerClass* _Timer;
	PositionClass* _Position;
	ApplicationClass* _Application;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif