
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mmsystem.h>

#include "oregGeneric.h"

#include "OpenGL.h"

#include "BloomShader.h"
#include "RenderTarget.h"

#undef min
#undef max

#include <SDL\SDL.h>
#include <ctime>

#include "NewtonianUniverse.h"
#include "Settings.h"

#include "Camera.h"

#include "oregFont.h"

#include "math.h"

#ifdef main
# undef main
#endif

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "sdl.lib")
#pragma comment (lib, "sdl_ttf.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "glew32.lib")

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
	double minMass = 2.0, maxMass = 1000000.0;
	int maxVel = 150;
	for (int i = 0; i < bodyCount; i++)
	{
		UniverseBody * body = new UniverseBody ();
		body->position = vec3d (randrange (-maxSpawnRange, maxSpawnRange), randrange (-maxSpawnRange, maxSpawnRange), 0.0);
		body->mass = randrangebinomial (minMass, maxMass);
		double angle = randrange (0.0, MATH_2_PI);
		double vel = randrange (0.0, maxVel);
		body->velocity.x () = cos (angle) * vel;
		body->velocity.y () = sin (angle) * vel;
		body->density = 1.2;
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

	//SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 32);
	SDL_Surface * screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL | SDL_RESIZABLE);

	glewInit ();

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

	ore::Font debugFont ("arial.ttf", 16);
	ore::t_String debugText;

	double speed = 0.0f;

	BloomShader bloom;
	bloom.Bind ();
	glActiveTexture (GL_TEXTURE0);
	RenderTarget target (SCREEN_WIDTH, SCREEN_HEIGHT);
	bloom.SetSourceTexture (0);
	glUseProgram (0);

	std::cout << "Post-setup getError: " << glGetError () << std::endl;

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

				glViewport (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

				glMatrixMode (GL_PROJECTION);
				glLoadIdentity ();
				gluPerspective (45.0f, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 1.0, 1000000.0);

				target.SetSize (SCREEN_WIDTH, SCREEN_HEIGHT);
				bloom.Bind ();
				bloom.SetSourceTexture (0);
				glUseProgram (0);
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

		target.Bind ();

		glClear (GL_COLOR_BUFFER_BIT);
		universe.Draw ();

		target.ResetRenderTarget ();
		
		glEnable (GL_TEXTURE_2D);

		camera.StripTransformationsGL ();

		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();
		gluOrtho2D (0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		ore::Generic::BindTexture (target.GetTexture ());
		bloom.Bind ();
		glBegin (GL_QUADS);
			glTexCoord2f (0.0f, 1.0f);
			glVertex2f (0.0f, 0.0f);

			glTexCoord2f (1.0f, 1.0f);
			glVertex2f (SCREEN_WIDTH, 0.0f);

			glTexCoord2f (1.0f, 0.0f);
			glVertex2f (SCREEN_WIDTH, SCREEN_HEIGHT);

			glTexCoord2f (0.0f, 0.0f);
			glVertex2f (0.0f, SCREEN_HEIGHT);
		glEnd ();

		glUseProgram (0);

		debugFont.RenderString (5.0f, 5.0f, "glGetError: " + ore::RealToString (glGetError ()));

		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();

		SDL_GL_SwapBuffers ();
	}

	return 0;
}