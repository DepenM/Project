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
    /** \par Ground */
    
	//! b1 is a pointer to an object of the type b2Body
    b2Body* b1;   
    {
      //! shape is of the type b2EdgeShape. It specifies the shape of the ground. Shape starts at (-90.0f, 0.0f) and ends at (90.0f, 0.0f).
      b2EdgeShape shape; 
      
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
      //! bd is of the type b2BodyDef. bd is used to define b1 upon which ground will be fixed
      b2BodyDef bd; 
      b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(&shape, 0.0f);
    }
          
    /** \par Top horizontal shelf */
    {
	//! shape is of the type b2PolygonShape. It specifies the shape of the top horizontal shelf. Its dimensions are (6.0f, 0.25f). 
      b2PolygonShape shape;
      shape.SetAsBox(6.0f, 0.25f);
	 //! bd is of the type b2BodyDef. bd is used to define b1 upon which ground will be fixed
      b2BodyDef bd;
      bd.position.Set(-31.0f, 30.0f);
      //! b1 is a pointer to an object of the type b2Body
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }

    /** \par Dominos */
    {
	//! shape is of the type b2PolygonShape. It specifies the shape of the Dominos. Their dimensions are (0.1f, 1.0f). 
      b2PolygonShape shape;
      shape.SetAsBox(0.1f, 1.0f);
	//! fd is of the type b2FixtureDef. It specifies the physical properties of the Dominos. Their density is 20.0f and friction is 0.1f.
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
      fd.friction = 0.1f;
		
      for (int i = 0; i < 10; ++i)
	{
		//! bd is of the type b2BodyDef. bd is used to define body upon which a domino will be fixed.
	  b2BodyDef bd;
	  bd.type = b2_dynamicBody;
	  bd.position.Set(-35.5f + 1.0f * i, 31.25f);
	  //! body is a pointer to an object of the type b2Body
	  b2Body* body = m_world->CreateBody(&bd);
	  body->CreateFixture(&fd);
	}
    }
      
    /** \par Another horizontal shelf */
    {
		//! shape is of the type b2PolygonShape. It specifies the shape of the horizontal shelf upon which balls are kept. Its dimensions are (7.0f, 0.25f, b2Vec2(-20.f,20.f), 0.0f).
      b2PolygonShape shape;
      shape.SetAsBox(7.0f, 0.25f, b2Vec2(-20.f,20.f), 0.0f);
	//! bd is of the type b2BodyDef. bd is used to define ground upon which shelf will be fixed
      b2BodyDef bd;
      bd.position.Set(1.0f, 6.0f);
      //! ground is a pointer to an object of the type b2Body
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }


    /** \par The pendulum that knocks the dominos off */
    {
		//! b2 is a pointer to an object of the type b2Body
      b2Body* b2;
      {
		  //! shape is of the type b2PolygonShape. It specifies the shape of the static box with which pendulum thread is attached. Their dimensions are (0.25f, 1.5f).
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 1.5f);
	  //! bd is of the type b2BodyDef. bd is used to define b2 upon which static box will be fixed
	b2BodyDef bd;
	bd.position.Set(-36.5f, 28.0f);
	b2 = m_world->CreateBody(&bd);
	b2->CreateFixture(&shape, 10.0f);
      }
	
	//! b4 is a pointer to an object of the type b2Body
      b2Body* b4;
      {
		  //! shape is of the type b2PolygonShape. It specifies the shape of the moving box with which pendulum thread is attached. Their dimensions are (0.25f, 0.25f).
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 0.25f);
	  //! bd is of the type b2BodyDef. bd is used to define b4 upon which moving box will be fixed
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(-40.0f, 33.0f);
	b4 = m_world->CreateBody(&bd);
	b4->CreateFixture(&shape, 2.0f);
      }
	//! jd is of type b2RevoluteJointDef. It joins b2 and b4 at (-37.0f, 40.0f).
      b2RevoluteJointDef jd;
      b2Vec2 anchor;
      anchor.Set(-37.0f, 40.0f);
      jd.Initialize(b2, b4, anchor);
      m_world->CreateJoint(&jd);
    }
      
    /** \par The train of small spheres */
    {
		//! spherebody is a pointer to an object of the type b2Body
      b2Body* spherebody;
	//! circle is of the type b2CircleShape. It specifies the shape of the small spheres. Their radius is 0.5.
      b2CircleShape circle;
      circle.m_radius = 0.5;
	
	//! ballfd is of the type b2FixtureDef. It specifies the physical properties of the small spheres. Their density is 1.0f, friction is 0.0f and restitution is 0.0f.
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 1.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
	
      for (int i = 0; i < 10; ++i)
	{
		//! ballbd is of the type b2BodyDef. ballbd is used to define spherebody upon which small spheres will be fixed.
	  b2BodyDef ballbd;
	  ballbd.type = b2_dynamicBody;
	  ballbd.position.Set(-22.2f + i*1.0, 26.6f);
	  spherebody = m_world->CreateBody(&ballbd);
	  spherebody->CreateFixture(&ballfd);
	}
    }

    /** \par The pulley system */
    {
		//! bd is a pointer to an object of the type b2BodyDef. bd is used to define box1 upon which container will be placed.
      b2BodyDef *bd = new b2BodyDef;
      bd->type = b2_dynamicBody;
      bd->position.Set(-10,15);
      bd->fixedRotation = true;
      
      /** \par The open box */
      //! fd1 is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the bottom surface of box1. Their density is 10.0f and friction is 0.5f.
      b2FixtureDef *fd1 = new b2FixtureDef;
      fd1->density = 10.0;
      fd1->friction = 0.5;
      fd1->restitution = 0.f;
      fd1->shape = new b2PolygonShape;
       //! bs1 is of the type b2PolygonShape. It specifies the shape of the bottom surface of Box1. Their dimensions are (2,0.2, b2Vec2(0.f,-1.9f), 0).
      b2PolygonShape bs1;
      bs1.SetAsBox(2,0.2, b2Vec2(0.f,-1.9f), 0);
      fd1->shape = &bs1;
      //! fd2 is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the right surface of box1. Their density is 10.0f and friction is 0.5f.
      b2FixtureDef *fd2 = new b2FixtureDef;
      fd2->density = 10.0;
      fd2->friction = 0.5;
      fd2->restitution = 0.f;
      fd2->shape = new b2PolygonShape;
      //! bs2 is of the type b2PolygonShape. It specifies the shape of the right surface of Box1. Their dimensions are (0.2,2, b2Vec2(2.0f,0.f), 0).
      b2PolygonShape bs2;
      bs2.SetAsBox(0.2,2, b2Vec2(2.0f,0.f), 0);
      fd2->shape = &bs2;
      //! fd3 is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the left surface of box1. Their density is 10.0f and friction is 0.5f.
      b2FixtureDef *fd3 = new b2FixtureDef;
      fd3->density = 10.0;
      fd3->friction = 0.5;
      fd3->restitution = 0.f;
      fd3->shape = new b2PolygonShape;
      //! bs3 is of the type b2PolygonShape. It specifies the shape of the left surface of Box1. Their dimensions are (0.2,2, b2Vec2(-2.0f,0.f), 0).
      b2PolygonShape bs3;
      bs3.SetAsBox(0.2,2, b2Vec2(-2.0f,0.f), 0);
      fd3->shape = &bs3;
       
       //! box1 is a pointer to an object of the type b2Body
      b2Body* box1 = m_world->CreateBody(bd);
      box1->CreateFixture(fd1);
      box1->CreateFixture(fd2);
      box1->CreateFixture(fd3);

      /** \par The bar */
      bd->position.Set(10,15);	
      fd1->density = 34.0;	  
      //! box2 is a pointer to an object of the type b2Body
      b2Body* box2 = m_world->CreateBody(bd);
      box2->CreateFixture(fd1);

      /** \par  The pulley joint */
      //! myjoint is a pointer to an object of the type b2PulleyJointDef.It connects centers of box1 and box2 through (-10, 20) and (10, 20).
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();
      b2Vec2 worldAnchorOnBody1(-10, 15); // Anchor point on body 1 in world axis
      b2Vec2 worldAnchorOnBody2(10, 15); // Anchor point on body 2 in world axis
      b2Vec2 worldAnchorGround1(-10, 20); // Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(10, 20); // Anchor point for ground 2 in world axis
      float32 ratio = 1.0f; // Define ratio
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);
    }

    /** \par The revolving horizontal platform */
    {
		//! shape is of the type b2PolygonShape. It specifies the shape of the revolving horizontal platform. Its dimensions are (2.2f, 0.2f).
      b2PolygonShape shape;
      shape.SetAsBox(2.2f, 0.2f);
	
	//! bd is of the type b2BodyDef. bd is used to define b1 upon which the revolving horizontal platform is placed.
      b2BodyDef bd;
      bd.position.Set(14.0f, 14.0f);
      bd.type = b2_dynamicBody;
      //! b1 is a pointer to an object of the type b2Body
      b2Body* body = m_world->CreateBody(&bd);
      //! fd is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the horizontal platform. Its density is 1.f.
      b2FixtureDef *fd = new b2FixtureDef;
      fd->density = 1.f;
      fd->shape = new b2PolygonShape;
      fd->shape = &shape;
      body->CreateFixture(fd);
		
		//! shape2 is of the type b2PolygonShape. Shape2 is not displayed on the interface. Its dimensions are (0.2f, 2.0f).
      b2PolygonShape shape2;
      shape2.SetAsBox(0.2f, 2.0f);
      //! bd2 is of the type b2BodyDef. bd2 is used to define body2.
      b2BodyDef bd2;
      bd2.position.Set(14.0f, 16.0f);
      //! body2 is a pointer to an object of the type b2Body
      b2Body* body2 = m_world->CreateBody(&bd2);
		//! jointDef is of type b2RevoluteJointDef. It joins body and body2.
      b2RevoluteJointDef jointDef;
      jointDef.bodyA = body;
      jointDef.bodyB = body2;
      jointDef.localAnchorA.Set(0,0);
      jointDef.localAnchorB.Set(0,0);
      jointDef.collideConnected = false;
      m_world->CreateJoint(&jointDef);
    }

    /** \par The heavy sphere on the platform */
    {
		//! sbody is a pointer to an object of the type b2Body
      b2Body* sbody;
      //! circle is of the type b2CircleShape. It specifies the shape of the heavy sphere. Its radius is 1.0.
      b2CircleShape circle;
      circle.m_radius = 1.0;
	//! ballfd is of the type b2FixtureDef. It specifies the physical properties of the heavy sphere. Its density is 50.0f.
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 50.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
      //! ballbd is of the type b2BodyDef. bd is used to define sbody upon which heavy sphere will be placed.
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(14.0f, 18.0f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }


    /** \par The see-saw system at the bottom */
    {
      /** \par The triangle wedge */
      //! sbody is a pointer to an object of the type b2Body
      b2Body* sbody;
      //! poly is of the type b2PolygonShape. It specifies the shape of the the triangle wedge. Its coordinates are (-1,0),(1,0) and (0,1.5).
      b2PolygonShape poly;
      b2Vec2 vertices[3];
      vertices[0].Set(-1,0);
      vertices[1].Set(1,0);
      vertices[2].Set(0,1.5);
      poly.Set(vertices, 3);
      //! wedgefd is of the type b2FixtureDef. It specifies the physical properties of the wedge. Its density is 10.0f.
      b2FixtureDef wedgefd;
      wedgefd.shape = &poly;
      wedgefd.density = 10.0f;
      wedgefd.friction = 0.0f;
      wedgefd.restitution = 0.0f;
      //! wedgebd is of the type b2BodyDef. wedgebd is used to define sbody upon which wedge will be fixed
      b2BodyDef wedgebd;
      wedgebd.position.Set(30.0f, 0.0f);
      sbody = m_world->CreateBody(&wedgebd);
      sbody->CreateFixture(&wedgefd);

      /** \par The plank on top of the wedge */
      //! shape is of the type b2PolygonShape. It specifies the shape of the plank on top of wedge. Its dimensions are (15.0f, 0.2f).
      b2PolygonShape shape;
      shape.SetAsBox(15.0f, 0.2f);
      //! bd2 is of the type b2BodyDef. bd2 is used to define body upon which plank will be placed
      b2BodyDef bd2;
      bd2.position.Set(30.0f, 1.5f);
      bd2.type = b2_dynamicBody;
      //! body is a pointer to an object of the type b2Body
      b2Body* body = m_world->CreateBody(&bd2);
      //! fd2 is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the plank. Its density is 1.f.
      b2FixtureDef *fd2 = new b2FixtureDef;
      fd2->density = 1.f;
      fd2->shape = new b2PolygonShape;
      fd2->shape = &shape;
      body->CreateFixture(fd2);
     //! jd is of type b2RevoluteJointDef. It joins sbody and bbody at (30.0f, 1.5f).
      b2RevoluteJointDef jd;
      b2Vec2 anchor;
      anchor.Set(30.0f, 1.5f);
      jd.Initialize(sbody, body, anchor);
      m_world->CreateJoint(&jd);

      /** \par The light box on the right side of the see-saw */
      //! shape2 is of the type b2PolygonShape. It specifies the shape of the light box on right of see-saw. Its dimensions are (2.0f, 2.0f).
      b2PolygonShape shape2;
      shape2.SetAsBox(2.0f, 2.0f);
      //! bd3 is of the type b2BodyDef. bd3 is used to define body3 upon which light box will be placed.
      b2BodyDef bd3;
      bd3.position.Set(40.0f, 2.0f);
      bd3.type = b2_dynamicBody;
      //! body3 is a pointer to an object of the type b2Body
      b2Body* body3 = m_world->CreateBody(&bd3);
      //! fd3 is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the light box. Its density is 0.01f.
      b2FixtureDef *fd3 = new b2FixtureDef;
      fd3->density = 0.01f;
      fd3->shape = new b2PolygonShape;
      fd3->shape = &shape2;
      body3->CreateFixture(fd3);
    }
    
    /** \par The vertical wall */
   {
	  //! a1 is of the type b2PolygonShape. It specifies the shape of the vertical wall. Its dimensions are (0.0f, 10.0f,b2Vec2(-30.0f,10.0f),0.0f). 
	  b2PolygonShape a1;
      a1.SetAsBox(0.0f, 10.0f,b2Vec2(-30.0f,10.0f),0.0f);
      //! fd is of the type b2FixtureDef. It specifies the physical properties of the vertical wall. Its density is 10.0f, friction is 0.1f and restitution is 0.6f.
      b2FixtureDef fd;
      fd.shape = &a1;
      fd.density = 10.0f;
      fd.friction = 0.1f;
      fd.restitution = 0.6f;
      b1->CreateFixture(&fd);
	}
	
	/** \par The new revolving horizontal platform */
    {
		//! shape is of the type b2PolygonShape. It specifies the shape of the revolving horizontal platform. Its dimensions are (2.2f, 0.2f).
      b2PolygonShape shape;
      shape.SetAsBox(2.2f, 0.2f);
	
	//! bd is of the type b2BodyDef. bd is used to define b1 upon which the revolving horizontal platform is placed.
      b2BodyDef bd;
      bd.position.Set(17.0f, 11.0f);
      bd.type = b2_dynamicBody;
      //! b1 is a pointer to an object of the type b2Body
      b2Body* body = m_world->CreateBody(&bd);
      //! fd is a pointer to an object of the type b2FixtureDef. It specifies the physical properties of the horizontal platform. Its density is 1.f.
      b2FixtureDef *fd = new b2FixtureDef;
      fd->density = 1.f;
      fd->shape = new b2PolygonShape;
      fd->shape = &shape;
      body->CreateFixture(fd);
		
		//! shape2 is of the type b2PolygonShape. Shape2 is not displayed on the interface. Its dimensions are (0.2f, 2.0f).
      b2PolygonShape shape2;
      shape2.SetAsBox(0.2f, 2.0f);
      //! bd2 is of the type b2BodyDef. bd2 is used to define body2.
      b2BodyDef bd2;
      bd2.position.Set(17.0f, 13.0f);
      //! body2 is a pointer to an object of the type b2Body
      b2Body* body2 = m_world->CreateBody(&bd2);
		//! jointDef is of type b2RevoluteJointDef. It joins body and body2.
      b2RevoluteJointDef jointDef;
      jointDef.bodyA = body;
      jointDef.bodyB = body2;
      jointDef.localAnchorA.Set(0,0);
      jointDef.localAnchorB.Set(0,0);
      jointDef.collideConnected = false;
      m_world->CreateJoint(&jointDef);
    }
    
    /** \par The new heavy sphere on the new platform */
    {
		//! sbody is a pointer to an object of the type b2Body
      b2Body* sbody;
      //! circle is of the type b2CircleShape. It specifies the shape of the heavy sphere. Its radius is 1.0.
      b2CircleShape circle;
      circle.m_radius = 1.0;
	//! ballfd is of the type b2FixtureDef. It specifies the physical properties of the heavy sphere. Its density is 10.0f.
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 10.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.5f;
      //! ballbd is of the type b2BodyDef. bd is used to define sbody upon which heavy sphere will be placed.
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(17.0f, 15.0f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }
  }

  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
