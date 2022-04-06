#include "classZeroFun.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

using T = SolverTraits;

// Bracket interval function implemetation
/*!
 * This function tries to find an interval that brackets the zero of a
 * function f. It does so by sampling the value of f at points
 * generated starting from a given point
 *
 * @tparam Function the type of the function. must be convertible to double
 * (*)(double)
 * @param f The function.
 * @param x1 initial point
 * @param h initial increment for the sampling
 * @param maxIter maximum number of iterations
 * @return a tuple with the bracketing points and a bool which is true if number
 * of iterations not exceeded (bracket found)
 * @note I am adopting a simple algorithm. The search can be bettered using a
 * quadratic (or cubic) search.
 */
std::tuple<T::Real, T::Real, bool> SolverWithInterval::bracketInterval(const T::FunType &f, T::Real x1, T::Real h, unsigned int maxIter)
{
	constexpr T::Real expandFactor = 1.5;
	auto direction = 1.0;
	auto	x2 = x1 + h;
	auto y1 = f(x1);
	auto y2 = f(x2); 
	unsigned int iter{0u};

	// Get initial decrement direction
	while ((y1 * y2 > 0) && (iter < maxIter))
	{
		++iter;
		if (std::abs(y2) > std::abs(y1))
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		direction = (x2 > x1) ? 1.0 : -1.0;
		x1 = x2;
		y1 = y2;
		x2 += direction * h;
		y2 = f(x2);
		h *= expandFactor;
	}
	// swap to get elements in the correct order even when negative
	if (x1 > x2)
		std::swap(x1, x2);
	
	if (iter < maxIter)
		std::cout << "Bracket interval found: [" << x1 << ", " << x2 << "]" << std::endl;

	return std::make_tuple(x1, x2, iter < maxIter);
}

// Regula Falsi 
/*!
 * Compute the zero of a scalar function with the method of regula falsi
 *
 * @return The approximation of the zero of f 
 */
T::Real RegulaFalsi::solve()
{
	T::Real ya = f(a);
	T::Real yb = f(b);
	T::Real delta = b - a;
	T::Real resid0 = std::max(std::abs(ya), std::abs(yb));
  if (ya * yb > 0)
	{
		std::cout << "ERROR, function must change sign at the two end values" << std::endl;

		return std::numeric_limits<T::Real>::quiet_NaN();
	}

	T::Real yc{ya};
	T::Real c{a};
	T::Real incr = std::numeric_limits<T::Real>::max();
  constexpr T::Real small = 10.0 * std::numeric_limits<T::Real>::epsilon();
  while(std::abs(yc) > tol * resid0 + tola && incr > small)
    {
			T::Real incra = -ya / (yb - ya);
			T::Real incrb = 1. - incra;
			T::Real incr = std::min(incra, incrb);
      if (!(std::max(incra, incrb) <= 1.0 && incr >= 0))
					{
						std::cout << "Chord is failing" << std::endl;

						return std::numeric_limits<T::Real>::quiet_NaN();
					}
			
      c = a + incra * delta;
      // std::cout << c << std::endl;
      yc = f(c);
      if(yc * ya < 0.0)
        {
          yb = yc;
          b = c;
        }
      else
        {
          ya = yc;
          a = c;
        }
      delta = b - a;
    }
  return c;
}

// Bisection implemetation
/*!
 * Compute the zero of a scalar function with the method of bisection
 * The returned value is far from the zero at most given tolerance;
 *
 * @return The approximation of the zero of f
 *
 */
T::Real Bisection::solve()
{
	T::Real ya = f(a);
	T::Real yb = f(b);
	T::Real delta = b - a;

	if (ya * yb > 0)
	{
		std::cout << "ERROR, function must change sign at the two end values" << std::endl;

		return std::numeric_limits<T::Real>::quiet_NaN();
	}

	T::Real yc{ya};
	T::Real c{a}; 
	while (std::abs(delta) > 2 * tol)
	{
		c = (a + b) / 2.;
		yc = f(c); 
		
		if (yc * ya < 0.0)
		{
			yb = yc; 
			b = c;
		}
		else
		{
			ya = yc;
			a = c;
		}
		delta = b - a;
	}
	return (a + b) / 2.;
}

