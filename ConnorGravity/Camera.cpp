
#include "camera.h"

#define dtr(x) ((x) * (3.14159265/180.0))

CCamera & CCamera::operator= (const CCamera & o)
{
	position = o.position;
	rotation = o.rotation;
	moveX = o.moveX;
	moveY = o.moveY;
	moveZ = o.moveZ;
	mouseDampening = o.mouseDampening;
	autoUpdateMouseRelationalPoint = o.autoUpdateMouseRelationalPoint;
	mouseRelationalPoint = o.mouseRelationalPoint;
	return *this;
}

CCamera::CCamera ()
{
	rotation.x() = 0;
	rotation.y() = 0;
	rotation.z() = 0;

	position.x() = 0;
	position.y() = 0;
	position.z() = 0;

	moveX = true;
	moveZ = true;
	moveY = false;

	mouseDampening = 0.1f;
	autoUpdateMouseRelationalPoint = false;

	useMouseSmoothing = false;
	mouseVelocity = vec3f::ZERO;
	mouseVelocityDampening = 0.07f;
	mouseVelocityFriction = 0.5f;
}

CCamera & CCamera::Move (float distance, float relativeDirection)
{
	float rotX = 0.0f;

	if (moveY)
	{
		//	TODO: Uses incorrect equation for spherical coordinates, implement
		//		using CVector::VectorFromSphericalCoordinates
		rotX = rotation.x();
		position.y() += sinf (dtr (rotation.x())) * distance;
	}

	if (moveX)
		position.x() += cosf (dtr (rotation.y() + relativeDirection)) * distance * cosf (dtr (rotX));
	if (moveZ)
		position.z() -= sinf (dtr (rotation.y() + relativeDirection)) * distance * cosf (dtr (rotX));

	return *this;
}

CCamera & CCamera::MoveForward (float distance)
{
	return Move (distance, 90);
}

CCamera & CCamera::MoveBackward (float distance)
{
	return Move (distance, 270);
}

CCamera & CCamera::MoveRight (float distance)
{
	return Move (distance, 0);
}

CCamera & CCamera::MoveLeft (float distance)
{
	return Move (distance, 180);
}

CCamera & CCamera::SetMoveX (bool yes)
{
	moveX = yes;
	return *this;
}

CCamera & CCamera::SetMoveY (bool yes)
{
	moveY = yes;
	return *this;
}

CCamera & CCamera::SetMoveZ (bool yes)
{
	moveZ = yes;
	return *this;
}

CCamera & CCamera::TurnOnX (float amount)
{
	rotation.x() += amount;

	_ClampRotations ();

	return *this;
}

CCamera & CCamera::TurnOnY (float amount)
{
	rotation.y() += amount;

	_WrapRotations ();

	return *this;
}

CCamera & CCamera::TurnByMouseDelta (int mouseX, int mouseY)
{
	TurnOnXByMouseDelta (mouseY);
	TurnOnYByMouseDelta (mouseX);
	return *this;
}

CCamera & CCamera::TurnOnXByMouseDelta (int mouseY)
{
	int mouseDifferenceY = mouseRelationalPoint.y() - mouseY;
	
	if (!useMouseSmoothing)
		rotation.x() += mouseDifferenceY*mouseDampening;
	else
		mouseVelocity.y() += mouseDifferenceY*mouseVelocityDampening;

	_ClampRotations ();

	if (autoUpdateMouseRelationalPoint)
		mouseRelationalPoint.y() = mouseY;

	return *this;
}

CCamera & CCamera::TurnOnYByMouseDelta (int mouseX)
{
	int mouseDifferenceX = mouseRelationalPoint.x() - mouseX;

	if (!useMouseSmoothing)
		rotation.y() += mouseDifferenceX*mouseDampening;
	else
		mouseVelocity.x() += mouseDifferenceX*mouseVelocityDampening;

	_WrapRotations ();

	if (autoUpdateMouseRelationalPoint)
		mouseRelationalPoint.x() = mouseX;

	return *this;
}

void CCamera::ApplyCameraTransformationsFromMouseVelocity ()
{
	//	If we're trying to update but we don't have smoothing enabled,
	//		simply clear out the current settings. This allows the user
	//		to enable/disable the use of smoothing without accidentally
	//		preserving the previous mouse velocity.
	if (!useMouseSmoothing)
	{
		mouseVelocity = vec3f::ZERO;
		return;
	}

	rotation.y() += mouseVelocity.x();
	rotation.x() += mouseVelocity.y();

	mouseVelocity.x() -= mouseVelocity.x() * mouseVelocityFriction;
	mouseVelocity.y() -= mouseVelocity.y() * mouseVelocityFriction;
}

#ifdef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN_SAFE
#else
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <gl\GL.h>

#ifdef WIN32_LEAN_AND_MEAN_SAFE
#undef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN_SAFE
#endif


void CCamera::ApplyTransformationsGL (int sign, int transformOrder)
{
	glPushMatrix ();
	switch (transformOrder)
	{
	case (CAMERA_ORDER_ROT_TRANS):
		glRotatef (rotation.x()*sign, 1.0f, 0.0f, 0.0f);
		glRotatef (rotation.y()*sign, 0.0f, 1.0f, 0.0f);
		glRotatef (rotation.z()*sign, 0.0f, 0.0f, 1.0f);
		glTranslatef (position.x()*sign, position.y()*sign, position.z()*sign);
		break;

	case (CAMERA_ORDER_TRANS_ROT):
		glTranslatef (position.x()*sign, position.y()*sign, position.z()*sign);
		glRotatef (rotation.x()*sign, 1.0f, 0.0f, 0.0f);
		glRotatef (rotation.y()*sign, 0.0f, 1.0f, 0.0f);
		glRotatef (rotation.z()*sign, 0.0f, 0.0f, 1.0f);
		break;
	}
}

void CCamera::StripTransformationsGL ()
{
	glPopMatrix ();
}



void CCamera::_WrapRotations ()
{
	if (rotation.y() < 0.0f)
		rotation.y() += 360.0f;
	if (rotation.y() > 360.0f)
		rotation.y() -= 360.0f;
}

void CCamera::_ClampRotations ()
{
	if (rotation.x() > 90)
		rotation.x() = 90;
	if (rotation.x() < -90)
		rotation.x() = -90;
}