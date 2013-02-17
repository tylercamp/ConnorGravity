
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
	vec3d position, rotation;
	//	Position to use as a reference when using TurnByMouseDelta functions.
	vec3d mouseRelationalPoint;
	//	MoveY is the only one disabled by default
	bool moveX, moveY, moveZ;

	//	Whether or not to update mouseRelationalPoint with the new
	//		position given by TurnByMouseDelta. Default false.
	bool autoUpdateMouseRelationalPoint;

	//	Multiplicative (direct relationship), can increase or decrease the speed of mouse
	//		movement when mouse smoothing is disabled. Default 0.1f.
	double mouseDampening;

	//	Optional, smooths out camera rotations based on mouse input (less jittery), Default false.
	bool useMouseSmoothing;
	vec3d mouseVelocity;

	//	Multiplicative (direct relationship), can increase or decrease the speed of mouse
	//		movement when mouse smoothing is enabled. Default 0.07f.
	double mouseVelocityDampening;

	double mouseVelocityFriction;

	CCamera & operator= (const CCamera & o);

				CCamera ();

	CCamera &	SetMoveX (bool yes);
	CCamera &	SetMoveY (bool yes);
	CCamera &	SetMoveZ (bool yes);

	CCamera &	Move (double distance, double relativeDirection);
	CCamera &	MoveForward (double distance);
	CCamera &	MoveBackward (double distance);
	CCamera &	MoveLeft (double distance);
	CCamera &	MoveRight (double distance);


	//	Degrees (Negative = Down, Positive = Up)
	CCamera &	TurnOnX (double amount);
	//	Degrees	(Negative = Right, Positive = Left)
	CCamera &	TurnOnY (double amount);

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