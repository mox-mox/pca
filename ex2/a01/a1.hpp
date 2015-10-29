#ifndef A1_HPP
#define A1_HPP

#include <cstdint>
#include <array>
#include <fstream>



#define OPTIMISE CACHE
//#define OPTIMISE RUNTIME

#if !defined OPTIMISE || (OPTIMISE != CACHE) && (OPTIMISE != RUNTIME)
	#error "Please define OPTIMISE to CACHE or RUNTIME."
#endif

template < int64_t N >
class Relaxation
{
	private:
		const double max_theta;
		const double phi;
		const int64_t I;
		const int64_t J;
		const int64_t R;
		const int64_t H;
		std::array < std::array < double, N >, N > grid;
	public:
		Relaxation(double max_theta, double phi, int64_t I, int64_t J, double R, double H) throw(std::logic_error);
		Relaxation(double R, double H) throw(std::logic_error);


		void fill_grid(double(Relaxation < N > ::*grid_point_value_function)(int64_t i, int64_t j), std::array < std::array < double, N >, N >& grid);


#ifdef DEBUG
		double new_value(int64_t i, int64_t j) throw(std::logic_error);
#else
		double new_value(int64_t i, int64_t j);
#endif

		double init_value(int64_t i, int64_t j);

		void iterate();

		void print_grid();

		void export_data(std::ofstream& myfile);
};
#endif /* A1_HPP */
