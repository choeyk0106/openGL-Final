// main.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glu.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Shader.h"
#include "Object.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SOIL.h>

#define PI 3.141592f

void init();
void load_texture();
void create_min_max(Object& obj, glm::vec3 trans, float scale);
void background();
void rightground();
void leftground();
void floorground();

bool check_foward_camera();
bool check_backward_camera();
bool check_up_camera();
bool check_down_camera();
bool check_right_camera();
bool check_left_camera();

bool check_foward(Object& obj, Camera cam);
bool check_backward(Object& obj, Camera cam);
bool check_up(Object& obj, Camera cam);
bool check_down(Object& obj, Camera cam);
bool check_right(Object& obj, Camera cam);
bool check_left(Object& obj, Camera cam);

void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);
void processMouse(int button, int state, int x, int y); 
void processMousePassiveMotion(int, int);

GLuint      program;

GLint       loc_a_vertex, loc_a_normal;
GLint       loc_a_texcoord;

GLint       loc_u_pvm_matrix, loc_mv_matrix, loc_normal_matrix;
GLint       loc_u_texid;

glm::mat4   mat_PVM, mat_MV;
glm::mat4   mat_Proj, mat_View, mat_Model;

glm::vec3   Earth_max, Earth_min;
glm::vec3   BumbleBee1_max, BumbleBee1_min, BumbleBee2_max, BumbleBee2_min;
glm::vec3   Ironhide1_max, Ironhide1_min, Ironhide2_max, Ironhide2_min;
glm::vec3   Mirage1_max, Mirage1_min, Mirage2_max, Mirage2_min;
glm::vec3   Optimus1_max, Optimus1_min, Optimus2_max, Optimus2_min;
glm::vec3   Soundwave1_max, Soundwave1_min, Soundwave2_max, Soundwave2_min;

float       aspect_ratio = 1.0f;
float       mouse_x = 0.0f, mouse_y = 0.0f, mouse_z = 0.0f;

GLuint      textureid, textureid1, textureid2; 
GLuint      tex1_earth;
GLuint      tex1_Bumblebee, tex2_Bumblebee;
GLuint      tex1_IronHide, tex2_IronHide;
GLuint      tex1_Mirage, tex2_Mirage;
GLuint      tex1_Optimus, tex2_Optimus;
GLuint      tex1_Soundwave, tex2_Soundwave;

std::vector<glm::vec4> vertices;
std::vector<glm::vec2> texcoords;

Object      g_earth;
Object      g_BumbleBee, g_BumbleBee1, g_IronHide, g_IronHide1, g_Mirage, g_Mirage1;      // object
Object      g_Optimus, g_Optimus1, g_Soundwave, g_Soundwave1;
Camera      g_camera;     // viewer (you)

static float earth_rotate_degree = 0.0f;
static float obj_rotate_degree = 0.0f;
static float car_rotate_degree = 0.0f;

static bool obj_rotate_flag = false;
static bool car_rotate_flag = false;

int pre_MousePos_x;
int pre_MousePos_y;

int specialKey; 

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(1024, 640);
  glutCreateWindow("Image Texture Mapping");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMouseFunc(processMouse);
   glutPassiveMotionFunc(processMousePassiveMotion); // Detect Mouse Move
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK)
  {
    std::cerr << "failed to initialize glew" << std::endl;
    return -1;
  }

  init();

  glutMainLoop();

  return 0;
}

