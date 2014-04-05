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
    b2Body* ground3;
    b2Body* ground4;
    b2Body* ground5;
    b2Body* ground6;
    m_world->SetGravity( b2Vec2(0,0) );  
    {
      
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-2.5f, 3.5f), b2Vec2(-2.5f, 21.5f));
      b2BodyDef bd; 
      b1 = m_world->CreateBody(&bd);
      
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0003;
       
      b1->CreateFixture(&fd);      
      shape.Set(b2Vec2(4.5f, 3.5f), b2Vec2(4.5f, 28.5f));
      b1->CreateFixture(&fd);
    }
          
    //Top horizontal block
    {
      b2PolygonShape shape;
      shape.SetAsBox(12.5f, 3.5f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0003;
//      fd.filter.maskBits = 0xFFFF & ~0x0002;
	
      b2BodyDef bd;
      bd.position.Set(-8.0f, 25.0f);
      ground3 = m_world->CreateBody(&bd);
      ground3->CreateFixture(&fd);
      
      
    }


    //Bullets
    {
      b2PolygonShape shape;
      shape.SetAsBox(2.5f, 1.2f);
	
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0002;
      fd.filter.maskBits = 0xFFFF & ~ 0x0003;
      fd.filter.groupIndex = 1;
		
      for (int i = 0; i < 4; ++i)
	{
	  b2BodyDef bd;
	  bd.type = b2_dynamicBody;
	  bd.position.Set(1.0f, 16.4f + 2.4f * i);
	  b2Body* body = m_world->CreateBody(&bd);
	  body->CreateFixture(&fd);
	}
    }
      
    //Vertical block
    {
      b2PolygonShape shape;
      shape.SetAsBox(3.5f, 12.5f);//, b2Vec2(-20.f,20.f), 0.0f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
      fd.filter.categoryBits = 0x0001;
//      fd.filter.maskBits = ~ 0xFFFF;
	
      b2BodyDef bd;
      bd.position.Set(1.0f, 16.0f);
      ground1 = m_world->CreateBody(&bd);
      ground1->CreateFixture(&fd);
    }
    
    
     //Piston
    {
      b2PolygonShape shape;
      shape.SetAsBox(3.5f, 1.0f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0004;
      fd.filter.maskBits = 0x0002;
	
      b2BodyDef bd;
      bd.type=b2_dynamicBody;
      bd.position.Set(1.0f, 14.2f);
      ground2 = m_world->CreateBody(&bd);
      ground2->CreateFixture(&fd);
    }
    

    
    
    //Vertical Spring
    {
     b2DistanceJointDef distance_joint;

	 distance_joint.bodyA = ground1;

	distance_joint.bodyB = ground2;

	distance_joint.localAnchorA.Set(0.0f,-12.5f);

	distance_joint.localAnchorB.Set(0.0f,-1.0f);

	distance_joint.length = 12.75f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=0.4f;
	distance_joint.dampingRatio=0.5f;
	m_world->CreateJoint(&distance_joint);
   }
   
   
   
    //Internal horizontal block
    {
      b2PolygonShape shape;
      shape.SetAsBox(6.0f, 1.2f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0005;
      fd.filter.maskBits = 0xFFFF & ~0x0003;
	
      b2BodyDef bd;
      bd.position.Set(-16.0f, 26.0f);
      ground1 = m_world->CreateBody(&bd);
      ground1->CreateFixture(&fd);
            
      
      shape.SetAsBox(0.4f, 1.0f, b2Vec2(-4.1f, -2.2f), 0);
      ground1->CreateFixture(&fd);
      
  }
  
      //Small block
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.4f, 1.0f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0003;
//      fd.filter.maskBits = 0xFFFF & ~0x0002;
	
      b2BodyDef bd;
      bd.position.Set(-10.4f, 23.8f);
      ground5 = m_world->CreateBody(&bd);
      ground5->CreateFixture(&fd);
      
      
    }
      
/*      b2EdgeShape shape; 
      shape.Set(b2Vec2(-2.5f, 3.5f), b2Vec2(-2.5f, 21.5f));
      b2BodyDef bd;
      ground2 = m_world->CreateBody(&bd);
      
	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.bodyA = ground1;
	prismaticJointDef.bodyB = ground2;
	prismaticJointDef.collideConnected = false;
	
	ground1->CreateFixture(&prismaticJointDef);
	ground2->CreateFixture(&prismaticJointDef);*/
      
      
    
	//Horizontal Spring
    {
     b2DistanceJointDef distance_joint;

	 distance_joint.bodyA = ground5;

	distance_joint.bodyB = ground1;

	distance_joint.localAnchorA.Set(-0.4f,0.0f);

	distance_joint.localAnchorB.Set(-3.7f,-2.2f);

	distance_joint.length = 3.75f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=0.4f;
	distance_joint.dampingRatio=0.5f;
	m_world->CreateJoint(&distance_joint);
   }
   
   
   //Trigger
   	{	      b2Body* b4;
      {
		  
	b2Vec2 vertices[3];
vertices[0].Set(-1.0f, 0.0f);
vertices[1].Set(1.0f, 0.0f);
vertices[2].Set(-3.0f, -5.0f);

int32 count = 3;
b2PolygonShape polygon;
polygon.Set(vertices, count);
	  
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(-5.0f,22.0f);
	b4 = m_world->CreateBody(&bd);
	b4->CreateFixture(&polygon, 2.0f);
      }
       b2BodyDef bd;
      bd.position.Set(-5.0f, 22.0f);
      b2Body* ground = m_world->CreateBody(&bd);
       b2RevoluteJointDef jd;
	  jd.bodyA = ground;
      jd.bodyB = b4;
      jd.collideConnected = false;	
      jd.localAnchorA.Set(0,0);
      jd.localAnchorB.Set(0,0);
      m_world->CreateJoint(&jd);
	  

    }


    //Sliding block
    {
		b2EdgeShape shape; 
      shape.Set(b2Vec2(-13.2f, 1.85f), b2Vec2(13.2f, 1.85f));

      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0005;
      fd.filter.maskBits = 0x0003;
	
      b2BodyDef bd;
      bd.position.Set(-7.3f, 26.65f);
      ground4 = m_world->CreateBody(&bd);
      ground4->CreateFixture(&fd);
      shape.Set(b2Vec2(-13.2f, -1.85f), b2Vec2(0.5f, -1.85f));
      ground4->CreateFixture(&fd);
      shape.Set(b2Vec2(5.5f, -1.85f), b2Vec2(13.2f, -1.85f));
      ground4->CreateFixture(&fd);
      shape.Set(b2Vec2(-13.2f, -1.85f), b2Vec2(-13.2f, 1.85f));
      ground4->CreateFixture(&fd);
      shape.Set(b2Vec2(13.2f, -1.85f), b2Vec2(13.2f, 1.85f));
      ground4->CreateFixture(&fd);
    }
    
      //Firing pin
    {
      b2PolygonShape shape;
      shape.SetAsBox(2.0f, 0.6f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0003;
//      fd.filter.maskBits = 0xFFFF & ~0x0002;
	
      b2BodyDef bd;
      bd.position.Set(1.5f, 26.0f);
      ground6 = m_world->CreateBody(&bd);
      ground6->CreateFixture(&fd);            
    }    

	//Firing Spring
    {
     b2DistanceJointDef distance_joint;

	 distance_joint.bodyA = ground4;

	distance_joint.bodyB = ground6;

	distance_joint.localAnchorA.Set(13.2f,-0.65f);

	distance_joint.localAnchorB.Set(2.0f,0.0f);

	distance_joint.length = 5.0f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=0.4f;
	distance_joint.dampingRatio=0.5f;
	m_world->CreateJoint(&distance_joint);
   }




    }


  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
