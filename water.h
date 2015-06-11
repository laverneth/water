// -*- C++ -*-
//


#ifndef __WATER_H__
#define __WATER_H__


#include "scene/2d/node_2d.h"
#include "scene/2d/area_2d.h"
#include "scene/2d/canvas_modulate.h"
#include "scene/resources/shape_2d.h"
#include "scene/resources/rectangle_shape_2d.h"

class WaterColumn : public Area2D {    
  OBJ_TYPE(WaterColumn, Area2D);
public:
  WaterColumn(const Vector2&, const Vector2& delta); 
  void update(float& tension, float& damping) ;    
  void body_enter_shape ( int body_id, Object* body, int body_shape, int area_shape ); 
  float target_height_; 
  float height_ ;  
  float speed_ ;
protected:
  static void _bind_methods();
};

class Water : public Node2D {
  OBJ_TYPE(Water, Node2D);

public:
  Water();
  
  void _notification(int p_what);
  
  // Size in x must be a multiple of resolution
  void set_size(const Rect2&value);
  Rect2 get_size() const;

  
  // Size of the simulation grid
  void set_resolution(const uint32_t&value);
  uint32_t get_resolution() const;

  void set_color(const Color&value);
  Color get_color() const;
  
  /**
   * Physical Parameters
   */
  void set_tension(const float& value);
  float get_tension() const;

  void set_damping(const float&value);
  float get_damping() const;

  void set_spread(const float&value);
  float get_spread() const;



protected:

  static void _bind_methods();
  



  Vector<WaterColumn*> columns_;

  Rect2 rect_ ; 
  uint32_t ncols_ ; 
  uint32_t resolution_ ; 
  Color color_; 
  float damping_ ; 
  float tension_ ;
  float spread_ ; 
  bool size_changed_ ; 
  void _update() ; 
};


#endif
