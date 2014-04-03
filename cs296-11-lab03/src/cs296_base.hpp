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


#ifndef _CS296BASE_HPP_
#define _CS296BASE_HPP_

#include "render.hpp"
#include <Box2D/Box2D.h>
#include <cstdlib>

#define	RAND_LIMIT 32767

namespace cs296
{

  //! What is the difference between a class and a struct in C++?
  class base_sim_t;
  struct settings_t;
  
  //! Why do we use a typedef
  typedef base_sim_t* sim_create_fcn(); 

  //! Simulation settings. Some can be controlled in the GUI.
  struct settings_t
  {
    //! Notice the initialization of the class members in the constructor
    //! How is this happening?
    settings_t() :
      view_center(0.0f, 20.0f), /**< view_center is the coordinates of the center of the bottom of the display window. Its coordinates (0.0f, 20.0f)<br>*/
      hz(60.0f), /**< hz is the frequency of simulation. Its value is 60Hz <br>*/
      velocity_iterations(8), /**< velocity_iterations is the number of times constraints are iterated over to calculate the velocity <br>*/
      position_iterations(3), /**< position_iterations is the number of times constraints are iterated over to calculate the position <br>*/
      draw_shapes(1), /**< draw_shapes has two values. A value of 1 implies that shapes will be displayed<br>*/
      draw_joints(1), /**< draw_joints has two values. A value of 1 implies that joints will be displayed<br>*/
      draw_AABBs(0), /**< draw_AABBs has two values. A value of 1 implies that the b2BodyDef object will be displayed<br>*/
      draw_pairs(0), 
      draw_contact_points(0),  /**< draw_contact_points has two values. A value of 1 implies that contact points between colliding objects will be displayed<br>*/
      draw_contact_normals(0), /**< draw_contact_normals has two values. A value of 1 implies that contact normals between colliding objects will be displayed<br>*/
      draw_contact_forces(0), /**< draw_contact_forces has two values. A value of 1 implies that contact forces between colliding objects will be displayed<br>*/
      draw_friction_forces(0), /**< draw_friction_forces has two values. A value of 1 implies that friction forces between colliding objects will be displayed<br>*/
      draw_COMs(0), /**< draw_COMs has two values. A value of 1 implies that the center of mass of objects will be displayed<br>*/
      draw_stats(0),/**< draw_stats has two values. A value of 1 implies that the statistics will be displayed<br>*/
      draw_profile(0), /**< draw_profile has two values. A value of 1 implies that the profile will be displayed<br>*/
      enable_warm_starting(1), /**< enable_warm_starting has two values. A value of 0 implies that one body can pass through another and there is no impact of a collision. <br>*/
      enable_continuous(1), /**enable_continuous enables continuous collision detection <br>*/
      enable_sub_stepping(0), /**< enable_sub_stepping solves the equations again at the instance of collision <br>*/
      pause(0), /**< pause has two values. A value of 1 implies that the simulation will be paused. <br>*/
      single_step(0) /**<single_step has two values. A value of 1 implies that the simulation will be displayed one step at a time.<br>*/
    {}
    
    b2Vec2 view_center; /**< view_center is the coordinates of the center of the bottom of the display window <br>*/
    float32 hz; /**< hz is the frequency of simulation <br>*/
    int32 velocity_iterations; /**< velocity_iterations is the number of times constraints are iterated over to calculate the velocity <br>*/
    int32 position_iterations; /**< position_iterations is the number of times constraints are iterated over to calculate the position <br>*/
    int32 draw_shapes; /**< draw_shapes has two values. A value of 1 implies that shapes will be displayed<br>*/
    int32 draw_joints; /**< draw_joints has two values. A value of 1 implies that joints will be displayed<br>*/
    int32 draw_AABBs; /**< draw_AABBs has two values. A value of 1 implies that the b2BodyDef object will be displayed<br>*/
    int32 draw_pairs;
    int32 draw_contact_points;  /**< draw_contact_points has two values. A value of 1 implies that contact points between colliding objects will be displayed<br>*/
    int32 draw_contact_normals; /**< draw_contact_normals has two values. A value of 1 implies that contact normals between colliding objects will be displayed<br>*/
    int32 draw_contact_forces; /**< draw_contact_forces has two values. A value of 1 implies that contact forces between colliding objects will be displayed<br>*/
    int32 draw_friction_forces; /**< draw_friction_forces has two values. A value of 1 implies that friction forces between colliding objects will be displayed<br>*/
    int32 draw_COMs; /**< draw_COMs has two values. A value of 1 implies that the center of mass of objects will be displayed<br>*/
    int32 draw_stats; /**< draw_stats has two values. A value of 1 implies that the statistics will be displayed<br>*/
    int32 draw_profile; /**< draw_profile has two values. A value of 1 implies that the profile will be displayed<br>*/
    int32 enable_warm_starting; /**< enable_warm_starting has two values. A value of 0 implies that one body can pass through another and there is no impact of a collision. <br>*/
    int32 enable_continuous; /**enable_continuous enables continuous collision detection <br>*/
    int32 enable_sub_stepping; /**< enable_sub_stepping solves the equations again at the instance of collision <br>*/
    int32 pause; /**< pause has two values. A value of 1 implies that the simulation will be paused. <br>*/
    int32 single_step; /**<single_step has two values. A value of 1 implies that the simulation will be displayed one step at a time.<br>*/
  };
  
