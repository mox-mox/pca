#include <iostream>
#include <vector>
#include "getopt_pp.hpp"
#include <random>
#include <functional>
#include "nbody.hpp"



std::default_random_engine rng;
std::uniform_real_distribution < double > x_distribution(0, 1);
std::uniform_real_distribution < double > y_distribution(0, 1);
std::uniform_real_distribution < double > m_distribution(0, 1);

N_body::Particle::Position::Position(void) : x(x_distribution(rng)), y(y_distribution(rng)) {}
N_body::Particle::Position::Position(double x, double y) : x(x), y(y) {}
N_body::Particle::Speed::Speed(void) : x(0), y(0) {}
N_body::Particle::Particle(void) : pos(), speed(), mass(m_distribution(rng)) {}

N_body::Particle::Path N_body::Particle::Position::operator-(const Position& other)
{
	Path r0(x-other.x, y-other.y);
	return r0;
}
N_body::Particle::Path& operator*(double alpha, N_body::Particle::Path& path)
{
	path.x*=alpha;
	path.y*=alpha;
	return path;
}
double N_body::Particle::Position::operator^(int two)
{
#ifdef DEBUG
	assert(two==2);
#endif
	(void) two;
	return x*x+y*y;
}

N_body::Particle::Path N_body::Particle::f(const Particle& other)
{
	Path r0 = pos-other.pos;
	return (-gamma*(mass*other.mass)/(r0^2))*r0;
}
//N_body::Particle::Path N_body::Particle::f(const Particle& other)
//{
//	Path r0 = pos-other.pos;
//	return (-gamma*(mass*other.mass)/(r0^2))*r0;
//}




int main(int argc, char** argv)
{
	//{{{    Argument handling

	rng.seed(std::random_device{}());
	int32_t count;
	double delta_t;
	int32_t steps;
	double x_max;
	double y_max;
	double m_max;
	GetOpt::GetOpt_pp ops(argc, argv);
	ops.exceptions(std::ios::failbit | std::ios::eofbit);
	try
	{
		ops >> GetOpt::Option('c', "count", count, 1000);
		ops >> GetOpt::Option('d', "delta_t", delta_t, 0.1);
		ops >> GetOpt::Option('s', "steps", steps, 100);

		ops >> GetOpt::Option('x', "x_max", x_max, 1000.0);
		ops >> GetOpt::Option('y', "y_max", y_max, 1000.0);
		ops >> GetOpt::Option('m', "m_max", m_max, 1000.0);
//#ifdef VECTORISE
//		ops >> GetOpt::Option('t', "threads", num_threads, 3);
//#endif
		x_distribution = std::uniform_real_distribution < double > (0.0, x_max);
		y_distribution = std::uniform_real_distribution < double > (0.0, y_max);
		m_distribution = std::uniform_real_distribution < double > (0.01, m_max);
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr << "Error in arguments" << std::endl;
		std::cerr << "Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr << "Options:"<<std::endl;
		std::cerr << "-s|--steps   <NUM>: Set the number of simulation steps."<<std::endl;
		std::cerr << "-r|--radius  <NUM>: Set the radius of the stimuli."<<std::endl;
		std::cerr << "-h|--heat    <NUM>: Set the heat of the stimuli."<<std::endl;
//#ifdef VECTORISE
//		std::cerr << "-t|--threads <NUM>:    Set the number of threads that are started for the computation of the multiplication."<<std::endl;
//#endif
		return -1;
	}
	//}}}
	std::cout<<"hello word!"<<std::endl;
	//std::cout<<x_distribution<<std::endl;
	N_body foo(count);
	for(int i=0; i < 5; i++)
		std::cout<<"( "<<foo.particles[i].pos.x<<" | "<<foo.particles[i].pos.y<<" )"<<std::endl;
	return 0;
}
