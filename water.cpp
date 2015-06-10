#include "water.h"
#include "scene/resources/rectangle_shape_2d.h"
#include <iostream>
 
void Water::WaterColumn::update(float& tension, float& damping) {
  float dh = target_height_ - height_ ; 
  speed_ = tension*dh -damping*speed_ ; 
  height_ += speed_ ; 
}

Water::WaterColumn::WaterColumn(float height) : target_height_(height), 
						height_(height), 
						speed_(0.0){
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
  columns_.clear();
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
      columns_.push_back(memnew(WaterColumn(dy))) ; 
 
      RectangleShape2D shape ;
      // set rectangle sizes
      shape.set_extents(Vector2(resolution_, dy)) ; 

      Matrix32 m ; 
      // set matrix origin
      m[2] = Vector2(x, y) ; 
      //columns_[i]->area_.add_shape(, m) ; 
      
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
    
  case NOTIFICATION_DRAW: { 
    Vector2 pos = rect_.pos ; 
    for(uint32_t i=0; i<ncols_; ++i){
      Color color(color_); 
      color.a = i/float(ncols_) ; 
      draw_rect(Rect2(pos, Vector2(resolution_, columns_[i]->height_)), color);
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

