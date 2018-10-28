#include "node.hpp"


/*
Node::Node(Node const& parent, std::string const& name, glm::mat4 const& localTransform, glm::mat4 const& worldTransform) :
	
	m_parent{parent},
	m_children{},
	m_name{name},
	m_path{}, //TODO parent path + this node
	m_depth{}, //TODO parnet depth +1
	m_localTransform{localTransform},
	m_worldTransform{worldTransform}
	{
		//m_path = m_parent -> getPath() + m_name;
		//m_depth = m_parent -> getDepth()+1;	
	}

*/

Node::Node(Node* parent, std::string const& name, glm::mat4 const& local, glm::mat4 const& world) :
	
	m_parent{parent},
	m_children{},
	m_name{name},
	m_path{},
	m_depth{},
	m_localTransform{local},
	m_worldTransform{world}
	{
		if(m_parent!= nullptr) {
			m_path = m_parent -> getPath() + m_name;
		} else {
			m_path = m_name;
		}

		if(m_parent!= nullptr) {
			m_depth = m_parent -> getDepth()+1;	
		} else {
			m_depth = 0;
		}

		m_parent -> addChildren(this);
	}

Node* Node::getParent() const {
	return m_parent;
}

void Node::setParent(Node* parent) {
	m_parent = parent;
	
}

Node* Node::getChildren(std::string t_child) const {
	
	for (auto it = m_children.begin(); it != m_children.end(); it++) {
		
		if((*it) -> getName() == t_child) {
			return *it;
		}
	}
	
	return nullptr;
}

std::list<Node*> Node::getChildrenList() const {
	return m_children;
}

std::string Node::getName() const {
	return m_name;
}

std::string Node::getPath() const {
	return m_path;
}

int Node::getDepth() const {
	return m_depth;
}

glm::mat4 Node::getLocalTransform() const {
	return m_localTransform;
}

void Node::setLocalTransform(glm::mat4 t_local) {
	m_localTransform = t_local;
}

glm::mat4 Node::getWorldTransform() const {
	return m_worldTransform;	
}

void Node::setWorldTransform(glm::mat4 t_world) {
	m_worldTransform = t_world;
}

void Node::addChildren(Node* t_child) {
	m_children.push_back(t_child);
}

Node* Node::removeChildren(std::string t_child) {
	this -> m_children.remove(this -> getChildren(t_child));
}