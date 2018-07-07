#ifndef DESCARTES_VERTEX_HPP
#define DESCARTES_VERTEX_HPP

#include <string>

#include <SFML/Graphics.hpp>





struct DescartVertex
{
	float x, y;
	float radius;

	sf::Color color;
	std::string text;
};





#endif
