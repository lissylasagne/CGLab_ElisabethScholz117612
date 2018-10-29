#include "geometrynode.hpp"

GeometryNode::GeometryNode(Node* t_parent, std::string const& t_name,
	glm::mat4 const& t_local, glm::mat4 const& t_world,
	model const& t_model) :
	Node{t_parent, t_name, t_local, t_world},
	m_geometry{t_model}
{}

model GeometryNode::getGeometry(){
	return m_geometry;
}

void GeometryNode::setGeometry(model t_geometry){
 	m_geometry = t_geometry;
}