////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	_Input = 0;
	_Direct3D = 0;
	_Camera = 0;	
	_ColorShader = 0;
	_Timer = 0;
	_Position = 0;
	_Fps = 0;
	_Cpu = 0;
	FontShader_ = 0;	
	_Data = 0;
	_DataHeader = 0;
	_gridOn = false;
	_gpressed = false;
	_saveTexture = false;
	mode_ = TerrainClass::DataMode::Extrapolator0;
	COLOR_MODE_ = Plain::GreyScale;
	overdrivez_ = 1.0f;
	
	_RenderMiniAxis = true;

	_RenderTexture = 0;
	_DebugWindow = 0;
	_TextureShader = 0;
	//Objects to render 
	_DebugLine = nullptr;
	_Terrain = nullptr;
	CpuUsage_ = nullptr;
	AxleX_ = nullptr;
	AxleY_ = nullptr;
	AxleZ_ = nullptr;
	_AxisMini = nullptr;
	
	//Buttons
	CameraDebugButton_ = nullptr;
	GridButton_ = nullptr;
	LoadComMapButton_ = nullptr;
	OverdriveNButton_ = nullptr;
	OverdrivePButton_ = nullptr;
	MiniAxisBgButton_ = nullptr;
	ModeButton_ = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
	Shutdown();
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, LPWSTR path)
{
	bool result;
	float cameraX, cameraY, cameraZ, rotX, rotY, rotZ;
	char videoCard[128];
	int videoMemory;
	screenheight_ = screenHeight;
	screenwidth_ = screenWidth;

	_Input = new InputClass;
	if (!_Input) return false;
	result = _Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	_Direct3D = new D3DClass;
	if (!_Direct3D) return false;
	result = _Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	_ColorShader = new ColorShaderClass;
	if (!_ColorShader) return false;
	result = _ColorShader->Initialize(_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	_Data = new string*;
	if (!_Data) return false;
	*_Data = 0;

	_DataHeader = new string*;
	if (!_DataHeader) return false;
	*_DataHeader = 0;

	_Terrain = new TerrainClass;
	if (!_Terrain) return false;
	result = _Terrain->Initialize(_Direct3D, _DataHeader, _Data, path);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	InitializeAxis(hwnd);

	_AxisMini = new AxisRosette;
	if (!_AxisMini) return false;
	result = _AxisMini->Initialize(_Direct3D, 30.0f, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the axis object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	_Camera = new CameraClass;
	if (!_Camera) return false;

	_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	_Camera->Render();
	_Camera->GetViewMatrix(_baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 0;
	cameraY = 0;
	cameraZ = 0;

	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
	
	_Camera->SetPosition(cameraX, cameraY, cameraZ);
	_Camera->SetRotation(rotX, rotY, rotZ);

	// Create the position object.
	_Position = new PositionClass;
	if (!_Position) return false;

	_Position->SetPosition(cameraX, cameraY, cameraZ);
	_Position->SetRotation(rotX, rotY, rotZ);
	_Position->SetOffSet(_Terrain->Plot.GetTerrainWidth() / 2.f, 0, _Terrain->Plot.GetTerrainHeight() / 2.f);
	_Position->SetRadious(_Terrain->Plot.GetTerrainWidth() / 2.f);
	_Position->orbit = true;

	_Timer = new TimerClass;
	if (!_Timer) return false;

	result = _Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the fps object.
	_Fps = new FpsClass;
	if (!_Fps)	return false;
	_Fps->Initialize();

	_Cpu = new CpuClass;
	if (!_Cpu)	return false;
	_Cpu->Initialize();

	result = InitializeText(hwnd);
	if (!result) return false;

	_DebugLine = new VectorLineClass;
	if (!_DebugLine) return false;
	result = _DebugLine->Initialize(_Direct3D, hwnd);
	_DebugLine->Color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	result = InitializeButtons(hwnd);
	if (!result) return false;

	//#########
	//
	//#########
	// Create the render to texture object.
	_RenderTexture = new RenderTextureClass;
	if (!_RenderTexture) return false;

	// Initialize the render to texture object.
	result = _RenderTexture->Initialize(_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result) return false;

	_DebugWindow = new DebugWindowClass;
	if (!_DebugWindow) return false;

	result = _DebugWindow->Initialize(_Direct3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	_TextureShader = new TextureShaderClass;
	if (!_TextureShader) return false;

	result = _TextureShader->Initialize(_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	_Direct3D->GetVideoCardInfo(videoCard, videoMemory);
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool ApplicationClass::InitializeAxis(HWND hwnd)
{
	bool result;
	AxleX_ = new Axle;
	if (!AxleX_) return false;
	result = AxleX_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the x axle object.", L"Error", MB_OK);
		return false;
	}
	AxleY_ = new Axle;
	if (!AxleY_) return false;
	result = AxleY_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the y axle object.", L"Error", MB_OK);
		return false;
	}
	AxleZ_ = new Axle;
	if (!AxleZ_) return false;
	result = AxleZ_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the z axle object.", L"Error", MB_OK);
		return false;
	}

	AxleX_->SetDirection(D3DXVECTOR3(D3DX_PI, 0.f, 0.f));
	AxleX_->SetMainDivision(5);
	AxleX_->SetSupportDivision(4);
	AxleX_->SetMainDivisionLenght(5);
	AxleX_->SetSupportDivisionLenght(3);
	AxleX_->SetPosition(D3DXVECTOR3(0, 0.f, 2.f));
	//AxleZ_->SetTextOffset(D3DXVECTOR3(100.f, 100.f, 100.f));

	AxleY_->SetValuesRange(_Terrain->GetBottom(), _Terrain->GetTop(), "um");
	AxleY_->SetLenght(_Terrain->GetTop() - _Terrain->GetBottom());
	AxleY_->SetPosition(D3DXVECTOR3(_Terrain->GetBottom(), 2.f, 2.f));
	AxleY_->SetDirection(D3DXVECTOR3(0.f, 0.f, D3DX_PI / 2.f));

	AxleZ_->SetDirection(D3DXVECTOR3(0.f, -D3DX_PI / 2.f, 0.f));
	AxleZ_->SetPosition(D3DXVECTOR3(0, 0.f, 2.f));
	return true;
}

bool ApplicationClass::InitializeButtons(HWND hwnd)
{
	bool result;

	GridButton_ = new Button(this);
	if (!GridButton_) return false;
	fPTR = &ApplicationClass::Button_Grid;
	result = GridButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	GridButton_->SetRectangle(int_RECT(680, 20, 70, 15));
	GridButton_->textColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	GridButton_->SetRectangleColor(D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));
	GridButton_->SetText("Siatka");
	//GridButton_->OnClick(&fPTR);
	//############
	//_OverdriveP
	//############
	OverdrivePButton_ = new Button(this);
	if (!OverdrivePButton_) return false;
	fPTR = (&ApplicationClass::Button_OverdriveP);
	result = OverdrivePButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	OverdrivePButton_->SetRectangle(int_RECT(750, 40, 10, 8));
	OverdrivePButton_->textColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	OverdrivePButton_->SetRectangleColor(D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));
	OverdrivePButton_->SetText("+");
	//############
	//_OverdriveN
	//############
	OverdriveNButton_ = new Button(this);
	if (!OverdriveNButton_) return false;
	fPTR = (&ApplicationClass::Button_OverdriveN);
	result = OverdriveNButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	OverdriveNButton_->SetRectangle(int_RECT(750, 48, 10, 7));
	OverdriveNButton_->textColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	OverdriveNButton_->SetRectangleColor(D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));
	OverdriveNButton_->SetText("-");
	//#########
	//Mini Axis
	//#########
	MiniAxisBgButton_ = new Button(this);
	if (!MiniAxisBgButton_) return false;
	fPTR = (&ApplicationClass::Button_MiniAxisBG);
	result = MiniAxisBgButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	MiniAxisBgButton_->SetRectangle(int_RECT(730, 265, 70, 70));
	MiniAxisBgButton_->textColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	MiniAxisBgButton_->SetRectangleColor(D3DXVECTOR4(0.9f, 0.9f, 0.9f, 0.5f));
	MiniAxisBgButton_->SetText(" ");
	//###########
	//CameraDebug
	//###########
	CameraDebugButton_ = new Button(this);
	if (!CameraDebugButton_) return false;
	fPTR = (&ApplicationClass::Button_CameraDebug);
	result = CameraDebugButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	CameraDebugButton_->SetRectangle(int_RECT(screenwidth_ - 50, screenheight_ - 15, 50, 15));
	CameraDebugButton_->textColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	CameraDebugButton_->SetRectangleColor(D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));
	CameraDebugButton_->SetText("Debug");

	//##########
	//LoadComMap
	//##########
	LoadComMapButton_ = new Button(this);
	if (!LoadComMapButton_) return false;
	fPTR = (&ApplicationClass::Button_LoadComMap);
	result = LoadComMapButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	LoadComMapButton_->SetRectangle(int_RECT(screenwidth_ - 60, screenheight_ - 30, 60, 15));
	LoadComMapButton_->textColor = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	LoadComMapButton_->SetRectangleColor(D3DXVECTOR4(0.6f, 0.8f, 0.8f, 1.0f));
	LoadComMapButton_->SetText("Load Com");
	LoadComMapButton_->draw = true;
	//
	//
	//
	ModeButton_ = new Button(this);
	if (!ModeButton_) return false;
	fPTR = (&ApplicationClass::Button_Mode);
	result = ModeButton_->Initialize(_Direct3D, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the button object.", L"Error", MB_OK);
		return false;
	}
	ModeButton_->SetRectangle(int_RECT(screenwidth_ - 60, screenheight_ - 45, 60, 15));
	ModeButton_->textColor = D3DXVECTOR4(0.8f, 1.0f, 1.8f, 1.0f);
	ModeButton_->SetRectangleColor(D3DXVECTOR4(0.0f, 0.0f, 0.8f, 1.0f));
	ModeButton_->SetText("Mode");
	ModeButton_->draw = true;
	return true;
}

