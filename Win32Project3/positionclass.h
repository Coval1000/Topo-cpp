////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>
#define D3DX_PI    (3.14159265358979323846)
////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	bool orbit;

	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	void GetRotation(float&);

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetOffSet(float, float, float);
	void SetRadious(float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);
	void GetOffSet(float&, float&, float&);
	float GetRadious();

	void MoveOffSet(float, float, float);

	void IncRadious(float);
	void OrbitCamera(int, int );

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveForwBack(int);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void MoveLeft(bool);
	void MoveRight(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);
	void SpinCamera();

private:
	void Sphere();

	float _frameTime;
	float _radious;

	float _offX, _offY, _offZ;
	float _positionX, _positionY, _positionZ;
	float _rotationX, _rotationY, _rotationZ;

	float _leftSpeed, _rightSpeed;
	float _forwardSpeed, _backwardSpeed;
	float _upwardSpeed, _downwardSpeed;
	float _leftTurnSpeed, _rightTurnSpeed;
	float _lookUpSpeed, _lookDownSpeed;
	float _moveSpeed;
	
};


#endif