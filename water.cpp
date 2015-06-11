#include "water.h"

#include <iostream>
#include "scene/2d/physics_body_2d.h"

void WaterColumn::update(float& tension, float& damping) {
  float dh = target_height_ - height_ ; 
  speed_ = tension*dh -damping*speed_ ; 
  height_ += speed_ ; 
}

WaterColumn::WaterColumn(const Vector2& pos, 
			 const Vector2& delta) : target_height_(delta.y), 
						 height_(delta.y), 
						 speed_(0.0){
  Ref<RectangleShape2D> shape = memnew(RectangleShape2D); 
  shape->set_extents(delta) ; 
  
  Matrix32 m ; 
  // set matrix origin
  m[2] = pos; 
  add_shape(shape, m) ; 
  connect("body_enter_shape", this, "body_enter_shape"); 

}

void WaterColumn::body_enter_shape ( int body_id, Object* body, int body_shape, int area_shape ){
  RigidBody2D *obj = body->cast_to<RigidBody2D>();
  if(obj){ 
    speed_ = (obj-> get_linear_velocity().length());
  }
  
}

void WaterColumn::_bind_methods(){
  ObjectTypeDB::bind_method(_MD("body_enter_shape","bodyshape"),&WaterColumn::body_enter_shape);
}

Water::Water() {
  set_size(Rect2(0.,0.,100.,20.));
  resolution_ = 5 ;
  color_ = Color(1,1,1,1);
  damping_ = 0.025;
  tension_ = 0.025; 
  spread_  = 0.25 ; 
  size_changed_ = true; 
  _update();

  set_fixed_process(true);
}

void Water::_update() {

  // Is size changed,
  if(size_changed_){
    float dx = rect_.size.x;
    float dy = rect_.size.y;
    size_changed_ = false ; 
    ncols_ = uint32_t(dx/resolution_);  
    for(int i=0; i<columns_.size(); ++i){
      memdelete(columns_[i]) ; 
    }
    columns_.clear();
    float x = rect_.pos.x ; 
    float y = rect_.pos.y ; 
    for(uint32_t i=0; i<ncols_; ++i){
      WaterColumn* col = memnew(WaterColumn(Vector2(x,y), Vector2(resolution_, dy))); 
      columns_.push_back(col) ; 
      add_child(col) ;       
      x += resolution_ ; 
    }
  }
  update() ; 
}


void Water::_notification(int p_what) {

  switch(p_what) {
    
  case NOTIFICATION_ENTER_TREE: {
    update() ; 
  } break;

  case NOTIFICATION_LOCAL_TRANSFORM_CHANGED: {    
    if (!is_inside_tree())
      break;
  } break;

  case NOTIFICATION_FIXED_PROCESS: { 
    int n = columns_.size() ; 
    double l[n] ; 
    double r[n] ; 
    for(int i=0; i<n; ++i){
      columns_[i]->update(tension_, damping_) ; 
    }
    for(int iter=0; iter<8; ++iter){
      for(int i=0; i<n; ++i){
	if(i>0){
	  l[i] = spread_*(columns_[i]->height_ - columns_[i-1]->height_) ; 
	  columns_[i-1]->speed_ += l[i];
	}
	if(i<n-1){
	  r[i] = spread_*(columns_[i]->height_ - columns_[i+1]->height_) ; 
	  columns_[i+1]->speed_ += r[i];
	}
      }
      for(int i=0; i<n; ++i){
	if(i>0){
	  columns_[i-1]->height_ += l[i];
	}
	if(i<n-1){
	  columns_[i+1]->height_ += r[i];
	}
      }
    }
  
    update() ; 

  }break ; 
    
  case NOTIFICATION_DRAW: { 
    Vector2 pos = rect_.pos ; 
    for(uint32_t i=0; i<ncols_; ++i){
      draw_rect(Rect2(pos, Vector2(resolution_, columns_[i]->height_)), color_);
      pos.x += resolution_ ; 
    }
    
  } break ; 
    
  }

}


void Water::set_size(const Rect2& rect) {
  rect_ = rect ;
  size_changed_ = true ; 
  _update() ; 
}

Rect2 Water::get_size() const {
  return rect_ ; 
}

void Water::set_resolution(const uint32_t& resolution) {
  resolution_ = resolution ;
  size_changed_ = true ; 
  _update();
}

uint32_t Water::get_resolution() const {
  return resolution_ ; 
}

void Water::set_color(const Color& color) {
  color_ = color ;
  size_changed_ = false; 
  _update() ; 
}

Color Water::get_color() const {
  return color_ ; 
}

void Water::set_damping(const float& val) {
  damping_ = val ;
}

float Water::get_damping() const {
  return damping_ ; 
}

void Water::set_tension(const float& val) {
  tension_ = val ;
}

float Water::get_tension() const {
  return tension_ ; 
}

void Water::set_spread(const float& val) {
  spread_ = val ;
}

float Water::get_spread() const {
  return spread_ ; 
}

void Water::_bind_methods() {

  ObjectTypeDB::bind_method(_MD("set_size","size"),&Water::set_size);
  ObjectTypeDB::bind_method(_MD("get_size"),&Water::get_size);
  ADD_PROPERTY( PropertyInfo(Variant::RECT2,"Size"),_SCS("set_size"),_SCS("get_size"));

  ObjectTypeDB::bind_method(_MD("set_resolution","resolution"),&Water::set_resolution);
  ObjectTypeDB::bind_method(_MD("get_resolution"),&Water::get_resolution);
  ADD_PROPERTY( PropertyInfo(Variant::INT,"Resolution"),_SCS("set_resolution"),_SCS("get_resolution"));

  ObjectTypeDB::bind_method(_MD("set_color","color"),&Water::set_color);
  ObjectTypeDB::bind_method(_MD("get_color"),&Water::get_color);
  ADD_PROPERTY( PropertyInfo(Variant::COLOR,"Color"),_SCS("set_color"),_SCS("get_color"));


  ObjectTypeDB::bind_method(_MD("set_damping","damping"),&Water::set_damping);
  ObjectTypeDB::bind_method(_MD("get_damping"),&Water::get_damping);
  ADD_PROPERTY( PropertyInfo(Variant::REAL,"Damping"),_SCS("set_damping"),_SCS("get_damping"));

  ObjectTypeDB::bind_method(_MD("set_tension","tension"),&Water::set_tension);
  ObjectTypeDB::bind_method(_MD("get_tension"),&Water::get_tension);
  ADD_PROPERTY( PropertyInfo(Variant::REAL,"Tension"),_SCS("set_tension"),_SCS("get_tension"));

  ObjectTypeDB::bind_method(_MD("set_spread","spread"),&Water::set_spread);
  ObjectTypeDB::bind_method(_MD("get_spread"),&Water::get_spread);
  ADD_PROPERTY( PropertyInfo(Variant::REAL,"Spread"),_SCS("set_spread"),_SCS("get_spread"));
}

