#ifndef A1_HPP
#define A1_HPP

#include <cstdint>
#include <array>




//template < int64_t N=1024 > class Relaxation;
//
//template < int64_t N=1024 >
//typedef  int (Relaxation<N>::*grid_point_value_function)(double i, double j);

template < int64_t N=1024 >
class Relaxation
{
	private:
		const double max_theta;
		const double phi;
		const int64_t I;
		const int64_t J;
		const int64_t R;
		const int64_t H;
		//double grid[N][N];
		std::array < std::array < double, N >, N > *grid;
	public:
		Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error);
		~Relaxation();


		void fill_grid(double (*grid_point_value_function)(double i, double j), std::array < std::array < double, N >, N >& grid);

#ifdef DEBUG
		double new_value(int64_t i, int64_t j) throw(std::logic_error);
#else
		double new_value(int64_t i, int64_t j);
#endif

		double init_value(int64_t i, int64_t j);




		void iterate();

		void print_grid();
};





#endif /* A1_HPP */