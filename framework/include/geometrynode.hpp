#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"
#include "model.hpp"
#include "pixel_data.hpp"
#include "structs.hpp"

// use gl definitions from glbinding 
using namespace gl;

class GeometryNode : public Node{
	public:
		GeometryNode(Node* t_parent, std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model);

		GeometryNode(Node* t_parent,
			std::string const& t_name,
			glm::mat4 const& t_local,
			glm::mat4 const& t_world,
			float distance,
			float speed,
			float size,
			glm::fvec3 color
			);

		//TODO: ctor implementieren und im ctor die texturen laden und initialisieren (setTexture ist schon fertig)

		GeometryNode(std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model);
		
		void setDistance(float t_distance);
		float getDistance() const;
		
		void setSpeed(float t_speed);
		float getSpeed() const;
		
		void setColor(glm::fvec3 t_color);
		glm::fvec3 getColor() const;
		
		void setSize(float t_size);
		float getSize() const;

		void setTexture(std::string const& file_name);
		pixel_data getTexture() const;

		//TODO: getter setter für m_textureObject

	private:
		float m_distanceFromOrigin;
		float m_speed;
		float m_size;

		glm::fvec3 m_color;

		pixel_data m_texture;
		texture_object m_textureObject;
};

#endif