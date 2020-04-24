////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"

PositionClass::PositionClass()
{
	orbit = false;
	_frameTime = 0.0f;

	_positionX = 0.0f;
	_positionY = 0.0f;
	_positionZ = 0.0f;

	_rotationX = 0.0f;
	_rotationY = 0.0f;
	_rotationZ = 0.0f;

	_forwardSpeed = 0.0f;
	_backwardSpeed = 0.0f;
	_upwardSpeed = 0.0f;
	_downwardSpeed = 0.0f;
	_leftTurnSpeed = 0.0f;
	_rightTurnSpeed = 0.0f;
	_lookUpSpeed = 0.0f;
	_lookDownSpeed = 0.0f;
	_leftSpeed = 0.0f;
	_rightSpeed = 0.0f;
	_moveSpeed = 0.0f;
	_offX = 0.0f;
	_offY = 0.0f;
	_offZ = 0.0f;

}

PositionClass::PositionClass(const PositionClass& other)
{
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetRadious(float r)
{
	_radious = r;
	if (orbit)Sphere();
	return;
}

void PositionClass::SetOffSet(float x, float y, float z)
{	
	_positionX += x - _offX;
	_positionY += y - _offY;
	_positionZ += z - _offZ;
	_offX = x;
	_offY = y;
	_offZ = z;
	if (orbit)Sphere();
	return;
}

void PositionClass::SetPosition(float x, float y, float z)
{
	_positionX = x + _offX;
	_positionY = y + _offY;
	_positionZ = z + _offZ;
	if (orbit)Sphere();
	return;
}

void PositionClass::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;
	if (orbit)Sphere();
	return;
}


void PositionClass::MoveOffSet(float x, float y, float z)
{
	_offX = x;
	_offY = y;
	_offZ = z;
	_radious = sqrt(pow(_positionX - _offX,2) + pow(_positionY - _offY , 2) + pow(_positionZ - _offZ, 2));	
	if (orbit)SpinCamera();
	return;
}


float PositionClass::GetRadious()
{
	return _radious;
}

void PositionClass::GetOffSet(float& x, float& y , float&z )
{
	x = _offX;
	y = _offY;
	z = _offZ;
	return;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = _positionX;
	y = _positionY;
	z = _positionZ;
	return;
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = _rotationX;
	y = _rotationY;
	z = _rotationZ;
	return;
}

void PositionClass::SetFrameTime(float time)
{
	_frameTime = time;
	return;
}

void PositionClass::GetRotation(float& y)
{
	y = _rotationY;
	return;
}
//###########
//Translation
//###########
void PositionClass::MoveForwBack(int val)
{
	float radians, radiansPitch;

	if (val != 0)
	{
		_moveSpeed += val * _frameTime * 0.002f;
	}
	else
	{
		_moveSpeed = 0.0f;
	}

	radiansPitch = _rotationX * 0.0174532925f;
	_positionY -= sinf(radiansPitch) * _moveSpeed;

	radians = _rotationY * 0.0174532925f;
	_positionX += cosf(radiansPitch) * sinf(radians) * _moveSpeed;
	_positionZ += cosf(radiansPitch) * cosf(radians) * _moveSpeed;

	return;
}

