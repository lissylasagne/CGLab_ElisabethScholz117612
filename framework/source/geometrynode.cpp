#include "geometrynode.hpp"

GeometryNode::GeometryNode(Node* t_parent, std::string const& t_name,
		glm::mat4 const& t_local, glm::mat4 const& t_world) :
	Node{t_parent, t_name, t_local, t_world}
{}

GeometryNode::GeometryNode(std::string const& t_name,
		glm::mat4 const& t_local, glm::mat4 const& t_world) :
	Node{t_name}
{
	setLocalTransform(t_local);
	setWorldTransform(t_world);
}

// big fat beautiful constructor
GeometryNode::GeometryNode(std::string const& t_name, glm::mat4 const& t_local,
			glm::mat4 const& t_world, float t_distance, float t_speed, float t_size,
			glm::fvec3 t_color, std::string const& file_name) :
	Node{t_name},
	m_distance{t_distance},
	m_speed{t_speed},
	m_size{t_size},
	m_color{t_color}
{
	setLocalTransform(t_local);
	setWorldTransform(t_world);
	setTexture(file_name);
	initTexture();
}

void GeometryNode::setDistance(float t_distance) {
	m_distance = t_distance;
}

float GeometryNode::getDistance() const {
	return m_distance;
}

void GeometryNode::setSpeed(float t_speed) {
	m_speed = t_speed;
}

float GeometryNode::getSpeed() const {
	return m_speed;
}

void GeometryNode::setColor(glm::fvec3 t_color) {
	m_color = t_color;
}

glm::fvec3 GeometryNode::getColor() const{
	return m_color;
}

void GeometryNode::setSize(float t_size){
	m_size = t_size;
}

float GeometryNode::getSize() const{
	return m_size;
}

void GeometryNode::setTexture(std::string const& file_name){
	m_texture = texture_loader::file(file_name);
}

pixel_data GeometryNode::getTexture() const{
	return m_texture;
}

texture_object GeometryNode::getTextureObject() const{
	return m_textureObject;
}

void GeometryNode::initTexture(){
	texture_object temp{};
	glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &temp.handle);
    glBindTexture(GL_TEXTURE_2D, temp.handle);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, m_texture.channels, m_texture.width, m_texture.height, 
			0, m_texture.channels, m_texture.channel_type, m_texture.ptr());

	std::cout << "texture_object: ";
	std::cout << temp.handle;
	std::cout << "\n";
	m_textureObject = temp;
}