#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"

// use gl definitions from glbinding 
using namespace gl;

class Camera_Node {
	public:
		bool getPerspective();
		bool getEnabled();
		void setEnabled(bool t_enabled);
		std::mat4 getProjectionMatrix();
		void setProjectionMatrix(std::mat4 t_matrix);

	private:
		bool m_isPerspective;
		bool m_isEnabled;
		std::mat4 m_projectionMatrix;
};

#endif