
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

float randrange (float min, float max)
{
	return ((((float)rand()) / RAND_MAX) * (max - min)) + min;
}

int main ()
{
	SDL_Init (SDL_INIT_EVERYTHING);
	TTF_Init ();

	SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 32);
	SDL_Surface * screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL);

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

	glMatrixMode (GL_PROJECTION);
	gluPerspective (45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0, 1000000.0);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_TEXTURE_2D);

	NewtonianUniverse universe;

	srand (time (NULL));

	int bodyCount = 200;
	int maxSpawnRange = 1500;
	float minMass = 10, maxMass = 5000;
	int maxVel = 350;
	for (int i = 0; i < bodyCount; i++)
	{
		UniverseBody * body = new UniverseBody ();
		body->position = vec3f (randrange (-maxSpawnRange, maxSpawnRange), randrange (-maxSpawnRange, maxSpawnRange), 0.0f);
		body->mass = randrange (minMass, maxMass);
		float angle = randrange (0.0f, 6.282f);
		float vel = randrange (0.0f, maxVel);
		body->velocity.x () = cosf (angle) * vel;
		body->velocity.y () = sinf (angle) * vel;
		body->density = 6.0f;
		universe.AddBody (body);
	}

	/*
	float startDistance = 10.0f;

	for (int i = 0; i < bodyCount; i++)
	{
		UniverseBody * body = new UniverseBody ();
		float angle = randrange (0.0f, MATH_2_PI);
		vec3f normal;
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

	CCamera camera;
	camera.useMouseSmoothing = true;
	camera.autoUpdateMouseRelationalPoint = true;
	float scrollSpeed = 1.1f;

	camera.position.z () = -4000.0f;

	vec2f mouseClickCoords;
	bool isDragging = false;

	DWORD prevTime = timeGetTime ();

	glDepthFunc (GL_ALWAYS);

	gui::InitializeModules (gui::INIT_ALL);

	gui::ORGUI GUI;

	gui::g_FontModule->LoadFont ("arial.ttf", 20, "default");

	gui::Text * timeDeltaDisplay = new gui::Text ();
	timeDeltaDisplay->m_Font = "default";
	timeDeltaDisplay->m_Color = gui::COLOR_WHITE;
	GUI.AddChild (timeDeltaDisplay);

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
					camera.position.z () *= scrollSpeed;
				if (e.button.button == SDL_BUTTON_WHEELUP)
					camera.position.z () /= scrollSpeed;
			}

			if (e.type == SDL_MOUSEBUTTONUP)
				isDragging = false;
		}



		int state = SDL_GetMouseState (NULL, NULL);
		if (SDL_BUTTON (4) & state) // mousewheel up
			camera.position.z () /= scrollSpeed;
		if (SDL_BUTTON (5) & state) // mousewheel down
			camera.position.z () *= scrollSpeed;

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();

		camera.ApplyCameraTransformationsFromMouseVelocity ();
		if (isDragging)
		{
			int mx, my;
			SDL_GetMouseState (&mx, &my);
			camera.TurnByMouseDelta (mx, my);
		}

		camera.ApplyTransformationsGL (CAMERA_SIGN_POSITIVE);

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DWORD curTime = timeGetTime ();
		float dt = (curTime - prevTime) / 1000.0f;
		universe.Update (dt);
		prevTime = curTime;

		glDisable (GL_TEXTURE_2D);
		universe.Draw ();
		glEnable (GL_TEXTURE_2D);

		camera.StripTransformationsGL ();
		
		timeDeltaDisplay->m_Text = "Time Delta: " + ore::RealToString (dt);
		timeDeltaDisplay->m_Position.x = 5;
		timeDeltaDisplay->m_Position.y = 5;

		glLoadIdentity ();

		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();

		gluOrtho2D (0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		gui::Rect renderArea (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		GUI.Render (&renderArea);

		glPopMatrix ();

		SDL_GL_SwapBuffers ();
	}

	return 0;
}