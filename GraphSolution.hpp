#ifndef GRAPH_SOLUTION_HPP
#define GRAPH_SOLUTION_HPP

#include "Graph.hpp"
#include "DescartVertex.hpp"





struct Box
{
	float left;
	float top;
	float width;
	float height;
};





class GraphSolution
{
public:
	typedef Graph< DescartVertex > graph_type;


	// constructor & operator=
	GraphSolution(graph_type *graph = nullptr);

	GraphSolution(GraphSolution const &graphsol);
	GraphSolution &operator=(GraphSolution const &graphsol);

	~GraphSolution();





	// core methods
	GraphSolution &adjust();

	GraphSolution &tweak();
	double energy();



	// graph
	GraphSolution &setGraph(graph_type *newgraph);
	GraphSolution &releaseGraph();
	graph_type *getGraph() const;



	// box
	GraphSolution &setBox(Box const &newbox);
	Box const &getBox() const;



private:
	void calculate_energy_();


	// data members
	graph_type *graph_;
	double energy_;

	Box box_ = {0.0f, 0.0f, 1000.0f, 1000.0f};



};





#endif
