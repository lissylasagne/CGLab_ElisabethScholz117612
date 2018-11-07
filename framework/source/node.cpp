#include "node.hpp"
#include <iostream>

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

Node::Node(std::string const& name):
	m_parent{},
	m_children{std::list<Node*>()},
	m_name{name},
	m_path{name},
	m_depth{0},
	m_localTransform{},
	m_worldTransform{}
{
	std::cout<<"Default Constructor was called.\n";
}

Node::Node(Node* parent, std::string const& name, glm::mat4 const& local,
		glm::mat4 const& world) :
	
	m_parent{parent},
	m_children{},
	m_name{name},
	m_path{},
	m_depth{},
	m_localTransform{local},
	m_worldTransform{world}
{
	std::cout<<"Custom Constructor was called.\n";
	//set path
	if(m_parent!= nullptr) {
		m_path = m_parent -> getPath() + "/" + m_name;
	} else {
		m_path = m_name;
	}

	//set depth
	if(m_parent!= nullptr) {
		m_depth = m_parent -> getDepth()+1;	
	} else {
		m_depth = 0;
	}

	//add this node to parent's children
	m_parent -> addChildren(this);
}

Node::~Node() {
	std::cout << "~Node() is executed\n"; 
}

Node* Node::getParent() const {
	return m_parent;
}

void Node::setParent(Node* t_parent) {
	m_parent = t_parent;
	m_path = t_parent->getPath() + m_name;
	m_depth = t_parent->getDepth() + 1;
}

Node* Node::getChildren(std::string t_child) const {
	//search for t_child in list of children
	for (auto it = m_children.begin(); it != m_children.end(); it++) {
		
		if((*it) -> getName() == t_child) {
			return *it;
		}
	}
	
	//if t_child cannot be found
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
	t_child->setParent(this);
}

Node* Node::removeChildren(std::string t_child){ //find t_child in children list
  Node* tmp = getChildren(t_child);
  m_children.remove(getChildren(t_child));
  return tmp;
}