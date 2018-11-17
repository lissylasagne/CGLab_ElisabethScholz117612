#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"
#include "model.hpp"

// use gl definitions from glbinding 
using namespace gl;

class PointLightNode : public Node{
	public:
		PointLightNode(Node* t_parent, std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model);
		PointLightNode(std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model);

		void setIntensity(float t_intensity);
		float getIntensity() const;
		void setColor(glm::fvec3 t_color);
		glm::fvec3 getColor() const;

	private:
		float m_lightIntensity;
		glm::fvec3 m_lightColor;

};

#endif