// Secant implemetation
/*!
 * Computes the zero of a scalar function with the method of the secant
 * 
 * @return The approximation of the zero of f
 *
 */
T::Real Secant::solve()
{
	T::Real ya = f(a);
	T::Real resid = std::abs(ya); 
	T::Real c{a}; 
	unsigned int iter{0u};
	T::Real check = tol * resid + tola;
	bool goOn = resid > check;

	while (goOn && iter < maxIt)
	{
		++iter; 
		T::Real yb = f(b);
		c = a - ya * (b - a) / (yb - ya);
		T::Real yc = f(c);
		resid = std::abs(yc); 
		goOn = resid > check;
		ya = yc; 
		a = c;
	}

	if (iter < maxIt)
		return c; 
	else 
	{
		std::cout << "ERROR, could not find the zero" << std::endl;
 
		return std::numeric_limits<T::Real>::quiet_NaN();
	}
}

// Brent implemetation
T::Real Brent::solve() 
{
	auto ya = f(a);
  auto yb = f(b);

  // First check.
  if((ya * yb) >= 0.0)
    {
      if(ya == 0.)
        return a;
      else if(yb == 0.)
        return b;
      else
			{
				std::cout << "ERROR, could not find the zero" << std::endl;
				 
				return std::numeric_limits<T::Real>::quiet_NaN(); // precondition not met
			}	 
    };
  //
  if(std::abs(ya) < std::abs(yb))
    {
      std::swap(a, b);
      std::swap(ya, yb);
    }
  //
  auto     c = a;
  auto     d = c;
  auto     yc = ya;
  bool     mflag{true};
  auto     s = b;
  auto     ys = yb;
  unsigned iter{0u};
  do
    {
			++iter;
      //
      if(ya != yc and yb != yc)
        {
          auto yab = ya - yb;
          auto yac = ya - yc;
          auto ycb = yc - yb;
          // inverse quadratic interpolation
          s = a * ya * yc / (yab * yac) + b * ya * yc / (yab * ycb) -
              c * ya * yb / (yac * ycb);
        }
      else
        {
          // secant
          s = b - yb * (b - a) / (yb - ya);
        }
      //
      if(((s - 3 * (a + b) / 4) * (s - b) >= 0) or // condition 1
         (mflag and
          (std::abs(s - b) >= 0.5 * std::abs(b - c))) or // condition 2
         (!mflag and
          (std::abs(s - b) >= 0.5 * std::abs(c - d))) or // condition 3
         (mflag and (std::abs(b - c) < tol)) or          // condition 4
         (!mflag and (std::abs(c - d) < tol))            // condition 5
				 )
        {
          mflag = true;
          s = 0.5 * (a + b); // back to bisection step
        }
      else
        mflag = false;
      //
      ys = f(s);
      d = c;
      c = b;
      yc = yb;
      //
      if(ya * ys < 0)
        {
          b = s;
          yb = ys;
        }
      else
        {
          a = s;
          ya = ys;
        }
      //
      if(std::abs(ya) < std::abs(yb))
        {
          std::swap(a, b);
          std::swap(ya, yb);
        }
		}
  while(ys != 0. && std::abs(b - a) > tol && iter < maxIt);
	if (iter < maxIt)
		return s;
	else {
				std::cout << "ERROR, could not find the zero " << std::endl;
				 
				return std::numeric_limits<T::Real>::quiet_NaN();
	}
}

// Newton implemetation
/*!
 * Computes the zero of a scalar function with the method of Newton
 *
 * @return The approximation of the zero of f
 */
T::Real Newton::solve()
{
	T::Real y0 = f(x0);
	T::Real resid = std::abs(y0);
	unsigned int iter{0u};
	T::Real check = tol * resid + tola;
	bool goOn = resid > check;
	while(goOn && iter < maxIt)
	{
		++iter;
		x0 += -y0/df(x0);
		y0 = f(x0);
		resid = std::abs(y0);
		goOn = resid > check;
	}

	if (iter < maxIt)
		return x0;
	else
	{
		std::cout << "ERROR, could not find the zero" << std::endl;
 
		 return std::numeric_limits<T::Real>::quiet_NaN();
	}
}
