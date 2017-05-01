#include "Camera.h"
#include <glm/ext.hpp>

#include <iostream>

const glm::vec3 Camera::center_position() const
{
  return  glm::vec3(position_.x + front_dir_.x,
                 position_.y + front_dir_.y, 
                 position_.z + front_dir_.z);
}

void Camera::move_forward(float delta)
{
  position_ += delta * front_dir_;  
}

void Camera::move_backward(float delta)
{
  move_forward(-delta);
}

void Camera::move_left(float delta)
{
  position_ -= delta * right_dir_;
}

void Camera::move_right(float delta)
{
  move_left(-delta);
}

void  Camera::move_down(float delta)
{
  position_+=-delta*up_dir_;
}

void Camera::move_up(float delta)
{
  position_+=delta*up_dir_;
}


void Camera::rotate_left(float delta)
{
  front_dir_ = glm::rotateY(front_dir_, delta);
  right_dir_ = glm::cross(front_dir_, up_dir_);
}

void Camera::rotate_right(float delta)
{
  rotate_left(-delta);
}

void Camera::rotate_up(float delta)
{
  front_dir_ = glm::rotateX(front_dir_, delta);
  right_dir_ = glm::cross(front_dir_, up_dir_);
}

void Camera::rotate_down(float delta)
{
  rotate_up(-delta);
}

void Camera::HOME()
{
  position_=glm::vec3(-5.0,3,40);
  front_dir_ = glm::vec3(0,0,-1);
  up_dir_=glm::vec3(0,1,0);
  right_dir_=glm::vec3(1,0,0);
  fovy_=45;
}

  glm::vec3 Camera::check_move_forward(float delta){
    glm::vec3 pos = position_;
    pos+= delta * front_dir_; 
    return pos; 
  }
  glm::vec3 Camera::check_move_backward(float delta){
    return check_move_forward(-delta);
  }
  glm::vec3 Camera::check_move_left(float delta){
      glm::vec3 pos = position_;
      pos -= delta * right_dir_;
    return pos;  
  }
  glm::vec3 Camera::check_move_right(float delta){
      return check_move_left(-delta);
  }
  glm::vec3 Camera::check_move_up(float delta){
      glm::vec3 pos = position_;
    pos +=delta*up_dir_;
    return pos;  
  }
  glm::vec3 Camera::check_move_down(float delta){
      glm::vec3 pos = position_;
    pos +=-delta*up_dir_;
    return pos;  
  } 
