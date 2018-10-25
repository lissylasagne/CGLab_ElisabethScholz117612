#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"

// use gl definitions from glbinding 
using namespace gl;

class GeometryNode {
	public:
		GeometryNode(node* t_parent, std::string t_name,
			Model t_model);
		Model getGeometry();
		void setGeometry(Model t_geometry);

	private:
		Model m_geometry;

};

#endif