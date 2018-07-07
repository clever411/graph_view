#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>





template<typename Vertex>
struct Graph
{
	// types
	typedef Vertex vertex_type;
	typedef std::pair<
		vertex_type *, vertex_type *
	> edge_type;



	// data member
	std::vector<vertex_type> vers;
	std::vector<edge_type> edgs;

};





#endif
