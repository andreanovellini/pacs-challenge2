#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include "GetPot"
#include "classZeroFun.hpp"
#include "SolverFactory.hpp"
using T = SolverTraits;

int main(int argc, char** argv)
{	
	std::cout << "======== Running the solver ========\n" << std::endl;
	
	// Function for which we want to calculate the zero
	auto fun = [](const T::Real& x) {return 0.5 - std::exp(M_PI * x);};
	// Derivative of the function (needed for Newton method)
	auto dfun = [](const T::Real& x) {return - M_PI * std::exp(M_PI * x);};

	// Read from command_line the datafile name and the method name	
	GetPot command_line(argc, argv);

	const std::string filename = command_line.follow("data.dat", 2, "-f", "--file" ); // file storing the parameters
	const std::string method = command_line("method", "Bisection");	// method name

	// Read parameters from datafile
	GetPot datafile(filename.c_str());
	
	const	std::string section = "Parameters/";
	
	const T::Real a = datafile((section + "a").data(), -1.0);										// Interval lower extreme
	const	T::Real b = datafile((section + "b").data(), 1.0);										// Interval upper extreme
	const T::Real tol = datafile((section + "tol").data(), 1e-4);								// Tolerance
	const T::Real tola = datafile((section + "tola").data(), 1e-10);						// Absolute tolerance
	const unsigned int maxIt = datafile((section + "maxIt").data(), 150);				// Max number of iteration for the methods
	const T::Real h_interval = datafile((section + "h_interval").data(), 0.01);	// Step for the bracket interval function
	const unsigned int maxIter = datafile((section + "maxIter").data(), 200);		// Max number of iteration for bracket interval function
	const T::Real x0 = datafile((section + "x0").data(), 0.0);									// Starting point for Newton-like methods
	const T::Real h = datafile((section + "h").data(), 1e-3);										// Step for derivative approximation in QuasiNewton method

	// Solver declaration
	SolverFactory solver;
	std::unique_ptr<SolverBase> solver_ptr = nullptr;

	if (method == "RegulaFalsi")
	{	
		// Regula Falsi method
		solver_ptr = solver.make_solver<RegulaFalsi>(fun, a, b, tol, tola, h_interval, maxIter);
	}
	else if (method == "Bisection")
	{
		// Bisection method
		solver_ptr = solver.make_solver<Bisection>(fun, a, b, tol, h_interval, maxIter);
	}
	else if (method == "Secant")
	{
		// Secant method 
		solver_ptr = solver.make_solver<Secant>(fun, a, b, tol, tola, maxIt, h_interval, maxIter);
	}
	else if (method == "Brent")
	{
		// Brent
		solver_ptr = solver.make_solver<Brent>(fun, a, b, tol, maxIt, h_interval, maxIter);
	}
	else if (method == "Newton")
	{
		// Newton method
		solver_ptr = solver.make_solver<Newton>(fun, dfun, x0, tol, tola, maxIt);
	}	
	else if (method == "QuasiNewton")
	{
		// QuasiNewton method
		solver_ptr = solver.make_solver<QuasiNewton>(fun, x0, h, tol, tola, maxIt);
	}
	else 
	{
		std::cout << "ERROR, invalid method" << std::endl;
		return 1;
	}
	
	// Solving for zero	
	const T::Real zero = solver_ptr -> solve();
	
	if (zero != std::numeric_limits<T::Real>::quiet_NaN())
	{
		std::cout << "Zero found with " << method << " method is: " << zero << std::endl;
	}
	else
	{
		std::cout << "Zero couldn't be found" << std::endl;
	}

	return 0;
}
