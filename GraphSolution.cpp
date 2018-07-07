#include "GraphSolution.hpp"

#include <algorithm>
#include <iterator>
#include <random>





// random engine for tweak
std::default_random_engine dre( time(0) );





// creation, copy destroy
GraphSolution::GraphSolution(graph_type *graph):
	graph_(graph), energy_(0.0f)
{
	calculate_energy_();
	return;
}

GraphSolution::GraphSolution(GraphSolution const &graphsol):
	graph_(nullptr), energy_(0.0f)
{
	operator=(graphsol);
	return;
}

GraphSolution &GraphSolution::operator=(GraphSolution const &graphsol)
{
	box_ = graphsol.box_;

	// clear
	if(graph_)
		delete graph_;

	// check
	if( !graphsol.graph_ )
	{
		graph_ = nullptr;
		energy_ = 0.0f;
		return *this;
	}

	// preparation
	auto const &vers = graphsol.graph_->vers;
	auto const &edgs = graphsol.graph_->edgs;

	graph_ = new graph_type;

	// vetrexes
	graph_->vers.resize(vers.size());
	std::copy(
		vers.cbegin(),
		vers.cend(),
		graph_->vers.begin()
	);

	// edges
	graph_->edgs.reserve( edgs.size() );
	for( auto b = edgs.cbegin(), e = edgs.cend(); b != e; ++b)
	{
		graph_->edgs.push_back(
			{ 
				&graph_->vers[ b->first - vers.data() ],
				&graph_->vers[ b->second - vers.data() ]
			}
		);
	}

	calculate_energy_();

	return *this;
}


GraphSolution::~GraphSolution()
{
	if(graph_)
		delete graph_;
	return;
}





// core methods
GraphSolution &GraphSolution::tweak()
{
	if(!graph_ || graph_->vers.empty())
	{
		energy_ = 0.0f;
		return *this;
	}

	std::uniform_int_distribution<int> idis( 0, graph_->vers.size()-1 );

	// choose ver
	auto &ver = graph_->vers[ idis(dre) ];

	// choose direction

	idis.param( 
		std::uniform_int_distribution<int>::param_type(0, 3)
	);

	float offsetx = 10.0f;
	float offsety = 10.0f;

	int dir = idis(dre);

	switch(dir) {
	case 0:
		offsety = -offsety;
		break;
	case 1:
		break;
	case 2:
		offsetx = -offsetx;
		break;
	case 3:
		offsety = -offsety;
		offsetx = -offsetx;
		break;
	default:
		throw std::logic_error("invalid dir");
		break;
	}

	
	ver.x += offsetx;
	ver.y += offsety;

	calculate_energy_();
	return *this;
}

double GraphSolution::energy()
{
	return energy_;
}

GraphSolution &GraphSolution::adjust()
{
	calculate_energy_();
	return *this;
}



// graph
GraphSolution &GraphSolution::setGraph(graph_type *newgraph)
{
	if(graph_)
		delete graph_;

	graph_ = newgraph;

	calculate_energy_();
	return *this;
}

GraphSolution &GraphSolution::releaseGraph()
{
	graph_ = nullptr;
	energy_ = 0.0f;
	return *this;
}

GraphSolution::graph_type *GraphSolution::getGraph() const
{
	return graph_;
}



// box
GraphSolution &GraphSolution::setBox(Box const &newbox)
{
	box_ = newbox;
	calculate_energy_();
	return *this;
}
Box const &GraphSolution::getBox() const
{
	return box_;
}





// other
void GraphSolution::calculate_energy_()
{
	energy_ = 0.0f;
	if(!graph_)
		return;

	double distance;
	double intersection;

	for(auto b = graph_->vers.cbegin(), e = graph_->vers.cend(); b != e; ++b)
	{
		for(auto b2 = std::next(b); b2 != e; ++b2)
		{
			distance = std::sqrt(
				std::pow( b->x - b2->x, 2) + 
				std::pow( b->y - b2->y, 2)
			);

			intersection = (b->radius + b2->radius) - distance;

			if(intersection > 0.0f)
				energy_ += std::pow(intersection, 2);
			if(distance < 400.0f)
				energy_ += 4000.0f/distance;
			else if(distance > 1000.0f)
				energy_ += std::sqrt(distance);
		}

		distance = ( b->x - b->radius ) - box_.left;
		if(distance < 0)
			energy_ += std::pow(distance, 2);
		else 
		{
			distance = ( box_.left + box_.width ) - ( b->x + b->radius );
			if(distance < 0)
				energy_ += std::pow(distance, 2);
		}

		distance = ( b->y - b->radius ) - box_.top;
		if(distance < 0)
			energy_ += std::pow(distance, 2);
		else
		{
			distance = ( box_.top + box_.height ) - ( b->y + b->radius );
			if(distance < 0)
				energy_ += std::pow(distance, 2);
		}

	}

	energy_ *= 0.01;

	return;
}





// end
