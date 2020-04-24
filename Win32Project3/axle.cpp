////////////////////////////////////////////////////////////////////////////////
// Filename: axleclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "axle.h"


Axle::Axle()
{
	Label_ = nullptr;
	Axle_ = nullptr;
	unit_ = "";
	maxValue_ = 255;
	minValue_ = 0;
	supportDivision_ = 1;
	mainDivision_ = 1;
	supportDivisionLenght_ = 1;
	mainDivisionLenght_ = 2;
	axleLenght_ = 255;
	division_type_ = DIVISION_TYPE::Const_Amount;
	validated_ = false;
	axlerotation_ = D3DXVECTOR3(0.f, 0.f, 0.f);
	axlebeginposition_ = D3DXVECTOR3(0.f, 0.f, 0.f);
	textOffset_ = D3DXVECTOR3(0.f, 0.f, 0.f);
	textposition_ = D3DXVECTOR3(0.f, 0.f, 0.f);
}

Axle::Axle(const Axle& other)
{
}

Axle::~Axle()
{
	Shutdown();
}

bool Axle::Initialize(D3DClass *direct, HWND hwnd)
{
	bool result = true;
	
	Axle_ = new VectorLineClass; 
	if (!Axle_) return false;

	Label_ = new Text;
	if (!Label_) return false;

	result &= Axle_->Initialize(direct, hwnd);

	result &= Label_->Initialize(direct, hwnd);
	if (!result) return false;

	Label_->SetText("test");
	Label_->color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	Axle_->Color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
	Axle_->SetPoints(2,D3DXVECTOR3(0.f, 0.f, 0.f),D3DXVECTOR3(axleLenght_, 0.f, 0.f));
	return true;
}

bool Axle::Render(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	D3DXMATRIX transformMatrix;
	Validate();
	D3DXMatrixRotationYawPitchRoll(&transformMatrix, axlerotation_.y, axlerotation_.x, axlerotation_.z);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &transformMatrix);
	return Axle_->Render(worldMatrix, viewMatrix, orthoMatrix);
}

bool Axle::RenderBanerLabel(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, D3DXVECTOR3 cameraRotation)
{
	D3DXMATRIX transformMatrix;
	
	Validate();

	D3DXMatrixRotationYawPitchRoll(&transformMatrix, cameraRotation.y*degToRad, cameraRotation.x*degToRad, cameraRotation.z*degToRad);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &transformMatrix);

	return Label_->Render(worldMatrix, viewMatrix, orthoMatrix);
}
void Axle::Shutdown()
{
	if (Axle_)
	{
		Axle_->Shutdown();
		delete Axle_;
		Axle_ = nullptr;
	}

	return;
}

void Axle::SetValuesRange(float min, float max, std::string unit)
{
	maxValue_ = max;
	minValue_ = min;
	unit_ = unit;
	validated_ = false;
	return;
}
void Axle::SetLenght(float l)
{
	axleLenght_ = l;
	validated_ = false;
	return;
}
void Axle::SetPosition(D3DXVECTOR3 pos)
{
	axlebeginposition_ = pos;
	validated_ = false;
	return;
}
void Axle::SetDirection(D3DXVECTOR3 angle)
{
	axlerotation_ = angle;
	validated_ = false;
	return;
}
void Axle::SetUnit(std::string unit)
{
	unit_ = unit;
	validated_ = false;
	return;
}
void Axle::SetTextSize(float size)
{
	Label_->scale = size;
	return;
}
void Axle::SetTextOffset(D3DXVECTOR3 offset)
{
	textOffset_ = offset;
	validated_ = false;
	return;
}
void Axle::SetSupportDivision(float dv)
{
	supportDivision_ = dv;
	validated_ = false;
	return;
}

void Axle::SetSupportDivisionLenght(float dvl)
{
	supportDivisionLenght_ = dvl;
	validated_ = false;
	return;
}

void Axle::SetMainDivision(float dv)
{
	mainDivision_ = dv;
	validated_ = false;
	return;
}