void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_forwardSpeed += _frameTime * 0.001f;

		if (_forwardSpeed > (_frameTime * 0.03f))
		{
			_forwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_forwardSpeed -= _frameTime * 0.0007f;

		if (_forwardSpeed < 0.0f)
		{
			_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = _rotationY * 0.0174532925f;

	// Update the position.
	_positionX += sinf(radians) * _forwardSpeed;
	_positionZ += cosf(radians) * _forwardSpeed;

	return;
}

void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_backwardSpeed += _frameTime * 0.001f;

		if (_backwardSpeed > (_frameTime * 0.03f))
		{
			_backwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_backwardSpeed -= _frameTime * 0.0007f;

		if (_backwardSpeed < 0.0f)
		{
			_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = _rotationY * 0.0174532925f;

	// Update the position.
	_positionX -= sinf(radians) * _backwardSpeed;
	_positionZ -= cosf(radians) * _backwardSpeed;

	return;
}

void PositionClass::MoveLeft(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_leftSpeed += _frameTime * 0.001f;

		if (_leftSpeed > (_frameTime * 0.03f))
		{
			_leftSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_leftSpeed -= _frameTime * 0.0007f;

		if (_leftSpeed < 0.0f)
		{
			_leftSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = _rotationY * 0.0174532925f;

	// Update the position.
	_positionX -= cosf(radians) * _leftSpeed;
	_positionZ += sinf(radians) * _leftSpeed;

	return;
}

void PositionClass::MoveRight(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_rightSpeed += _frameTime * 0.001f;

		if (_rightSpeed > (_frameTime * 0.03f))
		{
			_rightSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_rightSpeed -= _frameTime * 0.0007f;

		if (_rightSpeed < 0.0f)
		{
			_rightSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = _rotationY * 0.0174532925f;

	// Update the position.
	_positionX += cosf(radians) * _rightSpeed;
	_positionZ -= sinf(radians) * _rightSpeed;

	return;
}

void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_upwardSpeed += _frameTime * 0.003f;

		if (_upwardSpeed > (_frameTime * 0.03f))
		{
			_upwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_upwardSpeed -= _frameTime * 0.002f;

		if (_upwardSpeed < 0.0f)
		{
			_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	_positionY += _upwardSpeed;

	return;
}

void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_downwardSpeed += _frameTime * 0.003f;

		if (_downwardSpeed > (_frameTime * 0.03f))
		{
			_downwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_downwardSpeed -= _frameTime * 0.002f;

		if (_downwardSpeed < 0.0f)
		{
			_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	_positionY -= _downwardSpeed;

	return;
}
//########
//Rotation
//########
void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_leftTurnSpeed += _frameTime * 0.01f;

		if (_leftTurnSpeed > (_frameTime * 0.15f))
		{
			_leftTurnSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_leftTurnSpeed -= _frameTime* 0.005f;

		if (_leftTurnSpeed < 0.0f)
		{
			_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	_rotationY -= _leftTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (_rotationY < 0.0f)
	{
		_rotationY += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_rightTurnSpeed += _frameTime * 0.01f;

		if (_rightTurnSpeed > (_frameTime * 0.15f))
		{
			_rightTurnSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_rightTurnSpeed -= _frameTime* 0.005f;

		if (_rightTurnSpeed < 0.0f)
		{
			_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	_rotationY += _rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (_rotationY > 360.0f)
	{
		_rotationY -= 360.0f;
	}

	return;
}

void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_lookUpSpeed += _frameTime * 0.01f;

		if (_lookUpSpeed > (_frameTime * 0.15f))
		{
			_lookUpSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_lookUpSpeed -= _frameTime* 0.005f;

		if (_lookUpSpeed < 0.0f)
		{
			_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	_rotationX -= _lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if (_rotationX < -90.0f)
	{
		_rotationX = -90.0f;
	}

	return;
}

void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		_lookDownSpeed += _frameTime * 0.01f;

		if (_lookDownSpeed > (_frameTime * 0.15f))
		{
			_lookDownSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_lookDownSpeed -= _frameTime* 0.005f;

		if (_lookDownSpeed < 0.0f)
		{
			_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	_rotationX += _lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if (_rotationX > 90.0f)
	{
		_rotationX = 90.0f;
	}

	return;
}
//######## DEN
//Rotation END
//######## NDE
void PositionClass::IncRadious(float dR) 
{
	_radious -= dR * 0.05;
	Sphere();
	return;
}

void PositionClass::OrbitCamera(int x, int y)
{
	float speed = 0.2;
	_rotationY += speed * x;
	_rotationX += speed * y;
	if (_rotationX > 90.0) _rotationX = 90.0;
	else if (_rotationX < -15.0) _rotationX = -15.0;
	Sphere();
	return;
}

void PositionClass::Sphere()
{
	_positionX = _offX - _radious * cos((-_rotationY+90) * (D3DX_PI/180.0)) * cos(_rotationX * (D3DX_PI / 180.0));
	_positionY = _offY + _radious * sin(_rotationX * (D3DX_PI / 180.0));
	_positionZ = _offZ - _radious * sin((-_rotationY + 90) * (D3DX_PI / 180.0)) * cos(_rotationX * (D3DX_PI / 180.0));
	return;
}

void PositionClass::SpinCamera()
{
	double r;
	float x, z;
	x = _positionX - _offX;
	z = _positionZ - _offZ;
	r = sqrt(pow(x, 2) + pow(z, 2));
	_rotationY = atan2(x,z) * 180.0/D3DX_PI + 180.0;
	_rotationX = atan2(_positionY - _offY,r) * 180.0 / D3DX_PI;
	return;
}