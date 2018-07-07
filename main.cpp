#include <algorithm>
#include <iostream>
#include <random>
#include <string>

#include <SFML/Graphics.hpp>

#include "AnnealingAlgorithm.hpp"
#include "DescartVertex.hpp"
#include "Graph.hpp"
#include "GraphSolution.hpp"
#include "Line.hpp"



using namespace clever;
using namespace sf;
using namespace std;





// global objects
	// sfml objects
RenderWindow window;
VideoMode vmode = VideoMode::getDesktopMode();
char const *TITLE = "Application";
unsigned int FRAMERATE_LIMIT = 120u;

Event event;

	// graph&random
typedef Graph< DescartVertex > graph_type;
graph_type *graph = new graph_type;
default_random_engine global_dre( time(0) );





// init
void init_window()
{
	ContextSettings sets;
	sets.antialiasingLevel = 4;
	window.create(vmode, TITLE, Style::None, sets);
	window.setPosition({0u, 0u});
	window.setFramerateLimit(FRAMERATE_LIMIT);
	return;
}

void init_graph(graph_type &graph)
{
	graph.edgs.clear();
	graph.vers.clear();

	// constants
	constexpr static unsigned int const
		VERTEX_COUNT = 10u,
		EDGE_COUNT = 5u;
	constexpr static float const
		MIN_RADIUS = 30.0f,
		MAX_RADIUS = 100.0f;



	// create vertex
	DescartVertex ver;
	ver.color = Color::Black;

	uniform_real_distribution<float> xdis(
		MAX_RADIUS,
		window.getSize().x-MAX_RADIUS
	);
	uniform_real_distribution<float> ydis(
		MAX_RADIUS,
		window.getSize().y-MAX_RADIUS
	);
	uniform_real_distribution<float> rdis(
		MIN_RADIUS,
		MAX_RADIUS
	);

	for(size_t i = 0; i < VERTEX_COUNT; ++i)
	{
		ver.x = xdis(global_dre);
		ver.y = ydis(global_dre);
		ver.radius = rdis(global_dre);
		ver.text = to_string(i);
		graph.vers.push_back(ver);
	}

	// create edges
		// check correct
	static_assert(
		EDGE_COUNT <
		VERTEX_COUNT * (VERTEX_COUNT - 1) / 2
	);

	uniform_int_distribution<int> choice(0, VERTEX_COUNT-1);
	typename graph_type::edge_type edge;

	for(size_t i = 0; i < EDGE_COUNT; ++i)
	{
		do
		{
			edge.first = &graph.vers[ choice(global_dre) ];
			edge.second = &graph.vers[ choice(global_dre) ];
		}
		while(
			edge.first == edge.second ||
			any_of(
				graph.edgs.cbegin(), graph.edgs.cend(),
				[ &edge ]( typename graph_type::edge_type const &e )->bool {
					return
						(edge.first == e.first && edge.second == e.second) ||
						(edge.first == e.second && edge.second == e.first);
				}
			)
		);

		graph.edgs.push_back(edge);
	}

	return;
}



// functions
void draw_graph(
	graph_type const &graph,
	RenderTarget &target,
	RenderStates const &states = RenderStates::Default
)
{
	// edges
	Line line;
	line.setColor(Color::Cyan);
	line.setThickness(5.0f);
	for(auto b = graph.edgs.cbegin(), e = graph.edgs.cend(); b != e; ++b)
	{
		line.setPosition(
			{ b->first->x, b->first->y },
			{ b->second->x, b->second->y }
		);
		target.draw(line, states);
	}


	// vertex
	sf::CircleShape circle;
	for(auto b = graph.vers.cbegin(), e = graph.vers.cend(); b != e; ++b)
	{
		circle.setRadius(b->radius);
		circle.setOrigin( b->radius, b->radius );
		circle.setPosition(b->x, b->y);
		circle.setFillColor(b->color);
		target.draw(circle, states);
	}

	return;
}





// main
int main( int argc, char *argv[] )
{
	init_window();
	init_graph(*graph);

	GraphSolution sol(graph);
	sol.setBox({
		0.0f, 0.0f,
		(float)window.getSize().x,
		(float)window.getSize().y
	});

	

	while(window.isOpen())
	{
		if(window.pollEvent(event))
		{
			switch(event.type)
			{
			case Event::KeyPressed:

				switch(event.key.code)
				{
				case Keyboard::C:
					window.close();
					break;
				case Keyboard::R:
					std::cout << "randomize" << std::endl;
					init_graph(*sol.getGraph());
					sol.adjust();
					std::cout << "energy after randomize: " << sol.energy() << std::endl;
					break;
				case Keyboard::T:
					std::cout << "tweak" << std::endl;
					sol.tweak();
					std::cout << "energy after tweak: " << sol.energy() << std::endl;
					break;
				case Keyboard::A:
					std::cout << "annealing..." << std::endl;
					annealing( sol );
					std::cout << "energy after annealing: " << sol.energy() << std::endl;
					break;
				case Keyboard::E:
					std::cout << "energy: " << sol.energy() << std::endl;
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}
		}


		window.clear(Color::White);
		draw_graph(*graph, window);
		window.display();
	}

	return 0;
}





// end
