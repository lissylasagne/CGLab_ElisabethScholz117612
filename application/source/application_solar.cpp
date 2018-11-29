#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "texture_loader.hpp"

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
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 40.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,m_shading_mode{"blinn_phong"}
{
  // init for Planets - for some reason, only sun + one planet are rendered if
  // initPlanets() is dome before initGeometry() ???
  m_planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
  initializePlanetGeometry();
  initializePlanets();

  //init for Stars
  initializeStars(3000); //fills m_stars with random star positions and colors
  initializeStarGeometry(); //uses m_stars to create m_star_model

  //init shaders
  initializeShaderPrograms();

  //init textures
  initializeTextures();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);

  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);

}

void ApplicationSolar::render() const {
	renderPlanets();
  renderStars();
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);

  // upload matrix to gpu

  //PLANETS
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  //STARS
  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu

  //PLANETS
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  //STARS
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
  //blinn phong
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/planet.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet.frag"}}});
  // request uniform locations for shader program
  
  //simple shader uniforms
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1; // = daraus camera position ableiten?
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet").u_locs["PlanetColor"] = -1; // = diffuse and ambient color
  m_shaders.at("planet").u_locs["NormalTexture"] = -1;

  //planet shader specific uniforms
  m_shaders.at("planet").u_locs["LightPosition"] = -1;
  m_shaders.at("planet").u_locs["LightColor"] = -1;
  m_shaders.at("planet").u_locs["LightIntensity"] = -1;

  //ShaderMode uniform
  m_shaders.at("planet").u_locs["ShaderMode"] = -1;

  //STARS

  // store shader program objects in container
  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/star.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/star.frag"}}});
  // request uniform locations for shader program
  //no normal or model matrix required for drawing points
  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
}

void ApplicationSolar::initializePlanetGeometry() {
  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_planet_model.data.size(), m_planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, m_planet_model.vertex_bytes, m_planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, m_planet_model.vertex_bytes, m_planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * m_planet_model.indices.size(), m_planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(m_planet_model.indices.size());

}

void ApplicationSolar::initializeStarGeometry() {
  /////////// STAR GEOMETRY  EDITED VERSION
  //Model c'tor: (databuff is vector of GLfloats, List of used Attributes, trianglebuff = std::vector<GLuint>{})
  //data is stored in m_stars, Position and Normal are used attributes (same as with planet_model),
  //and trianglebuff is instanciated with the integer vector {0}

  // Position is still Position, Normal is not actually a normal, but the
  // three floats that describe the color of the star -> so we basically
  // misuse the model class for the stars
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * star_model.data.size(), 
    star_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // activate first attribute on gpu (POSITION, 3 Floats)
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, 
    GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::POSITION]);
   
  // !!!! Again, not really the Normale, but the color, but in the same position

  // activate second attribute on gpu (NORMALE, 3 Floats)
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats WITH OFFSET TO FLOAT AT INDEX 3 (start of normale) type is void pointer
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type,
   GL_FALSE, star_model.vertex_bytes, star_model.offsets[model::NORMAL]);

  //Deleted generation of generic buffer

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;

  // transfer number of indices to model object
  // Number of Elements is 6 times smaller than the number of floats in the star_model flaot vector!
  star_object.num_elements = GLsizei(star_model.indices.size()/6); 
}

