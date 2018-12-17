#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "scenegraph.hpp"
#include "structs.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;

 protected:
  //initialization
  void initializeShaderPrograms();
  void initializePlanetGeometry();
  void initializeStarGeometry();
  void initializePlanets();
  void initializeStars(int numberStars);
  void initializeSkybox();
  void initializeFramebuffer();

  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  //rendering
  void renderPlanets() const;
  void renderPlanet(GeometryNode* planet) const;
  void renderStars() const;

  glm::fmat4 makeModelMatrix(GeometryNode* planet) const;

  //misc
  SceneGraph* getSceneGraph() const;

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object texture_object;
  model_object skybox_object;

  texture_object frame_buffer_texture;
  texture_object frame_buffer_object;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;

  // camera projection matrix
  glm::fmat4 m_view_projection;

  //Edited: Member Variables to hold Scene Graph and the Planet Model
  //which will used for the planets

  SceneGraph m_scene;
  std::vector<float> m_stars;
  model m_planet_model;
  model m_star_model;
  std::string m_shading_mode;
};

#endif