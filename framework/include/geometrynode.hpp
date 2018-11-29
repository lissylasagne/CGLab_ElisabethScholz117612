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
			glm::mat4 const& t_local, glm::mat4 const& t_world,
			model const& t_model);

		// TODO
		// update c'tors here and in application_solar

		model getGeometry();
		void setGeometry(model t_geometry);
		void setDistance(float t_distance);
		float getDistance() const;
		void setSpeed(float t_speed);
		float getSpeed() const;
		void setColor(glm::fvec3 t_color);
		glm::fvec3 getColor() const;
		void setSize(float t_size);
		float getSize() const;

	private:
		model m_geometry;
		float m_distanceFromOrigin;
		float m_speed;
		glm::fvec3 m_color;
		float m_size;
		pixel_data m_normalmap;
};

#endif