//iterate trough the SceneGraph and transform and render the Planets
void ApplicationSolar::initializePlanets() {
  
  // Colors
  glm::fvec3 white    = glm::fvec3{1.0f,1.0f,1.0f};

  glm::fvec3 green    = glm::fvec3{0.0f,0.6f,0.0f};
  glm::fvec3 red      = glm::fvec3{0.6f,0.0f,0.0f};
  glm::fvec3 blue     = glm::fvec3{0.0f,0.0f,0.6f};

  glm::fvec3 yellow   = glm::fvec3{0.8f,0.8f,0.1f};

  glm::fmat4 unitmat = glm::fmat4{1.0f};

  //create root
  Node* root = new Node("root");
  m_scene = SceneGraph("scene", root);

  //create structure of solar system in scenegraph
  GeometryNode* sun = new GeometryNode("sun", unitmat, unitmat, m_planet_model);
  root->addChildren(sun);
  sun->setDistance(0.0f);
  sun->setSpeed(1.0f);
  sun->setSize(4.0f);
  sun->setColor(yellow); //yellow

  GeometryNode* planet1 = new GeometryNode("planet1", unitmat, unitmat, m_planet_model);
  sun->addChildren(planet1);
  planet1->setDistance(15.0f);
  planet1->setSpeed(1.4f);
  planet1->setSize(2.0f);
  planet1->setColor(green);

  GeometryNode* planet2 = new GeometryNode("planet2", unitmat, unitmat, m_planet_model);
  sun->addChildren(planet2);
  planet2->setDistance(30.0f);
  planet2->setSpeed(0.5f);
  planet2->setSize(5.0f);
  planet2->setColor(red);

  GeometryNode* moon = new GeometryNode("moon", unitmat, unitmat, m_planet_model);
  planet2->addChildren(moon);
  moon->setDistance(5.0f);
  moon->setSpeed(0.5f);
  moon->setSize(1.0f);
  moon->setColor(blue);

  PointLightNode* sunlight = new PointLightNode("sunlight", unitmat, unitmat);
  root->addChildren(sunlight);
  sunlight->setIntensity(0.6f);
  sunlight->setColor(white);
}

void ApplicationSolar::initializeStars(int numberStars) {
  //6 floats for each star: position and colour
  for(int i = 0; i < numberStars*6; i++) {
    //float random = 0.0f;
    if((i/3) % 2 == 0) {  //first 3 floats are position
      m_stars.push_back(float(rand() % 100 + (-50)));
    }
    else {  //second 3 floats are colour
      m_stars.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    }
  }
}

void ApplicationSolar::initializeTextures() {
  //std::string texture_path = m_resource_path + "textures/katze1.png";
  pixel_data data = texture_loader::file(m_resource_path + "textures/katze1.png"); 
/*
  glActiveTexture(GL_TEXTURE*);
  glGenTexture(tex_num, &texture_object);
  glBindTexture(target, texture_object);

  glTexParametri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParametri(target, GL_TEXTURE_MAX_FILTER, GL_LINEAR);

  glTexImage2D(target, level, internalformat, width, height, border, format, type, data);

  glActiveTexture(GL_TEXTURE k);
  glBindTexture(target, texture_object);
  
  int sampler_location = glGetUniformLocation(program_handle, ‘‘YourTexture’’);
  glUseProgram(program_handle);
  glUniform1i(sampler_location, k);

  uniform Sampler2D YourTexture;
  vec4 colour_from_tex = texture*(YourTexture, tex_coords);
  
*/

  texture_object t0;
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &t0.handle);
  glBindTexture(GL_TEXTURE_2D, t0.handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, data.channels, data.width, data.height, 0, data.channels, data.channel_type, data.ptr());
/*
  std::string path_normals = m_resource_path + "normal_textures/" + std::to_string(i) + ".png";
  pixel_data pix_dat_normal = texture_loader::file(path_normals); 

  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &normal_texture[i].handle);
  glBindTexture(GL_TEXTURE_2D, normal_texture[i].handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, pix_dat_normal.channels, pix_dat_normal.width, pix_dat_normal.height, 0, pix_dat_normal.channels, pix_dat_normal.channel_type, pix_dat_normal.ptr());
*/
}


///////////////////////////// rendering functions /////////////////////////

//deal with traversing the SceneGraph
void ApplicationSolar::renderPlanets() const{
	glUseProgram(m_shaders.at("planet").handle);

  GeometryNode* sun = dynamic_cast<GeometryNode*>(m_scene.getRoot()->getChildren("sun"));
  GeometryNode* planet1 = dynamic_cast<GeometryNode*>(sun->getChildren("planet1"));
  GeometryNode* planet2 = dynamic_cast<GeometryNode*>(sun->getChildren("planet2"));
  GeometryNode* moon = dynamic_cast<GeometryNode*>(planet2->getChildren("moon"));
  renderPlanet(sun);
  renderPlanet(planet1);
  renderPlanet(planet2);
  renderPlanet(moon);
}

