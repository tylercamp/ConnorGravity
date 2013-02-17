
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <vmmlib\vmmlib.hpp>

using namespace vmml;


#define CAMERA_SIGN_POSITIVE 1
#define CAMERA_SIGN_NEGATIVE -1
#define CAMERA_SIGN_WORLD_SPACE CAMERA_SIGN_NEGATIVE

#define CAMERA_ORDER_ROT_TRANS 2
#define CAMERA_ORDER_TRANS_ROT 3

class CCamera
{
public:
	vec3f position, rotation;
	//	Position to use as a reference when using TurnByMouseDelta functions.
	vec3f mouseRelationalPoint;
	//	MoveY is the only one disabled by default
	bool moveX, moveY, moveZ;

	//	Whether or not to update mouseRelationalPoint with the new
	//		position given by TurnByMouseDelta. Default false.
	bool autoUpdateMouseRelationalPoint;

	//	Multiplicative (direct relationship), can increase or decrease the speed of mouse
	//		movement when mouse smoothing is disabled. Default 0.1f.
	float mouseDampening;

	//	Optional, smooths out camera rotations based on mouse input (less jittery), Default false.
	bool useMouseSmoothing;
	vec3f mouseVelocity;

	//	Multiplicative (direct relationship), can increase or decrease the speed of mouse
	//		movement when mouse smoothing is enabled. Default 0.07f.
	float mouseVelocityDampening;

	float mouseVelocityFriction;

	CCamera & operator= (const CCamera & o);

				CCamera ();

	CCamera &	SetMoveX (bool yes);
	CCamera &	SetMoveY (bool yes);
	CCamera &	SetMoveZ (bool yes);

	CCamera &	Move (float distance, float relativeDirection);
	CCamera &	MoveForward (float distance);
	CCamera &	MoveBackward (float distance);
	CCamera &	MoveLeft (float distance);
	CCamera &	MoveRight (float distance);


	//	Degrees (Negative = Down, Positive = Up)
	CCamera &	TurnOnX (float amount);
	//	Degrees	(Negative = Right, Positive = Left)
	CCamera &	TurnOnY (float amount);

	CCamera &	TurnOnXByMouseDelta (int mouseY);
	CCamera &	TurnOnYByMouseDelta (int mouseX);
	CCamera &	TurnByMouseDelta (int mouseX, int mouseY);

	void		ApplyCameraTransformationsFromMouseVelocity ();

	void		ApplyTransformationsGL (int sign, int transformOrder = CAMERA_ORDER_ROT_TRANS);
	void		StripTransformationsGL ();

private:
	void		_ClampRotations ();
	void		_WrapRotations ();
};

#endif