#include "geometry_node.hpp"

GeometryNode::GeometryNode(node* t_parent, std::string t_name,
	Model t_model) :
	Node{t_parent, t_name},
	m_model{t_model}
{}

Model GeometryNode::getGeometry(){
	return m_model;
}

void GeometryNode::setGeometry(Model t_geometry){
	m_geometry = t_geometry;
}