  struct sim_t 
  { /**< sim_t begins the simulation<br>*/
    const char *name;
    sim_create_fcn *create_fcn;

    sim_t(const char *_name, sim_create_fcn *_create_fcn): 
      name(_name), create_fcn(_create_fcn) {;} /**< an object with the given arguments is created<br>*/
  };
  
  extern sim_t *sim; /**< extern implies that *sim is in another file<br>*/
  
  
  const int32 k_max_contact_points = 2048;  /**< k_max_contact_points is the maximum number of contact points allowed <br>*/
  struct contact_point_t
  {
    b2Fixture* fixtureA; /**< fixtureA stores the height, width, density, friction and restitution of A <br>*/
    b2Fixture* fixtureB; /**< fixtureB stores the height, width, density, friction and restitution of B <br>*/
    b2Vec2 normal; /**< normal has the coordinates of normal vector<br>*/
    b2Vec2 position; /**< position has the coordinates of position vector<br>*/
    b2PointState state; /**< state stores state of point <br>*/
  };
  
  class base_sim_t : public b2ContactListener
  {
  public:
    
    base_sim_t();

    //! Virtual destructors - amazing objects. Why are these necessary?
    virtual ~base_sim_t(); /**< destructor is called<br>*/
    
    void set_text_line(int32 line) { m_text_line = line; } /**< set_text_line sets the line at which debugging messages will be printed <br>*/
    void draw_title(int x, int y, const char *string);/**< draw_title prints the string at (x,y) <br>*/
    
    virtual void step(settings_t* settings); /**< step assigns settings to variables in m_world and m_debug_draw<br>*/

    virtual void keyboard(unsigned char key) { B2_NOT_USED(key); }
    virtual void keyboard_up(unsigned char key) { B2_NOT_USED(key); }

    void shift_mouse_down(const b2Vec2& p) { B2_NOT_USED(p); } /**< Detects shift_mouse_down<br>*/
    virtual void mouse_down(const b2Vec2& p) { B2_NOT_USED(p); } /**< Detects mouse_down<br>*/
    virtual void mouse_up(const b2Vec2& p) { B2_NOT_USED(p); } /**< Detects mouse_up<br>*/
    void mouse_move(const b2Vec2& p) { B2_NOT_USED(p); } /**< Detects mouse_move<br>*/

    
    // Let derived tests know that a joint was destroyed.
    virtual void joint_destroyed(b2Joint* joint) { B2_NOT_USED(joint); } /**<joint_destroyed informs derived tests that a joint has been destroyed.<br>*/
    
    // Callbacks for derived classes.
    virtual void begin_contact(b2Contact* contact) { B2_NOT_USED(contact); } /**< Callbacks for derived classes when contact starts<br>*/
    virtual void end_contact(b2Contact* contact) { B2_NOT_USED(contact); } /**< Callbacks for derived classes when contact ends<br>*/
    virtual void pre_solve(b2Contact* contact, const b2Manifold* oldManifold); /**< Callbacks for derived classes before solving the collision<br>*/
    virtual void post_solve(const b2Contact* contact, const b2ContactImpulse* impulse) 
    {
      B2_NOT_USED(contact);
      B2_NOT_USED(impulse);
    } /**< Callbacks for derived classes after solving the collision*<br>*/

  //!How are protected members different from private memebers of a class in C++ ?
  protected:

    //! What are Friend classes?
    friend class contact_listener_t;
    
    b2Body* m_ground_body; /**< m_ground_body is the ground <br>*/
    b2AABB m_world_AABB; /**<m_world_AABB is the simulation world of AABB. <br>*/
    contact_point_t m_points[k_max_contact_points]; /**< m_points is a vector that stores the contact points between two bodies.<br>*/
    int32 m_point_count; /**<m_point_count stores the maximum number of contact points.<br>*/

    debug_draw_t m_debug_draw; /**<m_debug_draw is used for drawing shapes in the world.<br>*/
    int32 m_text_line; /**<m_text_line stores the number of strings.<br>*/
    b2World* m_world; /**<m_worldD is the world in which the simulation is done<br>*/

    int32 m_step_count; /**<m_step_count stores the maximum number of steps.<br>*/
    
    b2Profile m_max_profile; /**<m_max_profile stores the maximum number of profiles.<br>*/
    b2Profile m_total_profile; /**<m_total_profile stores the total number of profiles.<br>*/
  };
}

#endif
