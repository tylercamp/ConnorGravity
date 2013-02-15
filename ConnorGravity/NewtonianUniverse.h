
#ifndef _NEWTONIAN_UNIVERSE_H_
#define _NEWTONIAN_UNIVERSE_H_

#include <vmmlib\vmmlib.hpp>
#include <list>

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include "Settings.h"

using namespace vmml;

class NewtonianUniverse;

class UniverseBody
{
public:
	float radius;
	float density;
	vec3f velocity;
	vec3f position;

	float volume ()
	{
		return 4.1888f * (radius * radius * radius);
	}

	float mass ()
	{
		return density * volume ();
	}

	bool CollidesWith (UniverseBody * other) const
	{
		return (position - other->position).length () <= radius + other->radius;
	}

	virtual void Draw ()
	{
		glPushMatrix ();
		glTranslatef (position.x(), position.y(), position.z());
		GLUquadricObj * quadric = gluNewQuadric ();
		glColor3f (1.0f, 1.0f, 1.0f);
		gluSphere (quadric, radius, 36, 36);
		gluDeleteQuadric (quadric);
		glPopMatrix ();
	}

	UniverseBody ()
	{
		radius = 0.0f;
		density = 0.0f;
		velocity = vec3f::ZERO;
		position = vec3f::ZERO;
	}

	virtual ~UniverseBody ()
	{
	}

protected:
	std::list <UniverseBody *>::iterator mm_BodyPosition;
	NewtonianUniverse * mm_OwnerUniverse;

	friend class NewtonianUniverse;
};



class NewtonianUniverse
{
public:
	void Update (float dt)
	{
		std::vector <UniverseBody *> bodyDestroyQueue;

		//	Collision checks first
		//	TODO: This collision detection method is VERY prone to undefined behavior (if a body intersects more than one thing
		//		at a time). Will need to revise.
		for (auto iter = m_Bodies.begin (); iter != m_Bodies.end (); iter++)
		{
			//	Check against all other bodies
			for (auto col = m_Bodies.begin (); col != m_Bodies.end (); col++)
			{
				if (col == iter)
					continue;

				if ((*iter)->CollidesWith (*col))
				{
					if ((*iter)->radius > (*col)->radius)
					{
						(*iter)->radius += (*col)->radius;
						bodyDestroyQueue.push_back (*col);
					}
					else
					{
						(*col)->radius += (*iter)->radius;
						bodyDestroyQueue.push_back (*iter);
					}
				}
			}
		}

		//	Prune collided bodies
		for (int i = 0; i < bodyDestroyQueue.size (); i++)
		{
			RemoveBody (bodyDestroyQueue [i]);
		}
		
		//	Apply general physics updates on bodies
		for (auto iter = m_Bodies.begin (); iter != m_Bodies.end (); iter++)
		{
			(*iter)->position += (*iter)->velocity * dt;

			vec3f gravForce = vec3f::ZERO;
			//	Get the sum gravitational force
			for (auto bodies = m_Bodies.begin (); bodies != m_Bodies.end (); bodies++)
			{
				if (bodies == iter)
					continue;

				vec3f forceVec = (*bodies)->position - (*iter)->position;
				float dist = forceVec.length();
				forceVec.normalize ();
				forceVec *= (((*bodies)->mass () * (*iter)->mass ()) / (dist * dist)) * GRAVITATIONAL_CONSTANT;
				gravForce += forceVec;
			}

			(*iter)->velocity += (gravForce * dt) / (*iter)->mass ();
		}
	}

	void Draw ()
	{
		for (auto iter = m_Bodies.begin (); iter != m_Bodies.end (); iter++)
		{
			(*iter)->Draw ();
		}
	}

	void AddBody (UniverseBody * body)
	{
		m_Bodies.push_back (body);
		body->mm_BodyPosition = --m_Bodies.end ();
	}

	void RemoveBody (UniverseBody * body)
	{
		if (body->mm_BodyPosition == m_Bodies.end ())
			return;

		m_Bodies.erase (body->mm_BodyPosition);
		body->mm_BodyPosition = m_Bodies.end ();
	}

private:
	std::list <UniverseBody *> m_Bodies;
};

#endif