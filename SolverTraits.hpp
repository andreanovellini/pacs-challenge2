#ifndef _SOLVER_TRAITS_HPP_
#define _SOLVER_TRAITS_HPP_

#include <iostream>
#include <functional>
#include <limits>
#include <cmath>

class SolverTraits
{
public:
	// Result type
	using Real = double;
	// Function type
	using FunType =	std::function<Real(const Real&)>;

};

#endif
