#include "scenegraph.hpp"

SceneGraph::SceneGraph() :
	m_name{"default"},
	m_root{nullptr}
{}

SceneGraph::SceneGraph(std::string const& t_name, Node* t_root) :
	m_name{t_name},
	m_root{t_root}
	{}

std::string SceneGraph::getName() const {
	return m_name;
}

void SceneGraph::setName(std::string const& t_name) {
	m_name = t_name;
}

Node* SceneGraph::getRoot() const {
	return m_root;
}

void SceneGraph::setRoot(Node* t_root) {
	m_root = t_root;
}

std::string SceneGraph::printGraph() const {
	return m_name;
}
