#include "cameranode.hpp"

CameraNode::CameraNode(Node* t_parent, std::string const& t_name, 
			glm::mat4 const& t_local, glm::mat4 const& t_world, 
			bool const& t_isPerspective, bool const& t_isEnabled,
			glm::mat4 t_projectionMatrix) :
	Node{t_parent, t_name, t_local, t_world},
	m_isPerspective{t_isPerspective},
	m_isEnabled{t_isEnabled},
	m_projectionMatrix{t_projectionMatrix}
{}

bool CameraNode::getPerspective() const{
	return m_isPerspective;
}

bool CameraNode::getEnabled() const{
	return m_isEnabled;
}

void CameraNode::setEnabled(bool t_enabled){
	m_isEnabled = t_enabled;
}

glm::mat4 CameraNode::getProjectionMatrix() const{
	return m_projectionMatrix;
}

void CameraNode::setProjectionMatrix(glm::mat4 t_matrix){
	m_projectionMatrix = t_matrix;
}