void init()
{
  g_earth.load_simple_obj("./data/earth/earth.obj");
  g_BumbleBee.load_simple_obj("./data/BumbleBee/RB-BumbleBee.obj");
  g_BumbleBee1.load_simple_obj("./data/BumbleBee/VH-BumbleBee.obj");
  g_IronHide.load_simple_obj("./data/Iron Hide/RB-IronHide.obj");
  g_IronHide1.load_simple_obj("./data/Iron Hide/VH-IronHide.obj");
  g_Mirage.load_simple_obj("./data/Mirage/RB-Mirage.obj");
  g_Mirage1.load_simple_obj("./data/Mirage/VH-Mirage.obj");
  g_Optimus.load_simple_obj("./data/Optimus/RB-OptimusBoss.obj");
  g_Optimus1.load_simple_obj("./data/Optimus/VH-Optimus.obj");
  g_Soundwave.load_simple_obj("./data/Soundwave/RB-SoundWave.obj");
  g_Soundwave1.load_simple_obj("./data/Soundwave/VH-SoundWave.obj");

  create_min_max(g_earth, glm::vec3(-5.0, 10.0, 3.8), 0.01);
  create_min_max(g_BumbleBee, glm::vec3(10.0, 0.0, 10.0), 0.01);
  create_min_max(g_BumbleBee1, glm::vec3(6.5, 0.0, 9.0), 0.01);
  create_min_max(g_Soundwave, glm::vec3(6.0, 0.0, 3.8), 0.01);
  create_min_max(g_Soundwave1, glm::vec3(2.0, 0.0, 2.0), 0.01);
  create_min_max(g_Optimus, glm::vec3(2.0, 0.0, -3.5), 0.01);
  create_min_max(g_Optimus1, glm::vec3(-2.0, 0.0, -6.0), 0.01);
  create_min_max(g_IronHide, glm::vec3(-15.0, 0.0, 0.0), 0.01);
  create_min_max(g_IronHide1, glm::vec3(-12.0, 0.0, -3.0), 0.01);
  create_min_max(g_Mirage, glm::vec3(-19.0, 0.0, 8.0), 0.01);
  create_min_max(g_Mirage1, glm::vec3(-17.0, 0.0, 6.0), 0.01);

  vertices.push_back(glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f));
  vertices.push_back(glm::vec4(1.0f, -1.0f, 0.0f, 1.0f));
  vertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  vertices.push_back(glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f));

  // texture coordinates
  texcoords.push_back(glm::vec2(0.0f, 0.0f));
  texcoords.push_back(glm::vec2(1.0f, 0.0f));
  texcoords.push_back(glm::vec2(1.0f, 1.0f));
  texcoords.push_back(glm::vec2(0.0f, 1.0f));

  load_texture();

  glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  program = Shader::create_program("./shader/texture.vert", "./shader/texture.frag");

  loc_a_vertex = glGetAttribLocation(program, "a_vertex");
  loc_a_texcoord = glGetAttribLocation(program, "a_texcoord");
  loc_a_normal = glGetAttribLocation(program, "a_normal");

  loc_u_pvm_matrix = glGetUniformLocation(program, "u_pvm_matrix");
  loc_normal_matrix = glGetUniformLocation(program, "normal_matrix");
  loc_u_texid = glGetUniformLocation(program, "u_texid");
  loc_mv_matrix = glGetUniformLocation(program, "mv_matrix");
  
}

void create_min_max(Object& obj, glm::vec3 trans, float scale){

  obj.max_x(trans.x, scale); obj.max_y(trans.y, scale); obj.max_z(trans.z, scale);
  obj.min_x(trans.x, scale); obj.min_y(trans.y, scale); obj.min_z(trans.z, scale);
}

