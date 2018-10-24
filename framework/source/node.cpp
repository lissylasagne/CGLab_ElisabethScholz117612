#include "node.hpp"

Node::Node(node* t_parent, std::string t_name) :
	m_parent{t_parent},
	m_children{},
	m_name{t_name},
	m_path{}, //TODO parent path + this node
	m_depth{}, //TODO parnet depth +1
	m_localTransform{},
	m_worldTransform{}
	{
		m_path = m_path.getPath() + m_name;
		m_depth = m_parent.getDepth()+1;
	//TODO world local mat4 zuweisen	
	}

Node* getParent() const {
	return m_parent;
}

void setParent(Node t_parent) {
	m_parent = t_parent;
}

Node* getChildren(std::string t_child) const {
	std::list<Node*>::iterator it;
	for(it = m_children.begin(); it != m_children.end(); ++it) {
		if (t_child == it->m_name) {
			return it;
		}
	}
	return nullptr;
}

std::list<Node*> getChildrenList() const;
std::string getName() const;
std::string getPath() const;
int getDepth() const;
glm::mat4 getLocalTransform() const;
void setLocalTransform(glm::mat4 t_local);
glm::mat4 getWorldTransform() const;
void setWorldTransform(glm::mat4 t_world);
void addChildren(Node* t_child);
Node*removeChildren(std::string t_child);