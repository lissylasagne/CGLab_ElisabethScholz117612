#include "geometrynode.hpp"

GeometryNode::GeometryNode(Node* t_parent, std::string const& t_name,
		glm::mat4 const& t_local, glm::mat4 const& t_world,
		model const& t_model) :
	Node{t_parent, t_name, t_local, t_world},
	m_geometry{t_model}
{}

GeometryNode::GeometryNode(std::string const& t_name,
		glm::mat4 const& t_local, glm::mat4 const& t_world,
		model const& t_model) :
	Node{t_name},
	m_geometry{t_model}
{
	setLocalTransform(t_local);
	setWorldTransform(t_world);
}

model GeometryNode::getGeometry(){
	return m_geometry;
}

void GeometryNode::setGeometry(model t_geometry){
 	m_geometry = t_geometry;
}

void GeometryNode::setDistance(float t_distance) {
	m_distanceFromOrigin = t_distance;
}

float GeometryNode::getDistance() const {
	return m_distanceFromOrigin;
}

void GeometryNode::setSpeed(float t_speed) {
	m_speed = t_speed;
}

float GeometryNode::getSpeed() const {
	return m_speed;
}

void GeometryNode::setColor(glm::fvec3 t_color) {
	m_color = t_color;
}

glm::fvec3 GeometryNode::getColor() const{
	return m_color;
}

void GeometryNode::setSize(float t_size){
	m_size = t_size;
}

float GeometryNode::getSize() const{
	return m_size;
}