void load_texture()
{
  int width, height, channels;
  GLubyte* image_data;

    textureid = SOIL_load_OGL_texture
    (
      "./data/back1.jpg",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    textureid1 = SOIL_load_OGL_texture
    (
      "./data/back2.jpg",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    textureid2 = SOIL_load_OGL_texture
    (
      "./data/LibertyStatue/Liberty-GreenBronze-1.bmp",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex1_Bumblebee = SOIL_load_OGL_texture
    (
      "./data/BumbleBee/VH_BumblebeeNew_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex2_Bumblebee = SOIL_load_OGL_texture
    (
      "./data/BumbleBee/RB_Bumblebee_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex1_IronHide = SOIL_load_OGL_texture
    (
      "./data/Iron Hide/RB_Ironhide_TexSet_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex2_IronHide = SOIL_load_OGL_texture
    (
      "./data/Iron Hide/VH_Ironhide_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex1_Mirage = SOIL_load_OGL_texture
    (
      "./data/Mirage/RB_Mirage_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex2_Mirage = SOIL_load_OGL_texture
    (
      "./data/Mirage/VH_Mirage_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex1_earth = SOIL_load_OGL_texture
    (
      "./data/earth/4096_earth.jpg",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex1_Optimus = SOIL_load_OGL_texture
    (
      "./data/Optimus/RB_Optimus_TextSet_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex2_Optimus = SOIL_load_OGL_texture
    (
      "./data/Optimus/VH_Optimus_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

      tex1_Soundwave = SOIL_load_OGL_texture
    (
      "./data/Soundwave/RB_Soundwave_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );

    tex2_Soundwave = SOIL_load_OGL_texture
    (
      "./data/Soundwave/VH_SoundwaveT3_TEXTSET_Color_NormX.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
      );    

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  //glm::mat4   mat_Proj, mat_View, mat_Model;

  mat_View = glm::lookAt(
    g_camera.position(),        // eye position
    g_camera.center_position(),     // center position
    g_camera.up_direction()     // up direction 
    );

  mat_Proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 1000.0f);

  //draw earth
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex1_earth);
  glUniform1i(loc_u_texid, 0);

  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 10.0, 3.8));
  mat_Model *= glm::rotate(earth_rotate_degree,glm::vec3(0,1,0));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_earth.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw BumbleBee
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex2_Bumblebee);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(10.0, 0.0, 10.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/2,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(PI/4,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(obj_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_BumbleBee.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw BumbleBeeCar
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex1_Bumblebee);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(6.5, 0.0, 9.0));
 mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/2,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(PI/4,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(car_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_BumbleBee1.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw Ironhide
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex1_IronHide);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-15.0, 0.0, 0.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/6,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(obj_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_IronHide.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw IronHideCar
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex2_IronHide);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-12.0, 0.0, -3.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(-PI/4,glm::vec3(0,0,1));
  mat_Model *= glm::rotate(car_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_IronHide1.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw Mirage
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex1_Mirage);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-19.0, 0.0, 8.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/6,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(obj_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_Mirage.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw MirageCar
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex2_Mirage);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-17.0, 0.0, 6.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/6,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(car_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_Mirage1.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw Optimus
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex1_Optimus);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0, 0.0, -3.5));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/2,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(PI/4,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(obj_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_Optimus.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw OptimusCar
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex2_Optimus);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.0, 0.0, -6.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/2,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(PI/4,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(car_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_Optimus1.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw Soundwave
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex1_Soundwave);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(6.0, 0.0, 3.8));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/2,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(PI/4,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(obj_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_Soundwave.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  //draw SoundwaveCar
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex2_Soundwave);
  glUniform1i(loc_u_texid, 0);
  
  mat_Model = glm::mat4(1.0);
  mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0, 0.0, 2.0));
  mat_Model *= glm::rotate(-PI/2,glm::vec3(1,0,0));
  mat_Model *= glm::rotate(PI/2,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(PI/4,glm::vec3(0,0,-1));
  mat_Model *= glm::rotate(car_rotate_degree,glm::vec3(0,0,1));
  mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_View*mat_Model;
  glUniformMatrix4fv(loc_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix3fv(loc_normal_matrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(mat_MV)))));
  g_Soundwave1.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord);

  background();
  leftground();
  rightground();
  //floorground();
  
  glUseProgram(0);

  Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  aspect_ratio = width / (float)height;
}

void idle()
{
   if(earth_rotate_degree==360){
    earth_rotate_degree = 0;
  }
  earth_rotate_degree +=0.01f;

  if(obj_rotate_flag){
    if(obj_rotate_degree==360){
      obj_rotate_degree = 0;
    }
    obj_rotate_degree +=0.003f;
  }

  if(car_rotate_flag){
    if(car_rotate_degree==360){
      car_rotate_degree = 0;
    }
    car_rotate_degree +=0.003f;
  }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{  
  if ('a' == key || 'A' == key)
  {
    g_camera.rotate_left(0.1);
  }
  else if ('d' == key || 'D' == key)
  {
    g_camera.rotate_right(0.1);
  }
    else if ('s' == key || 'S' == key)
  {
    g_camera.rotate_up(0.1);
  } 
  else if ('w' == key || 'W' == key)
  {
    g_camera.rotate_down(0.1);
  }
  else if('+'==key || '='==key)
  {
    if(check_foward_camera())
      g_camera.move_forward(1.0);
  }
  else if('-'==key || '_'==key)
  {
    if(check_backward_camera())
    g_camera.move_backward(1.0);
  }

  glutPostRedisplay();
}

void special(int key, int x, int y)
{ 
  switch (key)
  {
  case GLUT_KEY_UP:
  if(check_up_camera())
    g_camera.move_up(1.0);
    break;
  case GLUT_KEY_DOWN:
  if(check_down_camera())
    g_camera.move_down(1.0);
    break;
  case GLUT_KEY_LEFT:
  if(check_left_camera())
    g_camera.move_left(1.0);
    break;
  case GLUT_KEY_RIGHT:
  if(check_right_camera())
    g_camera.move_right(1.0);
    break;
  case GLUT_KEY_HOME:
    g_camera.HOME();
    break;
  default:
    break;
  }

  glutPostRedisplay();
} 

void processMouse(int button, int state, int x, int y) {

  glm::vec4 pos;

  specialKey = glutGetModifiers();
  if ((state == GLUT_DOWN) && (specialKey == GLUT_ACTIVE_ALT)) {

    if (button == GLUT_LEFT_BUTTON) 
    {
          obj_rotate_flag = !obj_rotate_flag;
    }
    else if (button == GLUT_MIDDLE_BUTTON) 
    {
        
    }
    else if (button == GLUT_RIGHT_BUTTON) 
    {
      car_rotate_flag = !car_rotate_flag;
    }
  }
} 

void processMousePassiveMotion(int x, int y)
{
  specialKey = glutGetModifiers();
    if(pre_MousePos_x - x < 0)
    {
        g_camera.rotate_left(0.006f);
          pre_MousePos_x = x;
         
         if(pre_MousePos_y - y <0)
         {
             g_camera.rotate_up(0.003f);
             pre_MousePos_y = y;
         }
          
         else
         {
              g_camera.rotate_down(0.003f);
              pre_MousePos_y = y;
         }
    }

    else
    {
        g_camera.rotate_right(0.006f);
        pre_MousePos_x = x;

         if(pre_MousePos_y - y <0)
        {
                g_camera.rotate_up(0.003f);
                pre_MousePos_y = y;
         }
     
         else
        {
                g_camera.rotate_down(0.003f);
                pre_MousePos_y = y;
          }
    }
  
} 

bool check_foward_camera(){
  if(!check_foward(g_earth, g_camera))
    return false;
  if(!check_foward(g_BumbleBee, g_camera))
    return false;
  if(!check_foward(g_BumbleBee1, g_camera))
    return false;
  if(!check_foward(g_Soundwave, g_camera))
    return false;
  if(!check_foward(g_Soundwave1, g_camera))
    return false;
  if(!check_foward(g_Optimus, g_camera))
    return false;
  if(!check_foward(g_Optimus1, g_camera))
    return false;
  if(!check_foward(g_IronHide, g_camera))
    return false;
  if(!check_foward(g_IronHide1, g_camera))
    return false;
  if(!check_foward(g_Mirage, g_camera))
    return false;
  if(!check_foward(g_Mirage1, g_camera))
    return false;

  return true;
}

bool check_backward_camera(){
  if(!check_backward(g_earth, g_camera))
    return false;
  if(!check_backward(g_BumbleBee, g_camera))
    return false;
  if(!check_backward(g_BumbleBee1, g_camera))
    return false;
  if(!check_backward(g_Soundwave, g_camera))
    return false;
  if(!check_backward(g_Soundwave1, g_camera))
    return false;
  if(!check_backward(g_Optimus, g_camera))
    return false;
  if(!check_backward(g_Optimus1, g_camera))
    return false;
  if(!check_backward(g_IronHide, g_camera))
    return false;
  if(!check_backward(g_IronHide1, g_camera))
    return false;
  if(!check_backward(g_Mirage, g_camera))
    return false;
  if(!check_backward(g_Mirage1, g_camera))
    return false;

  return true;
}

bool check_up_camera(){
  if(!check_up(g_earth, g_camera))
    return false;
  if(!check_up(g_BumbleBee, g_camera))
    return false;
  if(!check_up(g_BumbleBee1, g_camera))
    return false;
  if(!check_up(g_Soundwave, g_camera))
    return false;
  if(!check_up(g_Soundwave1, g_camera))
    return false;
  if(!check_up(g_Optimus, g_camera))
    return false;
  if(!check_up(g_Optimus1, g_camera))
    return false;
  if(!check_up(g_IronHide, g_camera))
    return false;
  if(!check_up(g_IronHide1, g_camera))
    return false;
  if(!check_up(g_Mirage, g_camera))
    return false;
  if(!check_up(g_Mirage1, g_camera))
    return false;

  return true;
}

bool check_down_camera(){
  if(!check_down(g_earth, g_camera))
    return false;
  if(!check_down(g_BumbleBee, g_camera))
    return false;
  if(!check_down(g_BumbleBee1, g_camera))
    return false;
  if(!check_down(g_Soundwave, g_camera))
    return false;
  if(!check_down(g_Soundwave1, g_camera))
    return false;
  if(!check_down(g_Optimus, g_camera))
    return false;
  if(!check_down(g_Optimus1, g_camera))
    return false;
  if(!check_down(g_IronHide, g_camera))
    return false;
  if(!check_down(g_IronHide1, g_camera))
    return false;
  if(!check_down(g_Mirage, g_camera))
    return false;
  if(!check_down(g_Mirage1, g_camera))
    return false;

  return true;
}

bool check_right_camera(){
  if(!check_right(g_earth, g_camera))
    return false;
  if(!check_right(g_BumbleBee, g_camera))
    return false;
  if(!check_right(g_BumbleBee1, g_camera))
    return false;
  if(!check_right(g_Soundwave, g_camera))
    return false;
  if(!check_right(g_Soundwave1, g_camera))
    return false;
  if(!check_right(g_Optimus, g_camera))
    return false;
  if(!check_right(g_Optimus1, g_camera))
    return false;
  if(!check_right(g_IronHide, g_camera))
    return false;
  if(!check_right(g_IronHide1, g_camera))
    return false;
  if(!check_right(g_Mirage, g_camera))
    return false;
  if(!check_right(g_Mirage1, g_camera))
    return false;

  return true;
}

bool check_left_camera(){
  if(!check_left(g_earth, g_camera))
    return false;
  if(!check_left(g_BumbleBee, g_camera))
    return false;
  if(!check_left(g_BumbleBee1, g_camera))
    return false;
  if(!check_left(g_Soundwave, g_camera))
    return false;
  if(!check_left(g_Soundwave1, g_camera))
    return false;
  if(!check_left(g_Optimus, g_camera))
    return false;
  if(!check_left(g_Optimus1, g_camera))
    return false;
  if(!check_left(g_IronHide, g_camera))
    return false;
  if(!check_left(g_IronHide1, g_camera))
    return false;
  if(!check_left(g_Mirage, g_camera))
    return false;
  if(!check_left(g_Mirage1, g_camera))
    return false;

  return true;
}

bool  check_foward(Object& obj, Camera cam){
  if(obj.get_min_x()-1 <= cam.check_move_forward(1).x && cam.check_move_forward(1).x <= obj.get_max_x()+1){
    if(obj.get_min_y()-1 <= cam.check_move_forward(1).y && cam.check_move_forward(1).y <= obj.get_max_y()+1){
      if(obj.get_min_z()-1 <= cam.check_move_forward(1).z && cam.check_move_forward(1).z <= obj.get_max_z()+1){
        return false;
      }
    }
  }
  return true;
}

bool  check_backward(Object& obj, Camera cam){

  if(obj.get_min_x()-1 <= cam.check_move_backward(1).x && cam.check_move_backward(1).x <= obj.get_max_x()+1){
    if(obj.get_min_y()-1 <= cam.check_move_backward(1).y && cam.check_move_backward(1).y <= obj.get_max_y()+1){
      if(obj.get_min_z()-1 <= cam.check_move_backward(1).z && cam.check_move_backward(1).z <= obj.get_max_z()+1){
        return false;
      }
    }
  }
  return true;
}

bool  check_up(Object& obj, Camera cam){

  if(obj.get_min_x()-1 <= cam.check_move_up(1).x && cam.check_move_up(1).x <= obj.get_max_x()+1){
    if(obj.get_min_y()-1 <= cam.check_move_up(1).y && cam.check_move_up(1).y <= obj.get_max_y()+1){
      if(obj.get_min_z()-1 <= cam.check_move_up(1).z && cam.check_move_up(1).z <= obj.get_max_z()+1){
        return false;
      }
    }
  }
  return true;
}

bool  check_down(Object& obj, Camera cam){

  if(obj.get_min_x()-1 <= cam.check_move_down(1).x && cam.check_move_down(1).x <= obj.get_max_x()+1){
    if(obj.get_min_y()-1 <= cam.check_move_down(1).y && cam.check_move_down(1).y <= obj.get_max_y()+1){
      if(obj.get_min_z()-1 <= cam.check_move_down(1).z && cam.check_move_down(1).z <= obj.get_max_z()+1){
        return false;
      }
    }
  }
  return true;
}

bool  check_right(Object& obj, Camera cam){

  if(obj.get_min_x()-1 <= cam.check_move_right(1).x && cam.check_move_right(1).x <= obj.get_max_x()+1){
    if(obj.get_min_y()-1 <= cam.check_move_right(1).y && cam.check_move_right(1).y <= obj.get_max_y()+1){
      if(obj.get_min_z()-1 <= cam.check_move_right(1).z && cam.check_move_right(1).z <= obj.get_max_z()+1){
        return false;
      }
    }
  }
  return true;
}

bool  check_left(Object& obj, Camera cam){

  if(obj.get_min_x()-1 <= cam.check_move_left(1).x && cam.check_move_left(1).x <= obj.get_max_x()+1){
    if(obj.get_min_y()-1 <= cam.check_move_left(1).y && cam.check_move_left(1).y <= obj.get_max_y()+1){
      if(obj.get_min_z()-1 <= cam.check_move_left(1).z && cam.check_move_left(1).z <= obj.get_max_z()+1){
        return false;
      }
    }
  }
  return true;
}

void background(){
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureid1);
  glUniform1i(loc_u_texid, 0);

    mat_Model = glm::mat4(1.0);
    mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, 14.0, -30.0));
    //mat_Model *= glm::rotate(PI/2,glm::vec3(-1,0,0));
    //mat_Model *= glm::rotate(-PI/3,glm::vec3(1,0,0));
    mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(64.0f,64.0f,0.0f));
    mat_PVM = mat_Proj*mat_View*mat_Model;

   glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
   glVertexAttribPointer(loc_a_vertex, 4, GL_FLOAT, false, 0, vertices.data());
   glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, false, 0, texcoords.data());
   glEnableVertexAttribArray(loc_a_vertex);
   glEnableVertexAttribArray(loc_a_texcoord);
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   glDisableVertexAttribArray(loc_a_vertex);
   glDisableVertexAttribArray(loc_a_texcoord);
}

void rightground(){
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureid1);
  glUniform1i(loc_u_texid, 0);

    mat_Model = glm::mat4(1.0);
    mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(60.0, 14.0, -20.0));
    mat_Model *= glm::rotate(PI/2,glm::vec3(0,-1,0));
    mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(64.0f,64.0f,0.0f));
    mat_PVM = mat_Proj*mat_View*mat_Model;

   glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
   glVertexAttribPointer(loc_a_vertex, 4, GL_FLOAT, false, 0, vertices.data());
   glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, false, 0, texcoords.data());
   glEnableVertexAttribArray(loc_a_vertex);
   glEnableVertexAttribArray(loc_a_texcoord);
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   glDisableVertexAttribArray(loc_a_vertex);
   glDisableVertexAttribArray(loc_a_texcoord);
}

