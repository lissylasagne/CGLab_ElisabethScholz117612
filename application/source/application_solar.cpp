#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeShaderPrograms();
  initializePlanets();
  initializeStars(100);
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const {

  //Planets

  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);
  //render
  GeometryNode* sun = dynamic_cast<GeometryNode*>(m_scene.getRoot()->getChildren("sun"));
  GeometryNode* planet1 = dynamic_cast<GeometryNode*>(sun->getChildren("planet1"));
  GeometryNode* planet2 = dynamic_cast<GeometryNode*>(sun->getChildren("planet2"));
  GeometryNode* moon = dynamic_cast<GeometryNode*>(planet2->getChildren("moon"));
  renderPlanet(sun);
  renderPlanet(planet1);
  renderPlanet(planet2);
  renderPlanet(moon);

	//Stars
  renderStars();
}



void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
	glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  /* binding of shader cannot happen in this place, but must be called seperately
  in the functions that upload View and Projection for shader
  glUseProgram(m_shaders.at("planet").handle);
  */
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;

  // store shader program objects in container
  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("stars").u_locs["NormalMatrix"] = -1;
  m_shaders.at("stars").u_locs["ModelMatrix"] = -1;
  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  
  /////////// PLANET GEOMETRY 

  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());


  /////////// STAR GEOMETRY 

  model star_model = model{m_stars, (model::POSITION + model::NORMAL), {0}};

  // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * star_model.data.size(), star_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::NORMAL]);

  // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * star_model.indices.size(), star_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(star_model.indices.size());

}

void ApplicationSolar::renderPlanet(GeometryNode* planet) const {
  glm::fmat4 model_matrix = glm::fmat4{1.0};
   
   if(planet->getName() == "moon") { 
    model_matrix = planet->getParent()->getLocalTransform();
    model_matrix = glm::translate(
      glm::rotate(
        model_matrix, 
        float(glfwGetTime())* dynamic_cast<GeometryNode*>(planet->getParent())->getSpeed(), 
        glm::fvec3{0.0f, 1.0f, 0.0f}),
      glm::fvec3{dynamic_cast<GeometryNode*>(planet->getParent())->getDistance(), 0.0f, 0.0f});
  } else model_matrix = planet->getLocalTransform();

  model_matrix = glm::translate(
    glm::rotate(
      model_matrix, 
      float(glfwGetTime())*planet->getSpeed(), 
      glm::fvec3{0.0f, 1.0f, 0.0f}),
    glm::fvec3{planet->getDistance(), 0.0f, 0.0f});

  // give model_matrix to shader
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  //give normal matrix to shader
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  
}


//iterate trough the SceneGraph and transform and render the Planets
void ApplicationSolar::initializePlanets() {
  
  glm::fmat4 unitmat{ 1.0f, 0.0f, 0.0f, 0.0f, 
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f, 
                      0.0f, 0.0f, 0.0f, 1.0f};
  //create root
  Node* root = new Node("root");
  m_scene = SceneGraph("scene", root);

  //create structure of solar system in scenegraph
  //GeometryNode sun{"sun", unitmat, unitmat, m_planet_model};
  GeometryNode* sun = new GeometryNode("sun", unitmat, unitmat, m_planet_model);

  root->addChildren(sun);
  sun->setDistance(0.0f);
  sun->setSpeed(1.0f);

  GeometryNode* planet1 = new GeometryNode("planet1", unitmat, unitmat, m_planet_model);
  sun->addChildren(planet1);
  planet1->setLocalTransform(glm::fmat4{ 1.0f, 0.0f, 0.0f, 0.0f, 
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 
                                        0.0f, 0.0f, 0.0f, 2.0});
  planet1->setDistance(10.0f);
  planet1->setSpeed(2.4f);


  GeometryNode* planet2 = new GeometryNode("planet2", unitmat, unitmat, m_planet_model);
  sun->addChildren(planet2);
  planet1->setLocalTransform(glm::fmat4{ 1.0f, 0.0f, 0.0f, 0.0f, 
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 
                                        0.0f, 0.0f, 0.0f, 5.0});
  planet1->setDistance(20.0f);
  planet1->setSpeed(0.5f);


  GeometryNode* moon = new GeometryNode("moon", unitmat, unitmat, m_planet_model);
  planet2->addChildren(moon);
  moon->setLocalTransform(glm::fmat4{ 1.0f, 0.0f, 0.0f, 0.0f, 
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f, 
                                        0.0f, 0.0f, 0.0f, 5.0});
  moon->setDistance(5.0f);
  moon->setSpeed(1.0f);
}


void ApplicationSolar::renderStars() const {
  glUseProgram(m_shaders.at("stars").handle);
  glBindVertexArray(star_object.vertex_AO);
  glPointSize(1.0);
  glDrawArrays(star_object.draw_mode, 0, (int)m_stars.size());
  //glDrawElements(star_object.draw_mode, (int)m_stars.size(), model::INDEX.type, NULL);

}


void ApplicationSolar::initializeStars(int numberStars) {
  glm::fmat4 unitmat{ 1.0f, 0.0f, 0.0f, 0.0f, 
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f, 
                      0.0f, 0.0f, 0.0f, 1.0f};
  GeometryNode* starNode = new GeometryNode("stars", unitmat, unitmat, m_star_model);
  m_scene.getRoot()->addChildren(starNode);

  std::vector<float> stars(numberStars*6);

  //6 floats for each star: position and colour
  for(int i = 0; i < numberStars*6; i++) {
    float random = 0.0f;
    if((i/3) % 2 == 0) {  //first 3 floats are position
      random = float(rand() % 200 + (-100));
    }
    else {  //second 3 floats are colour
      random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
    stars[i] = random;
  }
  m_stars = stars;

    std::cout << m_stars.size() << ' ';
}



///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_X  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  } //closer; negative z direction
  else if (key == GLFW_KEY_C  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  } //further; positive z direction
  else if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
    uploadView();
  } //up; negative y direction
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, 0.0f});
    uploadView();
  } //down; positive y direction
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
    uploadView();
  } //left; positive x direction
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  } //right; negative x direction
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  m_view_transform = glm::translate(m_view_transform, glm::fvec3{-(pos_x/100), pos_y/100, 0.0f});
  uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}