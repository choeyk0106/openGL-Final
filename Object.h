#pragma once
#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

struct Group
{
public:
  Group(const std::string& name);
    
public:
  std::string               m_name;

  std::vector<glm::vec3>    m_vertices;
  std::vector<glm::vec3>    m_normals;
  std::vector<glm::vec2>    m_texcoords;
  std::string               m_mtl_name;
};

struct Material
{
public:  
  Material();
  Material(const std::string& name, 
  glm::vec4& ambient, glm::vec4& diffuse, glm::vec4& specular, float& shininess);
  
public:
  std::string     m_name;

  glm::vec4       m_ambient;
  glm::vec4       m_diffuse;
  glm::vec4       m_specular;
  float           m_shininess;
};

class Object
{
public:
  Object() 
  :max_vx(0), max_vy(0), max_vz(0), min_vx(0), min_vy(0), min_vz(0)
  {}

  void draw(int loc_a_vertex, int loc_a_normal, int loc_a_texcoord);
  void circle_draw(int loc_a_vertex, int loc_a_normal, int loc_a_texcoord);
  void print();

  float max_x(float trans, float scale);
  float max_y(float trans, float scale);
  float max_z(float trans, float scale);
  float min_x(float trans, float scale);
  float min_y(float trans, float scale);
  float min_z(float trans, float scale);

  float get_max_x();
  float get_max_y();
  float get_max_z();
  float get_min_x();
  float get_min_y();
  float get_min_z();

  glm::vec3 get_max_pos();
  glm::vec3 get_min_pos();
	
  bool load_simple_obj(const std::string& filename);
  bool load_simple_mtl(const std::string& filename);

private:  
  std::string PATH;
  std::vector<Group>              m_groups;
  std::map<std::string, Material> m_materials;  

  float max_vx, max_vy, max_vz, min_vx, min_vy, min_vz;
};