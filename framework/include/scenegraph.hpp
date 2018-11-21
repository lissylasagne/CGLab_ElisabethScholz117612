#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include "node.hpp"
#include "geometrynode.hpp"
#include "cameranode.hpp"
#include "pointlightnode.hpp"

class SceneGraph {
	public:
		SceneGraph();
		SceneGraph(std::string const& t_name, Node* t_root);
		std::string getName() const;
		void setName(std::string const& t_name);
		Node* getRoot() const;
		void setRoot(Node* t_root);
		std::string printGraph() const;

	private:
		std::string m_name;
		Node* m_root;
};

#endif