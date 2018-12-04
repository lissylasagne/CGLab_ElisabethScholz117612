#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <glbinding/gl/types.h>
#include "node.hpp"
#include "model.hpp"
#include "pixel_data.hpp"

// use gl definitions from glbinding 
using namespace gl;

class GeometryNode : public Node{
	public:
		GeometryNode(Node* t_parent, std::string const& t_name,
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model);

		GeometryNode(std::string const& t_name,
			glm::mat4 const& t_local,
			glm::mat4 const& t_world,
			float t_distance,
			float t_speed,
			float t_size,
			glm::fvec3 t_color,
			std::string const& file_name
			);

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

		void initTexture();
		//TODO: getter setter für m_textureObject

		/*
		void setNormalMap(std::string const& file_name);
		pixel_data getNormalMap() const;
		*/

	private:
		float m_distance;
		float m_speed;
		float m_size;

		glm::fvec3 m_color;

		pixel_data m_texture;
		texture_object m_textureObject;

		/*
		pixel_data m_normalMap;
		texture_object m_normalMapObject;
		*/
};

#endif