bool ApplicationClass::InitializeText(HWND hwnd)
{
	bool result = true;

	CpuUsage_ = new Text;
	if (!CpuUsage_) return false;
	result &= CpuUsage_->Initialize(_Direct3D, hwnd);
	CpuUsage_->color = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	CpuUsage_->position = D3DXVECTOR3(-screenwidth_/2+20, screenheight_/2- 40, 0);
	CpuUsage_->SetText(std::to_string(_Cpu->GetCpuPercentage()));

	FramesPerSecond_ = new Text;
	if (!FramesPerSecond_) return false;
	result &= FramesPerSecond_->Initialize(_Direct3D, hwnd);
	FramesPerSecond_->color = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	FramesPerSecond_->position = D3DXVECTOR3(-screenwidth_ / 2 + 20, screenheight_ / 2 - 20, 0);
	FramesPerSecond_->SetText(std::to_string(_Cpu->GetCpuPercentage()));

	IntersectionDebug_ = new Text;
	if (!IntersectionDebug_) return false;
	result &= IntersectionDebug_->Initialize(_Direct3D, hwnd);
	IntersectionDebug_->color = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	IntersectionDebug_->position = D3DXVECTOR3(-screenwidth_ / 2 + 300, screenheight_ / 2 - 20, 0);
	IntersectionDebug_->SetText(" ");

	SensivityZ_ = new Text;
	if (!SensivityZ_) return false;
	result &= SensivityZ_->Initialize(_Direct3D, hwnd);
	SensivityZ_->color = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	SensivityZ_->position = D3DXVECTOR3(-screenwidth_ / 2 + screenwidth_ - 170, screenheight_ / 2 - 40, 0);
	SensivityZ_->SetText("Czulosc z: 1");

	GridDensity_ = new Text;
	if (!GridDensity_) return false;
	result &= GridDensity_->Initialize(_Direct3D, hwnd);
	GridDensity_->color = D3DXVECTOR4(0.f, 0.f, 1.f, 1.f);
	GridDensity_->position = D3DXVECTOR3(-screenwidth_ / 2 + screenwidth_ - 170, screenheight_ / 2 - 20, 0);
	GridDensity_->SetText("Wielkosc siatki: 1");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	return true;
}
//########
//Shutdown
//########
void ApplicationClass::ShutdownButtons()
{
	if (GridButton_)
	{
		GridButton_->Shutdown();
		delete GridButton_;
		GridButton_ = nullptr;
	}

	if (OverdriveNButton_)
	{
		OverdriveNButton_->Shutdown();
		delete OverdriveNButton_;
		OverdriveNButton_ = nullptr;
	}

	if (OverdrivePButton_)
	{
		OverdrivePButton_->Shutdown();
		delete OverdrivePButton_;
		OverdrivePButton_ = nullptr;
	}
	return;

	if (LoadComMapButton_)
	{
		LoadComMapButton_->Shutdown();
		delete LoadComMapButton_;
		LoadComMapButton_ = nullptr;
	}

}

