#ifndef _SOLVER_FACTORY_HPP_
#define _SOLVER_FACTORY_HPP_

#include <memory>
#include <string>

#include "SolverBase.hpp"

class SovlerFactory 
{
	public:
		std::unique_ptr<SolverBase> operator()(const std::string& solver_name);

};

#endif
