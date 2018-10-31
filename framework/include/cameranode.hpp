#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"

// use gl definitions from glbinding 
using namespace gl;

class CameraNode : public Node{
	public:
		CameraNode(Node* t_parent, std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world, 
			bool const& t_isPerspective, bool const& t_isEnabled,
			glm::mat4 t_projectionMatrix);
		CameraNode(std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world, 
			bool const& t_isPerspective, bool const& t_isEnabled,
			glm::mat4 t_projectionMatrix);
		bool getPerspective() const;
		bool getEnabled() const;
		void setEnabled(bool t_enabled);
		glm::mat4 getProjectionMatrix() const;
		void setProjectionMatrix(glm::mat4 t_matrix);
	private:
		bool m_isPerspective;
		bool m_isEnabled;
		glm::mat4 m_projectionMatrix;
};

#endif