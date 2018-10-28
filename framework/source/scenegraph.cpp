#include "scenegraph.hpp"

SceneGraph::SceneGraph(std::string const& t_name, Node* t_root) :
	m_name{t_name},
	m_root{t_root}
	{}

std::string Node::getName() const {
	return m_name;
}

void Node::setName(std::string const& t_name) {
	m_name = t_name;
}

Node* Node::getRoot() const {
	return m_root;
}

void Node::setRoot(Node* t_root) {
	m_root = t_root;
}

std::string Node::printGraph() const {
	return t_name;
}