//deal with gl
void ApplicationSolar::renderPlanet(GeometryNode* planet) const {
  glm::fmat4 model_matrix = glm::fmat4{1.0};
  
  //TODO
  //the moon specific transformation should be shifted to updating the world_transformation with the parents and local transformations
  //scaling klappt nicht so richtig... 
  //model_matrix = glm::scale(model_matrix, glm::fvec3(planet->getSize()));

  // if the planet is a moon, then the transformations to the parent planets position need to happen first
  if(planet->getName() == "moon") { 
    GeometryNode* parentPlanet = dynamic_cast<GeometryNode*>(planet->getParent());
    model_matrix = parentPlanet->getLocalTransform();
    model_matrix = glm::rotate(model_matrix, float(glfwGetTime())* parentPlanet->getSpeed(), glm::fvec3{0.0f, 1.0f, 0.0f});
    model_matrix = glm::translate(model_matrix,glm::fvec3{parentPlanet->getDistance(), 0.0f, 0.0f});
  }
  // if not a moon, use localTransform
  else {
    model_matrix = planet->getLocalTransform();
  }

  // FIRST scale THEN rotate and THEN translate!
  //model_matrix = glm::scale(model_matrix, glm::fvec3(planet->getSize()));
  model_matrix = glm::rotate(model_matrix, float(glfwGetTime())*planet->getSpeed(), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{planet->getDistance(), 0.0f, 0.0f});

  // give model_matrix to shader
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  glm::fvec3 planetColor = planet->getColor();
  glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), planetColor.x, planetColor.y, planetColor.z);
/*
  //planet textures
  //was noch gemacht werden muss: eine funktion die einen vector planet_handle anlegt, in dem texture_objects sind, die dann auch in initializeTextures genutz werden
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, planet_texture[?].handle); //? = anzahl der planeten 
  int color_sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "ColorTex");
  glUniform1i(color_sampler_location, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, normal_texture[10].handle);
  glUniform1i(glGetUniformLocation(m_shaders.at("planet").handle, "NormalTexture"), 1);
*/
  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  //give normal matrix to shader
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  //give sunlight data to shader
  PointLightNode* sunlightNode = dynamic_cast<PointLightNode*>(m_scene.getRoot()->getChildren("sunlight"));
  glm::fvec3 lightColor = sunlightNode->getColor();
  glUniform3f(m_shaders.at("planet").u_locs.at("LightPosition"), 0.0f, 0.0f, 0.0f);
  glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"), sunlightNode->getIntensity());
  glUniform3f(m_shaders.at("planet").u_locs.at("LightColor"), lightColor.x, lightColor.y, lightColor.z);

  //give shadermode to shader
  if(m_shading_mode == "blinn_phong"){
    glUniform1i(m_shaders.at("planet").u_locs.at("ShaderMode"), 1); 
  }
  else{
    glUniform1i(m_shaders.at("planet").u_locs.at("ShaderMode"), 2);
  }

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  
}

void ApplicationSolar::renderStars() const {
  glUseProgram(m_shaders.at("stars").handle);
  glBindVertexArray(star_object.vertex_AO);
  //glPointSize(1.0);
  glDrawArrays(star_object.draw_mode, 0, (int)m_stars.size());
}

///////////////////////////// callback functions for window events ////////////

// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_R  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  } //closer; negative z direction
  else if (key == GLFW_KEY_E  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
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
  
  else if(key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_shading_mode = "blinn_phong";
    uploadView();
    uploadProjection();
  }

  else if(key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_shading_mode = "cel_shading";
    uploadView();
    uploadProjection();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
   if (pos_x > 0){
        m_view_transform = glm::rotate(m_view_transform, 0.005f,glm::fvec3{0.0f, 1.0f, 0.0f});
    }
    else if(pos_x < 0){
        m_view_transform = glm::rotate(m_view_transform, -0.005f,glm::fvec3{0.0f, 1.0f, 0.0f});
    }
    if(pos_y > 0){
        m_view_transform = glm::rotate(m_view_transform, 0.005f,glm::fvec3{1.0f, 0.0f, 0.0f});
    } 
    else if(pos_y < 0){
        m_view_transform = glm::rotate(m_view_transform, -0.005f,glm::fvec3{1.0f, 0.0f, 0.0f});
}
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