void ApplicationClass::Shutdown()
{
	if (_TextureShader)
	{
		_TextureShader->Shutdown();
		delete _TextureShader;
		_TextureShader = 0;
	}

		// Release the debug window object.
	if (_DebugWindow)
	{
		_DebugWindow->Shutdown();
		delete _DebugWindow;
		_DebugWindow = 0;
	}

	// Release the render to texture object.
	if (_RenderTexture)
	{
		_RenderTexture->Shutdown();
		delete _RenderTexture;
		_RenderTexture = 0;
	}

	if (_Data)
	{
		delete[] * _Data;
		*_Data = 0;
		delete _Data;
		_Data = 0;
	}
	if (_DataHeader)
	{
		delete[] * _DataHeader;
		*_DataHeader = 0;
		delete _DataHeader;
		_DataHeader = 0;
	}
	// Release the text object.
	if (CpuUsage_)
	{
		CpuUsage_->Shutdown();
		delete CpuUsage_;
		CpuUsage_ = 0;
	}

	if (AxleX_)
	{
		AxleX_->Shutdown();
		delete AxleX_;
		AxleX_ = nullptr;
	}

	if (AxleY_)
	{
		AxleY_->Shutdown();
		delete AxleY_;
		AxleY_ = nullptr;
	}

	if (AxleZ_)
	{
		AxleZ_->Shutdown();
		delete AxleZ_;
		AxleZ_ = nullptr;
	}

	if (_AxisMini)
	{
		_AxisMini->Shutdown();
		delete _AxisMini;
		_AxisMini = nullptr;
	}
	// Release the font shader object.
	if (FontShader_)
	{
		FontShader_->Shutdown();
		delete FontShader_;
		FontShader_ = 0;
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

	// Release the position object.
	if (_Position)
	{
		delete _Position;
		_Position = 0;
	}

	if (_Timer)
	{
		delete _Timer;
		_Timer = 0;
	}

	if (_ColorShader)
	{
		_ColorShader->Shutdown();
		delete _ColorShader;
		_ColorShader = 0;
	}

	if (_Terrain)
	{
		_Terrain->Shutdown();
		delete _Terrain;
		_Terrain = 0;
	}

	// Release the camera object.
	if (_Camera)
	{
		delete _Camera;
		_Camera = 0;
	}

	// Release the Direct3D object.
	if (_Direct3D)
	{
		_Direct3D->Shutdown();
		delete _Direct3D;
		_Direct3D = 0;
	}

	// Release the input object.
	if (_Input)
	{
		_Input->Shutdown();
		delete _Input;
		_Input = 0;
	}

	ShutdownButtons();

	return;
}
//#####
//Frame
//#####
bool ApplicationClass::Frame()
{
	bool result;

	// Read the user input.
	result = _Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (_Input->KeyboardState(DIK_ESCAPE) == true)
	{
		return false;
	}

	// Update the system stats.
	_Timer->Frame();
	_Fps->Frame();
	_Cpu->Frame();

	result = FramesPerSecond_->SetText(std::to_string(_Fps->GetFps()));
	if (!result) return false;

	if (_Fps->GetFps() < 30) FramesPerSecond_->color = D3DXVECTOR4(1.f, 0.f, 0.f, 1.f);
	else if (_Fps->GetFps() < 60) FramesPerSecond_->color = D3DXVECTOR4(1.f, 1.f, 0.f, 1.f);
	else FramesPerSecond_->color = D3DXVECTOR4(0.f, 1.f, 0.f, 1.f);

	result = CpuUsage_->SetText(std::to_string(_Cpu->GetCpuPercentage()));
	if (!result) return false;

	result = HandleInput(_Timer->GetTime());
	if (!result) return false;

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool ApplicationClass::IsOPressed()
{
	return _Input->KeyboardState(DIK_O);
}

bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	static bool mouseRightDown, mouseLeftDown;
	int mouseX, mouseY, mouseZ;
	float posX, posY, posZ, rotX, rotY, rotZ;

	result = _Input->Frame();
	if (!result)
	{
		return false;
	}

	mouseZ = _Input->MouseMoveZ();
	if(mouseZ)_Position->IncRadious(mouseZ);

	// Check if the left mouse button has been pressed.
	if (_Input->IsLeftMouseButtonDown() == true)
	{		
		_Position->OrbitCamera(_Input->MouseMoveX(), _Input->MouseMoveY());
		_Position->GetPosition(posX, posY, posZ);
		
		if (mouseLeftDown == false)
		{
			mouseLeftDown = true;
			_Input->GetMouseLocation(mouseX, mouseY);
			GridButton_->Click(mouseX, mouseY);
			OverdrivePButton_->Click(mouseX, mouseY);
			OverdriveNButton_->Click(mouseX, mouseY);
			CameraDebugButton_->Click(mouseX, mouseY);			
			MiniAxisBgButton_->Click(mouseX, mouseY);
			LoadComMapButton_->Click(mouseX, mouseY);
			ModeButton_->Click(mouseX, mouseY);
		}
	}
	else 
	{
		_Input->ResetTranslation();
		mouseLeftDown = false;
	}

	if (_Input->IsRightMouseButtonDown())
	{
		if (mouseRightDown == false) 
		{
			_Input->GetMouseLocation(mouseX, mouseY);
			TestIntersection(mouseX, mouseY);
			mouseRightDown = true;
		}

	}
	else mouseRightDown = false;
	// Set the frame time for calculating the updated position.
	_Position->SetFrameTime(frameTime);

	//###############
	//Camera movement
	//###############
	if (false)
	{
		keyDown = _Input->KeyboardState(DIK_LEFTARROW);
		_Position->TurnLeft(keyDown);

		keyDown = _Input->KeyboardState(DIK_RIGHTARROW);
		_Position->TurnRight(keyDown);

		keyDown = _Input->KeyboardState(DIK_W);
		_Position->MoveForward(keyDown);

		keyDown = _Input->KeyboardState(DIK_S);
		_Position->MoveBackward(keyDown);

		keyDown = _Input->KeyboardState(DIK_E);
		_Position->MoveUpward(keyDown);

		keyDown = _Input->KeyboardState(DIK_A);
		_Position->MoveLeft(keyDown);

		keyDown = _Input->KeyboardState(DIK_D);
		_Position->MoveRight(keyDown);

		keyDown = _Input->KeyboardState(DIK_Q);
		_Position->MoveDownward(keyDown);

		keyDown = _Input->KeyboardState(DIK_UPARROW);
		_Position->LookUpward(keyDown);

		keyDown = _Input->KeyboardState(DIK_DOWNARROW);
		_Position->LookDownward(keyDown);
	}
	//####
	//Grid
	//####
	if (_Input->KeyboardState(DIK_G))
	{
		if (!_gpressed)
		{
			_gpressed = true;
			_gridOn = !_gridOn;
		}
	} else _gpressed = false;

	if (_Input->KeyboardState(DIK_PGUP))
	{
		_Terrain->Plot.ChangeGridSize(1);
		GridDensity_->SetText("Wielkosc siatki: " + std::to_string(_Terrain->Plot.GetGridDensity()));
	}

	if (_Input->KeyboardState(DIK_PGDN)) 
	{
		_Terrain->Plot.ChangeGridSize(-1);
		GridDensity_->SetText("Wielkosc siatki: " + std::to_string(_Terrain->Plot.GetGridDensity()));
	}
	//#############
	//Terrain Color
	//#############
	if (_Input->KeyboardState(DIK_Z) && COLOR_MODE_ != Plain::GreyScale)
	{
		result = _Terrain->Plot.ChangeColorMode(_Terrain->Plot.ColorMode::GreyScale);
		if(result) COLOR_MODE_ = Plain::GreyScale;
	}

	if (_Input->KeyboardState(DIK_X) && COLOR_MODE_ != Plain::HighContrast)
	{
		result = _Terrain->Plot.ChangeColorMode(_Terrain->Plot.ColorMode::HighContrast);
		if (result) COLOR_MODE_ = Plain::HighContrast;
	}

	if (_Input->KeyboardState(DIK_C) && COLOR_MODE_ != Plain::HCGSHybrid)
	{
		result = _Terrain->Plot.ChangeColorMode(_Terrain->Plot.ColorMode::HCGSHybrid);
		if (result) COLOR_MODE_ = Plain::HCGSHybrid;
	}

	if (_Input->KeyboardState(DIK_V) && COLOR_MODE_ != Plain::FullBit)
	{
		result = _Terrain->Plot.ChangeColorMode(_Terrain->Plot.ColorMode::FullBit);
		if (result) COLOR_MODE_ = Plain::FullBit;
	}

	if (_Input->KeyboardState(DIK_B) && COLOR_MODE_ != Plain::FullBitEyeCorrect)
	{
		result = _Terrain->Plot.ChangeColorMode(_Terrain->Plot.ColorMode::FullBitEyeCorrect);
		if (result) COLOR_MODE_ = Plain::FullBitEyeCorrect;
	}
	//###########
	//Overdrive Z
	//###########
	if (_Input->KeyboardState(DIK_NUMPADMINUS)) 
	{
		overdrivez_ += -0.05f;
		SensivityZ_->SetText("Czulosc Z: " + std::to_string(overdrivez_));
	}

	if (_Input->KeyboardState(DIK_NUMPADPLUS))
	{
		overdrivez_ += 0.05f;
		SensivityZ_->SetText("Czulosc Z: " + std::to_string(overdrivez_));
	}

	// Get the view point position/rotation.
	_Position->GetPosition(posX, posY, posZ);
	_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	_Camera->SetPosition(posX, posY, posZ);
	_Camera->SetRotation(rotX, rotY, rotZ);

	return true;
}

bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, worldMatrixPlaine, transformMatrix, transformMatrix2;
	D3DXVECTOR3 rotation;
	bool result;

	// Render the entire scene to the texture first.
	/*result = RenderToTexture();
	if (!result) return false;*/
	// Clear the scene.
	_Direct3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	// Render the scene as normal to the back buffer.
	//result = RenderScene();
	//if (!result) return false;

	_Camera->Render();

	_Direct3D->GetWorldMatrix(worldMatrix);
	_Camera->GetViewMatrix(viewMatrix);
	_Direct3D->GetProjectionMatrix(projectionMatrix);
	_Direct3D->GetOrthoMatrix(orthoMatrix);

	rotation = _Camera->GetRotation();

	_Direct3D->TurnOnAlphaBlending();

	
	//#######
	//Terrain
	//#######
	
	worldMatrixPlaine = worldMatrix;
	D3DXMatrixScaling(&transformMatrix, 1.0f, overdrivez_, 1.0f);
	D3DXMatrixMultiply(&worldMatrixPlaine, &worldMatrixPlaine, &transformMatrix);

	if (_gridOn)
	{

		worldMatrixPlaine._42 += 0.0078125f;		

		_Terrain->Grid.Render();

		result = _ColorShader->Render(_Direct3D->GetDeviceContext(), _Terrain->Grid.GetIndexCount(), worldMatrixPlaine, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}
	_Terrain->Plot.Render();
	result = _ColorShader->Render(_Direct3D->GetDeviceContext(), _Terrain->Plot.GetIndexCount(), worldMatrixPlaine, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = _DebugLine->Render(worldMatrix, viewMatrix, projectionMatrix);
	if (!result) return false;
	
	result = AxleX_->Render(worldMatrix, viewMatrix, projectionMatrix);
	result = AxleX_->RenderBanerLabel(worldMatrix, viewMatrix, projectionMatrix, rotation);

	result = AxleZ_->Render(worldMatrix, viewMatrix, projectionMatrix);
	result = AxleZ_->RenderBanerLabel(worldMatrix, viewMatrix, projectionMatrix, rotation);

	worldMatrixPlaine = worldMatrix;
	D3DXMatrixScaling(&transformMatrix, overdrivez_, 1.0f, 1.0f);
	D3DXMatrixMultiply(&worldMatrixPlaine, &worldMatrixPlaine, &transformMatrix);
	result = AxleY_->Render(worldMatrixPlaine, viewMatrix, projectionMatrix);
	result = AxleY_->RenderBanerLabel(worldMatrix, viewMatrix, projectionMatrix, rotation);
	if (!result) return false;
	//#########
	//2D Render
	//#########
	_Direct3D->TurnZBufferOff();
	MiniAxisBgButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (_RenderMiniAxis)
	{
		worldMatrixPlaine = worldMatrix;
		D3DXMatrixRotationY(&transformMatrix, -rotation.y * degToRad);
		D3DXMatrixMultiply(&worldMatrixPlaine, &worldMatrixPlaine, &transformMatrix);
		D3DXMatrixRotationX(&transformMatrix, -rotation.x * degToRad);
		D3DXMatrixMultiply(&worldMatrixPlaine, &worldMatrixPlaine, &transformMatrix);
		D3DXMatrixTranslation(&transformMatrix, 365, 0, 30);
		D3DXMatrixMultiply(&worldMatrixPlaine, &worldMatrixPlaine, &transformMatrix);
		_AxisMini->Render(worldMatrixPlaine, _baseViewMatrix, orthoMatrix);
	}
	//#######
	//Buttons
	//#######
	
	result = GridButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;
	//	
	result = OverdriveNButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;
	//	
	result = OverdrivePButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = CameraDebugButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = LoadComMapButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = ModeButton_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;
	//DEN
	//END
	//NDE
	result = CpuUsage_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = FramesPerSecond_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = IntersectionDebug_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = SensivityZ_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	result = GridDensity_->Render(worldMatrix, _baseViewMatrix, orthoMatrix);
	if (!result) return false;

	//// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = _DebugWindow->Render(_Direct3D->GetDeviceContext(), 50, 50);
	//if (!result) return false;

	//// Render the debug window using the texture shader.
	//result = _TextureShader->Render(_Direct3D->GetDeviceContext(), _DebugWindow->GetIndexCount(), worldMatrix, _baseViewMatrix,
	//	orthoMatrix, _RenderTexture->GetShaderResourceView());
	//if (!result)
	//{
	//	return false;
	//}

	_Direct3D->TurnOffAlphaBlending();

	_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	_Direct3D->EndScene();
	if (_saveTexture)
	{
		D3DX11SaveTextureToFile(_Direct3D->GetDeviceContext(), _RenderTexture->GetRenderTargetTexture(), D3DX11_IFF_PNG, L"test.png");
		_saveTexture = false;
	}
	return true;
}

bool ApplicationClass::RenderToTexture()
{
	bool result;


	// Set the render target to be the render to texture.
	_RenderTexture->SetRenderTarget(_Direct3D->GetDeviceContext(), _Direct3D->GetDepthStencilView());
		// Clear the render to texture.
	_RenderTexture->ClearRenderTarget(_Direct3D->GetDeviceContext(), _Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool ApplicationClass::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;


	// Generate the view matrix based on the camera's position.
	_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_Direct3D->GetWorldMatrix(worldMatrix);
	_Camera->GetViewMatrix(viewMatrix);
	_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	D3DXMatrixRotationY(&worldMatrix, rotation);


	_Terrain->Plot.Render();
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//_Model->Render(_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	//result = _LightShader->Render(_Direct3D->GetDeviceContext(), _Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	_Model->GetTexture(), _Light->GetDirection(), _Light->GetDiffuseColor());
	//if (!result)
	//{
	//	return false;
	//}

	D3DXMatrixScaling(&worldMatrix, 1.0f, overdrivez_, 1.0f);
	result = _ColorShader->Render(_Direct3D->GetDeviceContext(), _Terrain->Plot.GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	return true;
}

//##### DEN
//Frame END
//##### NDE
void ApplicationClass::TestIntersection(int mouseX, int mouseY)
{
	float pointX, pointY;
	D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix;
	D3DXVECTOR3 direction, origin, camera_rotation;
	D3DXVECTOR3 points[2];
	bool result;

	double fxAy, fxBy, fyAx, fyBx, fxAz, fxBz, fyAz, fyBz;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)screenwidth_) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)screenheight_) - 1.0f) * -1.0f;

	camera_rotation = _Camera->GetRotation();
	origin = _Camera->GetPosition();
	
	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	_Direct3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	_Camera->GetViewMatrix(viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	fxAy = direction.z / direction.x;
	fxBy = origin.z - origin.x * (direction.z / direction.x);

	fyAx = direction.x / direction.z;
	fyBx = origin.x - origin.z * (direction.x / direction.z);

	fyAz = direction.y / direction.z;
	fyBz = origin.y - origin.z * (direction.y / direction.z);

	fxAz = direction.y / direction.x;
	fxBz = origin.y - origin.x * (direction.y / direction.x);

	double equation1, equation2, equation3, equation4;

	switch ((int)(camera_rotation.y) / 45)
	{
	default:
	case 0:
		equation1 = fxAy * 1000 + fxBy;
		equation2 = fyAz * equation1 + fyBz;
		equation3 = fxAy * -1000 + fxBy;
		equation4 = fyAz * equation3 + fyBz;
		points[0] = D3DXVECTOR3(-1000, equation4, equation3 );
		points[1] = D3DXVECTOR3(1000, equation2, equation1);
		_DebugLine->SetPoints(2, points);
		for (int y = 0; _Terrain->Plot.GetTerrainHeight() > y; y++)
		{
			for (int i = 0; _Terrain->Plot.GetTerrainWidth() > i; i++)
			{
				
				if (TestIntersection_Check(i, y, direction, origin)) return;

			}
		}
		break;
		
	case 1:
	case 2:
		equation1 = fyAx * 1000 + fyBx;
		equation2 = fxAz * equation1 + fxBz;
		equation3 = fyAx * -1000 + fyBx;
		equation4 = fxAz * equation3 + fxBz;
		points[0] = D3DXVECTOR3(equation3, equation4, -1000);
		points[1] = D3DXVECTOR3(equation1, equation2, 1000);
		_DebugLine->SetPoints(2,points);
		for (int i = 0; _Terrain->Plot.GetTerrainHeight() > i; i++)
		{
			for (int j = _Terrain->Plot.GetTerrainWidth() - 1; 0 <= j; j--)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;

	case 3:
	case 4:
		equation1 = fxAy * 1000 + fxBy;
		equation2 = fyAz * equation1 + fyBz;
		equation3 = fxAy * -1000 + fxBy;
		equation4 = fyAz * equation3 + fyBz;
		points[0] = D3DXVECTOR3(-1000, equation4, equation3);
		points[1] = D3DXVECTOR3(1000, equation2, equation1);
		_DebugLine->SetPoints(2, points);
		for (int j = _Terrain->Plot.GetTerrainWidth() - 1; 0 <= j; j--)
		{
			for (int i = _Terrain->Plot.GetTerrainWidth() - 1; 0 <= i; i--)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;

	case 5:
	case 6:
		equation1 = fyAx * 1000 + fyBx;
		equation2 = fxAz * equation1 + fxBz;
		equation3 = fyAx * -1000 + fyBx;
		equation4 = fxAz * equation3 + fxBz;
		points[0] = D3DXVECTOR3(equation3, equation4, -1000);
		points[1] = D3DXVECTOR3(equation1, equation2, 1000);
		_DebugLine->SetPoints(2, points);
		for (int i = _Terrain->Plot.GetTerrainHeight() - 1; 0 <= i; i--)
		{
			for (int j = 0; _Terrain->Plot.GetTerrainHeight() > j; j++)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;
	}

	/*switch ((int)(camera_rotation.y)/90)
	{
	default:
	case 0:
		for (int j = 0; _Terrain->GetTerrainHeight() > j; j++)
		{
			for (int i = 0; _Terrain->GetTerrainWidth() > i; i++)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;

	case 1:
		for (int i = 0; _Terrain->GetTerrainHeight() > i; i++)
		{
			for (int j = _Terrain->GetTerrainWidth() - 1; 0 <= j; j--)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;

	case 2:
		for (int j = _Terrain->GetTerrainWidth() - 1; 0 <= j; j--)
		{
			for (int i = _Terrain->GetTerrainWidth() - 1; 0 <= i; i--)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;

	case 3:
		for (int i = _Terrain->GetTerrainHeight() - 1; 0 <= i; i--)
		{
			for (int j = 0; _Terrain->GetTerrainHeight() > j; j++)
			{
				if (TestIntersection_Check(i, j, direction, origin)) return;
			}
		}
		break;
	}
*/
	// If not then set the intersection to "No".
	result = IntersectionDebug_->SetText(string("false ") + to_string(mouseX) + string(",") + to_string(mouseY));
	_Terrain->HighlightNode(POINT{ -1, -1 });
	return;
}

bool ApplicationClass::TestIntersection_Check(int i, int j, D3DXVECTOR3 direction, D3DXVECTOR3 origin)
{
	bool result;
	D3DXMATRIX worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 rayDirection, rayOrigin;

	// Get the world matrix and translate to the location of the sphere.
	_Direct3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&translateMatrix, i, _Terrain->Plot.GetTerrainValue(j, i) * overdrivez_, j);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	// Now get the inverse of the translated world matrix.
	D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
	D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDirection, &rayDirection);

	if (RaySphereIntersect(rayOrigin, rayDirection, 0.5f))
	{
		// If it does intersect then set the intersection to "yes" in the text string that is displayed to the screen.
		result = IntersectionDebug_->SetText(string("true ") + to_string(i) + string(", ") + to_string(_Terrain->Plot.GetTerrainValue(j, i)) + string(", ") + to_string(j));
		_Terrain->HighlightNode(POINT{ j, i });
		_Position->MoveOffSet(i, _Terrain->Plot.GetTerrainValue(j, i) * overdrivez_, j);
		return true;
	}
	return false;
}

bool ApplicationClass::RaySphereIntersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, float radius)
{
	float a, b, c, discriminant;


	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

//#######
//Buttons
//#######

void ApplicationClass::Button_Grid()
{
	_gridOn = !_gridOn;
	return;
}

void ApplicationClass::Button_OverdriveP()
{
	overdrivez_ += +0.05f;
	SensivityZ_->SetText("Czulosc Z: " + std::to_string(overdrivez_));
	return;
}

void ApplicationClass::Button_OverdriveN()
{
	overdrivez_ += -0.05f;
	SensivityZ_->SetText("Czulosc Z: " + std::to_string(overdrivez_));
	return;
}

void ApplicationClass::Button_CameraDebug()
{
	if (_DebugLine->draw)
	{
		_DebugLine->draw = false;
		CameraDebugButton_->SetRectangleColor(D3DXVECTOR4(0.8f, 0.5f, 0.5f, 1.0f));
	}
	else 
	{
		_DebugLine->draw = true;
		CameraDebugButton_->SetRectangleColor(D3DXVECTOR4(0.5f, 0.8f, 0.5f, 1.0f));
	}
	//_saveTexture = true;
	return;
}

void ApplicationClass::Button_MiniAxisBG()
{
	if (_RenderMiniAxis)
	{
		_RenderMiniAxis = false;
		MiniAxisBgButton_->SetPosition(790, 265);
	}
	else
	{
		_RenderMiniAxis = true;
		MiniAxisBgButton_->SetPosition(730, 265);
	}
	return;
}

void ApplicationClass::Button_LoadComMap()
{
	_Terrain->LoadComMap();
	return;
}

void ApplicationClass::Button_Mode()
{
	switch (mode_)
	{
	case TerrainClass::DataMode::Extrapolator1Interpolated:
		mode_ = TerrainClass::DataMode::Extrapolator0;
		_Terrain->Plot.SetMode(Plain::DataMode::Extrapolator0);
		_Terrain->Grid.SetMode(Plain::DataMode::Extrapolator0);
		break;
	case TerrainClass::DataMode::Extrapolator0:
		mode_ = TerrainClass::DataMode::Extrapolator1;
		_Terrain->Plot.SetMode(Plain::DataMode::Extrapolator1);
		_Terrain->Grid.SetMode(Plain::DataMode::Extrapolator1);
		break;
	case TerrainClass::DataMode::Extrapolator1:
		mode_ = TerrainClass::DataMode::Extrapolator1Interpolated;
		_Terrain->Plot.SetMode(Plain::DataMode::Extrapolator1Interpolated);
		_Terrain->Grid.SetMode(Plain::DataMode::Extrapolator1Interpolated);
		break;
	}
	return;
}
//########
//Mutators
//########

void ApplicationClass::Screen(int &x, int &y)
{
	x = screenwidth_;
	y = screenheight_;
	return;
}

