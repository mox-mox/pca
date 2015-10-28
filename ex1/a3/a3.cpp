// Compile with g++ -Wall -Wextra -Wpedantic -std=c++14
#include <iostream>
#include <iomanip>
#include "utils.h"


/**
 * \brief Template function for integrating by a rieman summ
 *
 * This function will calculate a one-dimensional integral for a function F by calculating the riemann sum.
 *
 *           u_b           n                / n: Number of samples, should be adequatly high
 *            ∫ f(x) dx ≈  Σ F(xi)*Δx with {  Δx = (u_b - l_b)/n
 *           l_b          i=0               \ xi = l_b + Δx/2 + i*Δx
 *
 * \param T            The data type used for the calculation.
 * \prarm n            The number of sample points for the riemann sum.
 * \param F            The function that should be integrated.
 * \param lower_bound  The lower bound of the integral.
 * \param upper_bound  The upper bound of the integral.
 *
 * \return             The Integral of F from lower_bound to upper_bound
 */
template < uint64_t n=1024, typename T=double >
T integrate(T(*F)(T x), T lower_bound=0, T upper_bound=1)
{
	// Calculate some needed constants
	T interval = upper_bound - lower_bound;
	T delta_x = interval/static_cast < T > (n);
	T xi = lower_bound + (delta_x/2);

	// To the summation
	T result = 0;
	for(uint64_t i=0; i < n; i++, xi += delta_x)
	{
		result += (*F)(xi);
	}
	return result * delta_x;
}



int main(int argc, char** argv)
{
	argc=argc;
	argv=argv;


	uint64_t t0;
	uint64_t t1;
	uint64_t t_ges=0;

	double pi;
	constexpr uint32_t num_iterations = 10000;
	std::cout<<std::setprecision (15);

	for(uint32_t i=0; i<num_iterations; i++)
	{
		rdtsc(t0);
		//
		//    1      4
		//    ∫ ( --------- ) dx ≈ π
		//    0    1 + x^2
		pi = integrate<1000, double>( [](auto x){ return 4/(1+(x*x)); } );
		rdtsc(t1);
		t_ges += t1-t0;
	}
	std::cout<<"    1                    1000"<<std::endl;
	std::cout<<"π ≈ ∫ 4 / (1+x^2) dx  ≈   Σ   4 / (1+xi^2)*Δx = "<<pi<<" was calculated in "<<t_ges/num_iterations<<" clock cycles on average."<<std::endl;
	std::cout<<"    0                    i=0"<<std::endl;

	for(uint32_t i=0; i<num_iterations; i++)
	{
		rdtsc(t0);
		//
		//    1      4
		//    ∫ ( --------- ) dx ≈ π
		//    0    1 + x^2
		pi = integrate<10000, double>( [](auto x){ return 4/(1+(x*x)); } );
		rdtsc(t1);
		t_ges += t1-t0;
	}
	std::cout<<"    1                   10000"<<std::endl;
	std::cout<<"π ≈ ∫ 4 / (1+x^2) dx  ≈   Σ   4 / (1+xi^2)*Δx = "<<pi<<" was calculated in "<<t_ges/num_iterations<<" clock cycles on average."<<std::endl;
	std::cout<<"    0                    i=0"<<std::endl;

	for(uint32_t i=0; i<num_iterations; i++)
	{
		rdtsc(t0);
		//
		//    1      4
		//    ∫ ( --------- ) dx ≈ π
		//    0    1 + x^2
		pi = integrate<100000, double>( [](auto x){ return 4/(1+(x*x)); } );
		rdtsc(t1);
		t_ges += t1-t0;
	}
	std::cout<<"    1                  100000"<<std::endl;
	std::cout<<"π ≈ ∫ 4 / (1+x^2) dx  ≈   Σ   4 / (1+xi^2)*Δx = "<<pi<<" was calculated in "<<t_ges/num_iterations<<" clock cycles on average."<<std::endl;
	std::cout<<"    0                    i=0"<<std::endl;

}
