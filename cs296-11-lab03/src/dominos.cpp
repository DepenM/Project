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
///This body contains the lower part of the trigger system
b2Body* b4;
///This body contains the slider
 b2Body* ground4;
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
    ///This body contains the magazine and the barrel
    b2Body* ground1;
    ///This body contains the piston
    b2Body* ground2;
    ///This body contains the static horizontal block
    b2Body* ground3;
    ///This body contains the small block attached to the muzzle
    b2Body* ground5;
    ///This body contains the firing pin
    b2Body* ground6;
    
    m_world->SetGravity( b2Vec2(0,-0.4) );  
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
      fd.filter.maskBits = 0x0002 | 0x0010;
       
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
      fd.restitution=0.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0007;
    fd.filter.maskBits = ~0xFFFF;
	
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
      fd.density = 0.05f;
      fd.restitution=0.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0002;
      fd.filter.maskBits = 0x0010 | 0x0009 | 0x0013 | 0x0003 | 0x0016;
      fd.filter.groupIndex = 1;
		
      for (int i = 0; i < 4; ++i)
	{
	  b2BodyDef bd;
	  bd.type = b2_dynamicBody;
	  bd.position.Set(1.0f, 16.4f + 2.4f * i);
	  ///This body stores a single bullet
	  b2Body* body = m_world->CreateBody(&bd);
	  body->SetFixedRotation(true);
//	  body->ApplyForce(b2Vec2(0.0f,1000.0f), b2Vec2(1.0f, 16.4f + 2.4f * i), true);
//	  body->SetGravityScale(10);
	  body->CreateFixture(&fd);
	}
	
    }
      
    //Magazine
    {
      b2PolygonShape shape;
      shape.SetAsBox(3.5f, 12.5f);//, b2Vec2(-20.f,20.f), 0.0f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
      fd.filter.categoryBits = 0x0001;
     fd.filter.maskBits = ~0xFFFF;
	
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
      fd.filter.categoryBits = 0x0010;
      fd.filter.maskBits = 0x0002 | 0x0003;
	
      b2BodyDef bd;
      bd.type=b2_dynamicBody;
      bd.position.Set(1.0f, 14.2f);
      ground2 = m_world->CreateBody(&bd);
      ground2->CreateFixture(&fd);
    }
    

    
    
    //Vertical Spring
    {///This is the spring connected to the piston
     b2DistanceJointDef distance_joint;

	 distance_joint.bodyA = ground1;

	distance_joint.bodyB = ground2;

	distance_joint.localAnchorA.Set(0.0f,-12.5f);

	distance_joint.localAnchorB.Set(0.0f,-1.0f);

	distance_joint.length = 140.25f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=0.04f;
	distance_joint.dampingRatio=1.0f;
	m_world->CreateJoint(&distance_joint);
   }
   
   
   
    //Barrel
    {
      b2PolygonShape shape;
      shape.SetAsBox(6.0f, 1.2f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0011;
     fd.filter.maskBits = ~0xFFFF;
	
      b2BodyDef bd;
      bd.position.Set(-16.0f, 26.0f);
      ground1 = m_world->CreateBody(&bd);
      ground1->CreateFixture(&fd);
            
      
 //     shape.SetAsBox(0.4f, 1.0f, b2Vec2(-4.1f, -2.2f), 0);
  //    ground1->CreateFixture(&fd);
      
  }
  
      //Small block
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.4f, 1.0f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0006;
      fd.filter.maskBits = ~0xFFFF;
	
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
      


    //Slide
    {
		b2EdgeShape shape; 
      shape.Set(b2Vec2(-13.2f, 1.85f), b2Vec2(13.2f, 1.85f));

      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
//      fd.friction = 0.0f;
      fd.filter.categoryBits = 0x0009;
      fd.filter.maskBits = 0x0002 | 0x0008;
      
 //     b2MassData * md=new b2MassData;
 //     md->center=b2Vec2(-7.3f, 26.65f);
 //     md->mass=1000000.0f;
 //     md->I=0.0f;
      
	
      b2BodyDef bd;
      bd.position.Set(-7.3f, 26.65f);
      bd.type=b2_dynamicBody;
      ground4 = m_world->CreateBody(&bd);
    //  ground4->SetMassData(md);
      ground4->CreateFixture(&fd);
      shape.Set(b2Vec2(-13.2f, -1.85f), b2Vec2(-0.5f, -1.85f));
      ground4->CreateFixture(&fd);
      shape.Set(b2Vec2(5.55f, -1.65f), b2Vec2(13.2f, -1.65f));
      ground4->CreateFixture(&fd);
      fd.filter.maskBits = 0x0008;
      shape.Set(b2Vec2(-13.2f, -1.85f), b2Vec2(-13.2f, 1.85f));
      ground4->CreateFixture(&fd);
      fd.filter.maskBits = 0x0002 | 0x0008;
      shape.Set(b2Vec2(13.2f, -1.85f), b2Vec2(13.2f, 1.85f));
      ground4->CreateFixture(&fd);
      
      shape.Set(b2Vec2(5.5f, -0.85f), b2Vec2(5.5f, -1.2f));
      fd.filter.categoryBits = 0x0016;
      fd.filter.maskBits = 0x0002;
      ground4->CreateFixture(&fd);
      
      
      b2PolygonShape shape1;
      shape1.SetAsBox(0.4f, 1.0f, b2Vec2(-12.8f, -2.85f),0);
      b2FixtureDef fd1;
      fd1.shape = &shape1;
      fd1.restitution=0.0f;
      //fd1.density=10.0f;
      fd1.filter.categoryBits = 0x0014;
      fd1.filter.maskBits = 0x0015;
      ground4->CreateFixture(&fd1);
      
  //    ground4->ApplyLinearImpulse( b2Vec2(500.0f, 0.0f), b2Vec2(-20.5f, 26.65f), true);
    }

      
    
	//Recoil Spring
    {///This is the spring connecting the small block and the slider
     b2DistanceJointDef distance_joint;

	 distance_joint.bodyA = ground5;

	distance_joint.bodyB = ground4;

	distance_joint.localAnchorA.Set(-0.4f,0.0f);

	distance_joint.localAnchorB.Set(-12.4f, -2.85f);

	distance_joint.length = 8.9f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=0.1f;
	distance_joint.dampingRatio=0.6f;
	m_world->CreateJoint(&distance_joint);
   }
   

    
    
    {///This joint ensures that the slider only moves along the x-axis
		b2PrismaticJointDef prismaticJointDef;
  prismaticJointDef.bodyA = ground4;
  prismaticJointDef.bodyB = ground3;
  prismaticJointDef.collideConnected = false;
   prismaticJointDef.localAxisA.Set(1,0);
      prismaticJointDef.localAnchorA.Set( 0.0f,0.0f);
  prismaticJointDef.localAnchorB.Set(0.7f,1.65f);
    m_world->CreateJoint(&prismaticJointDef);	
	}
		
    
      //Firing pin
    {
      b2PolygonShape shape;
      shape.SetAsBox(2.0f, 0.4f);
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 0.8f;
//      fd.friction = 0.1f;
      fd.filter.categoryBits = 0x0008;
     fd.filter.maskBits =0x0002 | 0x0005 | 0x0009;


      b2BodyDef bd;
      bd.type = b2_dynamicBody;
      bd.position.Set(1.5f, 26.0f);
      ground6 = m_world->CreateBody(&bd);
      ground6->CreateFixture(&fd);            
      ///This joint ensures that the firing pin only moves along the x-axis
      	       b2PrismaticJointDef prismaticJointDef;
  prismaticJointDef.bodyA = ground6;
  prismaticJointDef.bodyB = ground3;
  prismaticJointDef.collideConnected = false;
   prismaticJointDef.localAxisA.Set(1,0);
      prismaticJointDef.localAnchorA.Set( 0.0f,0.0f);//a little outside the bottom right corner
  prismaticJointDef.localAnchorB.Set(9.5f,1.0f);//bottom left corner
    m_world->CreateJoint(&prismaticJointDef);
    }    

	//Firing Spring
    {///This is the spring connected to the firing pin
     b2DistanceJointDef distance_joint;

	 distance_joint.bodyA = ground4;

	distance_joint.bodyB = ground6;

	distance_joint.localAnchorA.Set(13.2f,-0.65f);

	distance_joint.localAnchorB.Set(2.0f,0.0f);

	distance_joint.length = 6.0f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=3.0f;
	distance_joint.dampingRatio=0.0f;
	m_world->CreateJoint(&distance_joint);
   }
   
  //Blocking edges
  {
	  b2Body* ground;
	  b2EdgeShape shape; 
      shape.Set(b2Vec2(-7.5f, 0.0f), b2Vec2(2.5f, 0.0f));

      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
	  fd.friction = 0.0f;
      fd.filter.categoryBits = 0x0013;
      fd.filter.maskBits = 0x0002;
      
      b2BodyDef bd;
      
      bd.position.Set(1.0f, 27.3f);
      ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&fd);
      fd.friction = 50.0f;
      shape.Set(b2Vec2(-9.5f, 0.0f), b2Vec2(-7.5f, 0.0f));
      ground->CreateFixture(&fd);
      shape.Set(b2Vec2(-9.5f, -2.5f), b2Vec2(-7.5f, -2.5f));
      ground->CreateFixture(&fd);
      fd.friction = 0.0f;
      shape.Set(b2Vec2(-7.5f, -2.5f), b2Vec2(-2.8f, -2.5f));
      ground->CreateFixture(&fd);
      shape.Set(b2Vec2(-21.5f, -4.6f), b2Vec2(-21.5f, 0.0f));
      fd.restitution = 0.0f;
      fd.filter.categoryBits = 0x0015;
      fd.filter.maskBits = 0x0014;
      ground->CreateFixture(&fd);
      
  }

   
      //Trigger System
   	{	      //b2Body* b4;
	///This body contains the upper part of the trigger system
   	b2Body* b5;
      {
		  
	b2Vec2 vertices[4];
vertices[0].Set(-1.0f, 0.0f);
vertices[1].Set(0.0f, 0.0f);
vertices[2].Set(1.5f, -3.0f);
vertices[3].Set(0.0f, -2.0f);

int32 count = 4;
b2PolygonShape polygon;
polygon.Set(vertices, count);
b2FixtureDef fd2;
fd2.shape=&polygon;
fd2.density=2.0f;
fd2.filter.categoryBits = 0x0004;
fd2.filter.maskBits = 0x0005;
	b2Vec2 vertices3[3];
vertices3[0].Set(0.0f, -2.0f);
vertices3[1].Set(1.5f, -3.0f);
vertices3[2].Set(-2.0f, -4.0f);

int32 count3 = 3;
b2PolygonShape polygon3;
polygon3.Set(vertices3, count3);
b2FixtureDef fd3;
fd3.shape=&polygon3;
fd3.density=2.0f;
fd3.filter.categoryBits = 0x0004;
fd3.filter.maskBits = 0x0005;
	b2Vec2 vertices1[3];
vertices1[0].Set(0.75f, -1.5f);
vertices1[1].Set(5.0f, 0.0f);
vertices1[2].Set(5.0f, 2.0f);

int32 count1 = 3;
b2PolygonShape polygon1;
polygon1.Set(vertices1, count1);
b2FixtureDef fd;
fd.shape=&polygon1;
fd.density=2.0f;
fd.filter.categoryBits = 0x0004;
fd.filter.maskBits = 0x0005;
	b2Vec2 vertices2[4];
vertices2[0].Set(5.0f, 0.0f);
vertices2[1].Set(5.0f, 2.0f);
vertices2[2].Set(7.0f, 2.0f);
vertices2[3].Set(7.0f, 0.0f);

int32 count2 = 4;
b2PolygonShape polygon2;
polygon2.Set(vertices2, count2);
b2FixtureDef fd1;
fd1.shape=&polygon2;
fd1.density=2.0f;
fd1.filter.maskBits = 0x0005;
fd1.filter.categoryBits = 0x0004;
b2Vec2 vertices4[3];
vertices4[0].Set(4.0f, 2.0f);
vertices4[1].Set(2.0f, 2.0f);
vertices4[2].Set(2.875f, 0.25f);

int32 count4 = 3;
b2PolygonShape polygon4;
polygon4.Set(vertices4, count4);
b2FixtureDef fd4;
fd4.shape=&polygon4;
fd4.density=2.0f;
fd4.filter.categoryBits = 0x0004;
fd4.filter.maskBits = 0x0005;

	b2Vec2 vertices5[4];
vertices5[0].Set(3.0f, 3.0f);
vertices5[1].Set(4.0f, 3.0f);
vertices5[2].Set(4.0f, 2.0f);
vertices5[3].Set(3.0f, 2.0f);

int32 count5 = 4;
b2PolygonShape polygon5;
polygon5.Set(vertices5, count5);
b2FixtureDef fd5;
fd5.shape=&polygon5;
fd5.density=2.0f;
fd5.filter.categoryBits = 0x0005;
fd5.filter.maskBits = 0x0004 | 0x0008 | 0x0009;

	b2Vec2 vertices6[4];
vertices6[0].Set(3.0f, 4.1f);
vertices6[1].Set(4.0f, 4.1f);
vertices6[2].Set(4.0f, 5.5f);
vertices6[3].Set(3.0f, 5.5f);

int32 count6 = 4;
b2PolygonShape polygon6;
polygon6.Set(vertices6, count6);
b2FixtureDef fd6;
fd6.shape=&polygon6;
fd6.density=2.0f;
fd6.filter.maskBits = 0x0004 | 0x0008 | 0x0009;
fd6.filter.categoryBits = 0x0005;
	b2Vec2 vertices7[4];
vertices7[0].Set(3.25f, 3.0f);
vertices7[1].Set(3.75f, 3.0f);
vertices7[2].Set(3.75f, 4.1f);
vertices7[3].Set(3.25f, 4.1f);

int32 count7 = 4;
b2PolygonShape polygon7;
polygon7.Set(vertices7, count7);
b2FixtureDef fd7;
fd7.shape=&polygon7;
fd7.density=2.0f;
fd7.filter.categoryBits = 0x0012;
fd7.filter.maskBits = 0x0001;


	  
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(-5.0f,22.0f);
	b4 = m_world->CreateBody(&bd);
	b5 = m_world->CreateBody(&bd);
	b4->CreateFixture(&fd2);
	b4->CreateFixture(&fd3);
	b4->CreateFixture(&fd);
	b4->CreateFixture(&fd1);
	b4->CreateFixture(&fd4);
	b5->CreateFixture(&fd5);
	b5->CreateFixture(&fd6);
	b5->CreateFixture(&fd7);
///This is the horizontal spring in the trigger system
	  b2DistanceJointDef distance_joint;
 
	 distance_joint.bodyA = ground3;

	distance_joint.bodyB = b4;

	distance_joint.localAnchorA.Set(12.5f,-2.0f);

	distance_joint.localAnchorB.Set(7.0f,1.0f);

	distance_joint.length = 2.5f;

	distance_joint.collideConnected = true;
	distance_joint.frequencyHz=0.4f;
	distance_joint.dampingRatio=0.5f;
	m_world->CreateJoint(&distance_joint);
///This is the vertical spring in the trigger system	
		  b2DistanceJointDef distance_joint1;
 
	 distance_joint1.bodyA = ground4;

	distance_joint1.bodyB = b5;

	distance_joint1.localAnchorA.Set(5.8f,1.85f);

	distance_joint1.localAnchorB.Set(3.5f,5.5f);

	distance_joint1.length = 2.0f;

	distance_joint1.collideConnected = true;
	distance_joint1.frequencyHz=2.0f;
	distance_joint1.dampingRatio=0.5f;
	m_world->CreateJoint(&distance_joint1);
	
	
	
      }
       b2BodyDef bd;
      bd.position.Set(-5.0f, 22.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ///This joint ensures that the  upper part of the trigger system only moves vertically
       b2PrismaticJointDef prismaticJointDef;
  prismaticJointDef.bodyA = ground4;
  prismaticJointDef.bodyB = b5;
  prismaticJointDef.collideConnected = false;
   prismaticJointDef.localAxisA.Set(0,1);
      prismaticJointDef.localAnchorA.Set( 2.3f,-4.65f);
  prismaticJointDef.localAnchorB.Set(0.0f,0.0f);
    m_world->CreateJoint(&prismaticJointDef);
    ///This joint is used for rotating the trigger
       b2RevoluteJointDef jd;
	  jd.bodyA = ground;
      jd.bodyB = b4;
      jd.collideConnected = false;	
      jd.localAnchorA.Set(0,0);
      jd.localAnchorB.Set(0,0);
      m_world->CreateJoint(&jd);
// b4->ApplyAngularImpulse( 100 ,true); 

    }





    }


  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
