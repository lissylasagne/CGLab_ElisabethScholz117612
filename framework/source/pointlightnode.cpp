#include "geometrynode.hpp"

PointLightNode::PointLightNode(Node* t_parent, std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model) :
		Node{t_parent, t_name, t_local, t_world},
		m_geometry{t_model} {}

PointLightNode::PointLightNode(std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model) :
		Node{t_name},
		m_geometry{t_model}
{
	setLocalTransform(t_local);
	setWorldTransform(t_world);
}

void PointLightNode::setIntensity(float t_intensity) {
	m_lightIntesity = t_intensity;
}

float PointLightNode::getIntensity() const {
	return m_lightIntesity;
}

void PointLightNode::setColor(glm::fvec3 t_color) {
	m_lightColor = t_color;
}

glm::fvec3 PointLightNode::getColor() const {
	return m_lightColor;
}