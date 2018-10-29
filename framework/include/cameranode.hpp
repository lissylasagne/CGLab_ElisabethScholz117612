#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"

// use gl definitions from glbinding 
using namespace gl;

class CameraNode : public Node {
	public:
		CamerNode(Node* t_parent, std::string t_name, 
			bool t_isPerspective, bool t_isEnabled,
			glm::mat4 t_projectionMatrix);
		bool getPerspective();
		bool getEnabled();
		void setEnabled(bool t_enabled);
		glm::mat4 getProjectionMatrix();
		void setProjectionMatrix(glm::mat4 t_matrix);

	private:
		bool m_isPerspective;
		bool m_isEnabled;
		glm::mat4 m_projectionMatrix;
};

#endif