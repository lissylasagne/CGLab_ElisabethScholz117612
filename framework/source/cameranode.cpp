#include "geometry_node.hpp"

CameraNode::CameraNode(node* t_parent, std::string t_name, 
			bool t_isPerspective, bool t_isEnabled,
			glm::mat4 t_projectionMatrix) :
	Node{t_parent, t_name},
	m_isPerspective{t_isPerspective},
	m_isEnabled{t_isEnabled},
	m_projectionMatrix{t_projectionMatrix}
{}

bool CameraNode::getPerspective(){
	return m_isPerspective;
}

bool CameraNode::getEnabled(){
	return m_isEnabled;
}

void setEnabled(bool t_enabled){
	m_isEnabled = t_enabled;
}

std::mat4 getProjectionMatrix(){
	return m_projectionMatrix;
}

void setProjectionMatrix(glm::mat4 t_matrix){
	m_projectionMatrix = t_matrix;
}
