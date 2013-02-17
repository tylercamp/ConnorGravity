
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmsystem.h>

#undef min
#undef max

#include <SDL\SDL.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <ctime>

#include "NewtonianUniverse.h"
#include "Settings.h"

#include "Camera.h"

#include "ORGUI\ORGUI.h"
#include "ORGUI\math.h"

#ifdef main
# undef main
#endif

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "sdl.lib")
#pragma comment (lib, "sdl_ttf.lib")
#pragma comment (lib, "winmm.lib")

#ifdef ORE_DEBUG
#pragma comment (lib, "libfio-d.lib")
#else
#pragma comment (lib, "libfio.lib")
#endif

double randrange (double min, double max)
{
	return ((((double)rand()) / RAND_MAX) * (max - min)) + min;
}

double randrangebinomial (double min, double max)
{
	return pow (randrange (0.0, 1.0), 20.0) * (max - min) + min;
}

void GenerateRandomBodies (NewtonianUniverse * universe)
{
	int bodyCount = 100;
	int maxSpawnRange = 1500;
	double minMass = 2.0f, maxMass = 1000000.0f;
	int maxVel = 350;
	for (int i = 0; i < bodyCount; i++)
	{
		UniverseBody * body = new UniverseBody ();
		body->position = vec3d (randrange (-maxSpawnRange, maxSpawnRange), randrange (-maxSpawnRange, maxSpawnRange), 0.0f);
		body->mass = randrangebinomial (minMass, maxMass);
		double angle = randrange (0.0f, 6.282f);
		double vel = randrange (0.0f, maxVel);
		body->velocity.x () = cosf (angle) * vel;
		body->velocity.y () = sinf (angle) * vel;
		body->density = 1.2f;
		universe->AddBody (body);
	}

	/*
	double startDistance = 10.0f;

	for (int i = 0; i < bodyCount; i++)
	{
		UniverseBody * body = new UniverseBody ();
		double angle = randrange (0.0f, MATH_2_PI);
		vec3d normal;
		normal.x () = cosf (angle);
		normal.y () = sinf (angle);
		normal.z () = 0.0f;

		body->position = normal * startDistance;
		body->velocity = normal * randrange (maxVel / 2.0f, maxVel);
		body->density = 6.0f;
		body->radius = 2;
		universe.AddBody (body);
	}
	*/
}

int main ()
{
	srand (time (NULL));

	SDL_Init (SDL_INIT_EVERYTHING);
	TTF_Init ();

	SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 32);
	SDL_Surface * screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL | SDL_RESIZABLE);

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	glMatrixMode (GL_PROJECTION);
	gluPerspective (45.0f, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 1.0, 1000000.0);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_TEXTURE_2D);

	NewtonianUniverse universe;
	GenerateRandomBodies (&universe);

	CCamera camera;
	camera.useMouseSmoothing = true;
	camera.autoUpdateMouseRelationalPoint = true;
	double scrollSpeed = 10.0f;

	camera.SetMoveX (true);
	camera.SetMoveY (true);
	camera.SetMoveZ (true);

	camera.position.z () = -4000.0f;

	vec2f mouseClickCoords;
	bool isDragging = false;

	DWORD prevTime = timeGetTime ();

	glDepthFunc (GL_ALWAYS);

	gui::InitializeModules (gui::INIT_ALL);

	gui::ORGUI GUI;

	gui::g_FontModule->LoadFont ("arial.ttf", 13, "default");

	gui::Text * timeDeltaDisplay = new gui::Text ();
	timeDeltaDisplay->m_Font = "default";
	timeDeltaDisplay->m_Color = gui::COLOR_WHITE;
	GUI.AddChild (timeDeltaDisplay);

	double speed = 0.0f;

	bool run = true;
	while (run)
	{
		SDL_Event e;
		SDL_PumpEvents ();
		while (SDL_PollEvent (&e))
		{
			if (e.type == SDL_QUIT)
				run = false;

			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
				run = false;

			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
					speed = 0.0f;
				if (e.key.keysym.sym == SDLK_TAB)
				{
					if (e.key.keysym.mod & KMOD_CTRL)
						universe.ReleaseAllBodies ();
					GenerateRandomBodies (&universe);
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					isDragging = true;
					int mx, my;
					SDL_GetMouseState (&mx, &my);
					camera.mouseRelationalPoint.x () = mx;
					camera.mouseRelationalPoint.y () = my;
				}

				if (e.button.button == SDL_BUTTON_WHEELDOWN)
					speed += scrollSpeed;
				if (e.button.button == SDL_BUTTON_WHEELUP)
					speed -= scrollSpeed;
			}

			if (e.type == SDL_MOUSEBUTTONUP)
				isDragging = false;

			if (e.type == SDL_VIDEORESIZE)
			{
				SCREEN_WIDTH = e.resize.w;
				SCREEN_HEIGHT = e.resize.h;

				glMatrixMode (GL_PROJECTION);
				glLoadIdentity ();
				gluPerspective (45.0f, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 1.0, 1000000.0);
			}
		}


		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();

		camera.ApplyCameraTransformationsFromMouseVelocity ();
		if (isDragging)
		{
			int mx, my;
			SDL_GetMouseState (&mx, &my);
			camera.TurnByMouseDelta (mx, my);
		}

		camera.MoveForward (speed);

		camera.ApplyTransformationsGL (CAMERA_SIGN_POSITIVE);

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DWORD curTime = timeGetTime ();
		double dt = (curTime - prevTime) / 1000.0f;
		universe.Update (dt);
		prevTime = curTime;

		glDisable (GL_TEXTURE_2D);
		universe.Draw ();
		glEnable (GL_TEXTURE_2D);

		camera.StripTransformationsGL ();
		
		timeDeltaDisplay->m_Text = "Time Delta: " + ore::RealToString (dt);
		timeDeltaDisplay->m_Text += "\nSpeed: " + ore::RealToString (speed);
		timeDeltaDisplay->m_Text += "\nPosition: (" + ore::RealToString (camera.position.x()) + "," + ore::RealToString (camera.position.y()) + "," + ore::RealToString (camera.position.z()) + ")";
		timeDeltaDisplay->m_Position.x = 5;
		timeDeltaDisplay->m_Position.y = 5;

		glLoadIdentity ();

		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();

		gluOrtho2D (0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		glBlendFunc (GL_ONE, GL_ONE);
		gui::Rect renderArea (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		GUI.Render (&renderArea);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPopMatrix ();

		SDL_GL_SwapBuffers ();
	}

	return 0;
}