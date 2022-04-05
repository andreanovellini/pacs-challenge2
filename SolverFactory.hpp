#ifndef _SOLVER_FACTORY_HPP_
#define _SOLVER_FACTORY_HPP_

#include <memory>
#include <string>
#include <type_traits>
#include "classZeroFun.hpp"

class SolverFactory 
{
public:
	template<class SolverType, class ... Args>
	std::unique_ptr<SolverBase> make_solver(const Args&... args) const
	{
		return std::make_unique<SolverType>(args...);	
	}
};

#endif
