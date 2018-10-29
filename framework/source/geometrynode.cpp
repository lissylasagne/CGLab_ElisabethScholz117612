#include "geometrynode.hpp"

GeometryNode::GeometryNode(Node* t_parent, std::string t_name,
	Model t_model) :
	Node{t_parent, t_name},
	m_geometry{t_model}
{}

Model GeometryNode::getGeometry(){
	return m_geometry;
}

void GeometryNode::setGeometry(Model t_geometry){
	m_geometry = t_geometry;
}
