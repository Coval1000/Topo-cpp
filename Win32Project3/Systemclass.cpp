////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	_Input = 0;
	_Fps = 0;
	_Cpu = 0;
	_Timer = 0;
	_Position = 0;
	_Application = 0;
	_filename = 0;
	screenWidth_ = 0;
	screenHeight_ = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	SetProcessDPIAware();

	ApplicationHandle = this;
	_applicationName = L"Topo";

	wc = FillWndClass();
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, _hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}

bool SystemClass::Initialize()
{
	bool result;
	HRESULT hr;
	IClassFactory* pICF = nullptr;
	IRemote* pIRemote = nullptr;
	Window* window = nullptr;
	hr = CoInitialize(NULL);
	hr = CoGetClassObject(CLSID_Agent, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&pICF);
	if (FAILED(hr)) return false;
	hr = pICF->CreateInstance(NULL, IID_IRemote, (void**)&pIRemote);
	if (FAILED(hr)) 
	{ 
		pICF->Release();
		return false;
	}

	pIRemote->GetHandle(window);

	_hinstance = GetModuleHandle(NULL);

	if(!window)InitializeWindows(screenWidth_, screenHeight_);
	else
	{
		_hwnd = *((HWND*)(window->hwnd));
		screenHeight_ = window->height;
		screenWidth_ = window->width;
	}

	_Application = new ApplicationClass;
	if (!_Application)
	{
		return false;
	}

	BasicFileOpen(_filename);
	if (!_filename) return false;

	result = _Application->Initialize(_hinstance, _hwnd, screenWidth_, screenHeight_, _filename);
	if (!result)
	{
		return false;
	}

	pICF->Release();
	pIRemote->Release();
	window = nullptr;
	return true;
}

void SystemClass::Shutdown()
{
	CoTaskMemFree(_filename);
	// Release the application wrapper object.
	if (_Application)
	{
		_Application->Shutdown();
		delete _Application;
		_Application = 0;
	}

	// Release the position object.
	if (_Position)
	{
		delete _Position;
		_Position = 0;
	}

	// Release the timer object.
	if (_Timer)
	{
		delete _Timer;
		_Timer = 0;
	}

	// Release the cpu object.
	if (_Cpu)
	{
		_Cpu->Shutdown();
		delete _Cpu;
		_Cpu = 0;
	}

	// Release the fps object.
	if (_Fps)
	{
		delete _Fps;
		_Fps = 0;
	}


	
		// Release the input object.
		if (_Input)
		{
			_Input->Shutdown();
			delete _Input;
			_Input = 0;
		}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

bool SystemClass::Frame()
{
	bool /*keyDown,*/ result;
	//float rotationY;


	// Do the frame processing for the application object.
	result = _Application->Frame();
	if (!result)
	{
		return false;
	}

	if (_Application->IsOPressed())
	{
		_Application->Shutdown(); 
		delete _Application;
		_Application = 0;
		CoTaskMemFree(_filename);
		_filename = 0;


		_Application = new ApplicationClass;
		if (!_Application)
		{
			return false;
		}

		BasicFileOpen(_filename);
		if (!_filename)
		{
			return false;
		}

		result = _Application->Initialize(_hinstance, _hwnd, screenWidth_, screenHeight_, _filename);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(_hwnd);
	_hwnd = NULL;

	// Remove the application instance.
	if(_applicationName == L"Topo")UnregisterClass(_applicationName, _hinstance);
	_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

WNDCLASSEX SystemClass::FillWndClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	return wc;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_COPYDATA:
		pMyCDS = (PCOPYDATASTRUCT)lparam;
		switch (pMyCDS->dwData)
		{
		case 1:
			MessageBox(hwnd, (LPCWSTR)((MYREC *)(pMyCDS->lpData))->s1, L"TEST", MB_OK);
		}
		break;
	// All other messages pass to the message handler in the system class.
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}

void SystemClass::BasicFileOpen(LPWSTR filename)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->SetDefaultExtension(L"dat");

			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{

						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &_filename);

						pItem->Release();
					}
				}
				pFileOpen->Release();

			}
			CoUninitialize();

		}
	}
	return;
}