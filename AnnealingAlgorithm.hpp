#ifndef CLEVER_ANNEALING_ALGORITHM_HPP
#define CLEVER_ANNEALING_ALGORITHM_HPP


#include <cmath>
#include <random>





namespace clever
{





/*
 * Solution:
 * 	Solution(Solution &sol);
 * 	any_type tweak();
 * 	number_type energy();
 * 	any_type operator=(Solution &sol);
 *
 */

constexpr double const START_TEMPERATURE = 500.0f;
constexpr double const FINISH_TEMPERATURE = 0.001f;
constexpr unsigned int const ITERATIONS = 100u;
constexpr double const ALPHA = 0.99f;

template<typename Solution>
void annealing(Solution &solution) {
	// objects
	std::default_random_engine dre( std::time(0) );
	std::uniform_real_distribution<double> dis(0.0f, 1.0f);

	Solution
		work(solution),
		cur(solution);

	// main loop
	for(
		double temp = START_TEMPERATURE;
		temp > FINISH_TEMPERATURE;
		temp *= ALPHA
	)
	{
		cur.tweak();

		if(cur.energy() <= work.energy())
		{
			work = cur;
			if(cur.energy() < solution.energy())
				solution = cur;
		}
		else
		{
			if(
				std::exp(
					-(cur.energy() - work.energy()) / temp
				) > dis(dre)
			)
				work = cur; 
			else
				cur = work; 
		}
	}

	return;
}





}





#endif
