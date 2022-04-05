#include <cmath>
#include <iostream>
#include <ostream>

#include "classZeroFun.hpp"

using T = SolverTraits;
/*
T::Real fun(const T::Real& x)
{
	return 0.5 - std::exp(M_PI * x);
}
*/

/*
T::Real fun(const T::Real& x) 
{
	return x - 10;
}
*/

int main()
{	
	std::cout << "=== Running the solver ===\n" << std::endl;
	
	// Function
	auto fun = [](const T::Real& x) {return 0.5 - std::exp(M_PI * x);};
	auto dfun = [](const T::Real& x) {return - M_PI * std::exp(M_PI * x);};

	// Regula Falsi method
	RegulaFalsi rf1(fun, -1, 1, 1e-4, 1e-10);
	std::cout << "Zero found with RegulaFalsi method: " << rf1.solve() << "\n" << std::endl;
	// RegulaFalsi method with guessed interval
	RegulaFalsi rf2(fun, 1e-4, 1e-10, 0, 0.01, 200);
	std::cout << "Zero found with RegulaFalsi method and guessed interval: " << rf2.solve() << "\n" << std::endl;

	// Bisection method
	Bisection b1(fun, -1, 1, 1e-4);
	std::cout << "Zero found with Bisection method: " << b1.solve() << "\n" << std::endl;
	// Bisection method with guessed interval
	Bisection b2(fun, 1e-4, 0, 0.01, 200);
	std::cout << "Zero found with Bisection method and guessed interval: " << b2.solve() << "\n" << std::endl;

	// Secant method
	Secant s1(fun, -1, 1, 1e-4, 1.e-10, 150);
	std::cout << "Zero found with Secant method: " << s1.solve() << "\n" << std::endl;
	// Secant method with guessed interval
	Secant s2(fun, 1e-4, 1e-10, 150, 0, 0.01, 200);
	std::cout << "Zero found with Secant method and guessed interval: " << s2.solve() << "\n" << std::endl;	
	
	// Newton method
	Newton n1(fun, dfun, 0, 1e-4, 1e-10, 150);
	std::cout << "Zero found with Newton method: " << n1.solve() << "\n" << std::endl;

	// QuasiNewton method
	QuasiNewton n2(fun, 0, 0.01, 1e-4, 1e-10, 150);
	std::cout << "Zero found with QuasiNewton method: " << n2.solve() << "\n" << std::endl;

	// Brent method
	Brent br1(fun, -1, 1, 1e-4, 150);
	std::cout << "Zero found with Brent method: " << b1.solve() << "\n" << std::endl;
	// Brent method with guessed interval
	Brent br2(fun, 1e-4, 150, 0, 0.01, 200);
	std::cout << "Zero found with Brent method: " << b2.solve() << "\n" << std::endl;

	return 0;
}