void leftground(){

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureid1);
  glUniform1i(loc_u_texid, 0);

    mat_Model = glm::mat4(1.0);
    mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-64.0, 14.0, -20.0));
    mat_Model *= glm::rotate(PI/2,glm::vec3(0,1,0));
    mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(64.0f,64.0f,0.0f));
    mat_PVM = mat_Proj*mat_View*mat_Model;

   glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
   glVertexAttribPointer(loc_a_vertex, 4, GL_FLOAT, false, 0, vertices.data());
   glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, false, 0, texcoords.data());
   glEnableVertexAttribArray(loc_a_vertex);
   glEnableVertexAttribArray(loc_a_texcoord);
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   glDisableVertexAttribArray(loc_a_vertex);
   glDisableVertexAttribArray(loc_a_texcoord);
}

void floorground(){

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureid1);
  glUniform1i(loc_u_texid, 0);

    mat_Model = glm::mat4(1.0);
    mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, -40.0, -20.0));
    mat_Model *= glm::rotate(PI/2,glm::vec3(-1,0,0));
    //mat_Model *= glm::rotate(-PI/3,glm::vec3(1,0,0));
    mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(64.0f));
    mat_PVM = mat_Proj*mat_View*mat_Model;

   glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
   glVertexAttribPointer(loc_a_vertex, 4, GL_FLOAT, false, 0, vertices.data());
   glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, false, 0, texcoords.data());
   glEnableVertexAttribArray(loc_a_vertex);
   glEnableVertexAttribArray(loc_a_texcoord);
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   glDisableVertexAttribArray(loc_a_vertex);
   glDisableVertexAttribArray(loc_a_texcoord);
}
