/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */


#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296
{
  /**  The is the constructor 
   * This is the documentation block for the constructor.
   */ 
  
  dominos_t::dominos_t()
  {
    //Ground
    /*! \var b1 
     * \brief pointer to the body ground 
     */ 
    b2Body* b1; 
    b2Body* ground1;
    b2Body* ground2;  
    {
      
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
      b2BodyDef bd; 
      b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(&shape, 0.0f);
    }
          
    //Top horizontal block
    {
      b2PolygonShape shape;
      shape.SetAsBox(12.5f, 3.5f);
	
      b2BodyDef bd;
      bd.position.Set(-8.0f, 25.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }
     //Vertical block
    {
      b2PolygonShape shape;
      shape.SetAsBox(3.5f, 12.5f);//, b2Vec2(-20.f,20.f), 0.0f);
	
      b2BodyDef bd;
      bd.position.Set(1.0f, 16.0f);
      ground1 = m_world->CreateBody(&bd);
      ground1->CreateFixture(&shape, 0.0f);
    }
    //Piston
    {
      b2PolygonShape shape1;
      shape1.SetAsBox(3.5f, 1.5f);
      
		b2FixtureDef fd;
      fd.shape = &shape1;
      fd.density = 20.0f;
      fd.friction = 0.1f;
      b2BodyDef bd1;
      bd1.type=b2_dynamicBody;
      bd1.position.Set(1.0f, 14.75f);
      ground2 = m_world->CreateBody(&bd1);
      ground2->CreateFixture(&fd);
     }
     //The bottom spring
     {
      b2DistanceJointDef distance_joint;

	  distance_joint.bodyA = ground1;

		distance_joint.bodyB = ground2;

		distance_joint.localAnchorA.Set(0.0f,-12.5f);

		distance_joint.localAnchorB.Set(0.0f,-1.5f);

		distance_joint.length = 9.75f;

		distance_joint.collideConnected = true;
		distance_joint.frequencyHz=4.0f;
		distance_joint.dampingRatio=0.5f;
		m_world->CreateJoint(&distance_joint); 
    }
    {

    //Bullets
    {
      b2PolygonShape shape;
      shape.SetAsBox(1.25f, 0.6f);
	
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
      fd.friction = 0.1f;
		
      for (int i = 0; i < 1; ++i)
	{
	  b2BodyDef bd;
	  bd.type = b2_dynamicBody;
	  bd.position.Set(1.0f, 16.85f + 0.6f * i);
	  b2Body* body = m_world->CreateBody(&bd);
	  body->CreateFixture(&fd);
	}
    }

	
   

    


    }


  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