void Axle::SetMainDivisionLenght(float dvl)
{
	mainDivisionLenght_ = dvl;
	validated_ = false;
	return;
}

void Axle::SetDivisionType(DIVISION_TYPE typ)
{
	division_type_ = typ;
	validated_ = false;
	return;
}

bool Axle::IsValidated() 
{
	return validated_;
}

bool Axle::Validate()
{
	if (validated_) return true;
	D3DXQUATERNION transformVectQuaternion, transformQuaternion, transformQuaternionInv, qResult;
	bool result;
	switch (division_type_)
	{
	case Const_Amount:
		result=DivideCosntAmount();
		break;
	case Const_Value:
		result=DivideCosntValue();
		break;
	default:
		return false;
	}
	validated_ = true;

	transformVectQuaternion = D3DXQUATERNION(axleLenght_ / 2.f + textOffset_.x + axlebeginposition_.x, textOffset_.y + axlebeginposition_.y, textOffset_.z + axlebeginposition_.z + mainDivisionLenght_, 0.0f);
	D3DXQuaternionRotationYawPitchRoll(&transformQuaternionInv, axlerotation_.y, axlerotation_.x, axlerotation_.z);
	D3DXQuaternionNormalize(&transformQuaternionInv, &transformQuaternionInv);
	D3DXQuaternionInverse(&transformQuaternion, &transformQuaternionInv);

	qResult = transformQuaternion * transformVectQuaternion * transformQuaternionInv;
	Label_->position = D3DXVECTOR3(qResult.x, qResult.y + 7.0f, qResult.z);
	Label_->SetText(std::to_string(maxValue_) + unit_);
	return result;
}

bool Axle::DivideCosntAmount()
{
	D3DXVECTOR3 *vects = nullptr;
	float maindivide, supportdivide;
	int i = 0;
	int points = 2 + (int)mainDivision_ + ((int)supportDivision_ - 1) * (int)mainDivision_; //iloœæ segmentów, nie podzia³ek. 1 g³ówna + 2 na koñcach
	bool result;
	points *= 2;	
	maindivide = axleLenght_ / (int)mainDivision_;
	supportdivide = axleLenght_ / ((int)mainDivision_ * (int)supportDivision_);

	vects = new D3DXVECTOR3[points];
	if (!vects) return false;
	vects[i] = D3DXVECTOR3(0.f, 0.f, 0.f) + axlebeginposition_;
	vects[++i] = D3DXVECTOR3(axleLenght_, 0.f, 0.f) + axlebeginposition_;

	vects[++i] = D3DXVECTOR3(0.f, 0.f, 0.f) + axlebeginposition_;
	vects[++i] = D3DXVECTOR3(0.f, 0.f, mainDivisionLenght_) + axlebeginposition_;

	vects[++i] = D3DXVECTOR3(axleLenght_, 0.f, 0.f) + axlebeginposition_;
	vects[++i] = D3DXVECTOR3(axleLenght_, 0.f, mainDivisionLenght_) + axlebeginposition_;

	for (int d = (int)supportDivision_ * (int)mainDivision_ - 1; d > 0; --d)
	{
		for (int s = supportDivision_; s > 0;--d, --s)
		{
			vects[++i] = D3DXVECTOR3(d * supportdivide, 0.f, 0.f) + axlebeginposition_;
			vects[++i] = D3DXVECTOR3(d * supportdivide, 0.f, supportDivisionLenght_) + axlebeginposition_;

		}
		if (d > 0)
		{
			vects[++i] = D3DXVECTOR3(d * supportdivide, 0.f, 0.f) + axlebeginposition_;
			vects[++i] = D3DXVECTOR3(d * supportdivide, 0.f, mainDivisionLenght_) + axlebeginposition_;
		}
	}

	result = Axle_->SetPoints(points, vects);
	delete[] vects;
	return result;
}

bool Axle::DivideCosntValue()
{
	return false;
}