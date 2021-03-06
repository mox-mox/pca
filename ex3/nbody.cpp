#include <iostream>
#include <iomanip>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include "getopt_pp.hpp"
#include <random>
#include <functional>
#include "nbody.hpp"
#include <cfloat>

double delta_t;	// [s]


//{{{ Randomness

std::default_random_engine rng;
std::uniform_real_distribution < double > x_distribution(0, 1);
std::uniform_real_distribution < double > y_distribution(0, 1);
std::uniform_real_distribution < double > m_distribution(0, 1);
//}}}

//{{{ Constructors

N_body::Particle::Vec2d::Vec2d(void) : x(x_distribution(rng)), y(y_distribution(rng)) {}
N_body::Particle::Vec2d::Vec2d(double x, double y) : x(x), y(y) {}
N_body::Particle::Vec2d::Vec2d(const Vec2d& other) : x(other.x), y(other.y) {}
N_body::Particle::Particle(void) : pos(), speed(0, 0), mass(m_distribution(rng)) {}

N_body::N_body(int32_t count, int32_t mode) : particles(count), forces(count, Particle::Force(0, 0)), mode(mode)
{
	if( mode )
	{
		path = "./data/";

		struct stat info;

		if( stat(path.c_str(), &info))
		{
			std::cout<<"Creating folder \""<<path<<"\"."<<std::endl;
			int temp = system(("mkdir "+path).c_str());
			(void) temp;
		}
	}
}


//{{{ Operators

N_body::Particle::Vec2d N_body::Particle::Vec2d::operator-(Vec2d other)
{
	other.x = x-other.x;
	other.y = y-other.y;
	return other;
}
N_body::Particle::Vec2d N_body::Particle::Vec2d::operator+(Vec2d other)
{
	other.x += x;
	other.y += y;
	return other;
}
N_body::Particle::Vec2d operator*(double alpha, N_body::Particle::Vec2d vec)
{
	vec.x *= alpha;
	vec.y *= alpha;
	return vec;
}
N_body::Particle::Vec2d operator/(N_body::Particle::Vec2d vec, double alpha)
{
	vec.x /= alpha;
	vec.y /= alpha;
	return vec;
}
N_body::Particle::Vec2d N_body::Particle::Vec2d::operator=(const N_body::Particle::Vec2d&vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}
double N_body::Particle::Vec2d::operator^(int two)
{
#ifdef DEBUG
		assert(two == 2);
#endif
	(void) two;
	return x*x+y*y;
}
//}}}

N_body::Particle::Force N_body::Particle::force(const Particle& other)
{
	Vec2d r0 = pos-other.pos;

	return (-gamma*(mass*other.mass)/(r0^2))*r0;
}

void N_body::save_state(int32_t step)
{
	std::ofstream data_output;
	std::ostringstream out;

	out<<std::setfill('0')<<std::setw(max_step_length)<<step;
	data_output.open(path+"step_"+out.str()+".dat");
	data_output<<"#    x_pos\t"<<"     y_pos\t"<<"     mass"<<std::endl;

	for( auto& particle : particles )
	{
		data_output<<std::setfill(' ')<<std::setw(10)<<particle.pos.x<<"\t"<<std::setfill(' ')<<std::setw(10)<<particle.pos.y<<"\t"<<std::setfill(' ')<<std::setw(10)<<particle.mass<<std::endl;
	}
	data_output.close();
}


void N_body::iterate(void)
{
	//{{{ Generate the forces...

	Particle::Force force(0, 0);

	forces[0] = Particle::Force(0, 0);

	for( uint32_t j = 1; j < particles.size(); j++ )	// Partial loop unrolling to avoid having to do zero initialisation
	{
		force = particles[0].force(particles[j]);
		forces[0] = forces[0]+force;
		forces[j] = forces[j]-force;
	}
	for( uint32_t i = 0; i < particles.size()-1; i++ )
	{
		for( uint32_t j = i+1; j < particles.size(); j++ )
		{
			force = particles[i].force(particles[j]);
			forces[i] = forces[i]+force;
			forces[j] = forces[j]-force;
		}
	}
	//}}}

	//{{{ ...and let the particles fly.

	std::cout<<"pos:  ( "<<particles[0].pos.x<<" | "<<particles[0].pos.y<<" )"<<std::endl;
	for( uint32_t i = 0; i < particles.size(); i++ )
	{
		//std::cout<<"Old pos:  ( "<<particles[i].pos.x<<" | "<<particles[i].pos.y<<" )"<<"Old speed:  ( "<<particles[i].speed.x<<" | "<<particles[i].speed.y<<" )"<<std::endl;
		particles[i].react(forces[i]);
		//std::cout<<"New pos:  ( "<<particles[i].pos.x<<" | "<<particles[i].pos.y<<" )"<<"Old speed:  ( "<<particles[i].speed.x<<" | "<<particles[i].speed.y<<" )"<<std::endl<<std::endl;
	}
	//}}}
}

void N_body::simulate(int32_t steps)
{
	max_step_length = std::to_string(steps).length();
	for( int32_t i = 0; i < steps; i++ )
	{
		iterate();
		if( mode ) save_state(i);
	}
}




















int main(int argc, char** argv)
{
	//{{{    Argument handling

	rng.seed(std::random_device {} ());
	int32_t count;
	int32_t steps;
	double x_max;
	double y_max;
	double m_max;
	bool output;
	GetOpt::GetOpt_pp ops(argc, argv);
	ops.exceptions(std::ios::failbit|std::ios::eofbit);
	try
	{
		ops>>GetOpt::Option('c', "count", count, 10);
		ops>>GetOpt::Option('d', "delta_t", delta_t, 1.0);
		ops>>GetOpt::Option('s', "steps", steps, 10);
		ops>>GetOpt::OptionPresent('o', "output", output);

		ops>>GetOpt::Option('x', "x_max", x_max, 1000.0);
		ops>>GetOpt::Option('y', "y_max", y_max, 1000.0);
		//ops>>GetOpt::Option('m', "m_max", m_max,  1000000000000000.0 );
		ops>>GetOpt::Option('m', "m_max", m_max,  static_cast<double>(FLT_MAX) );
		//ops>>GetOpt::Option('m', "m_max", m_max,  DBL_MAX/1000000 );
//#ifdef VECTORISE
//		ops >> GetOpt::Option('t', "threads", num_threads, 3);
//#endif
		x_distribution = std::uniform_real_distribution < double > (0.0, x_max);
		y_distribution = std::uniform_real_distribution < double > (0.0, y_max);
		m_distribution = std::uniform_real_distribution < double > (0.01, m_max);
	}
	catch(GetOpt::GetOptEx ex)
	{
		std::cerr<<"Error in arguments"<<std::endl;
		std::cerr<<"Usage: "<<argv[0]<<" [options]"<<std::endl;
		std::cerr<<"Options:"<<std::endl;
		std::cerr<<"-s|--steps   <NUM>: Set the number of simulation steps."<<std::endl;
		std::cerr<<"-r|--radius  <NUM>: Set the radius of the stimuli."<<std::endl;
		std::cerr<<"-h|--heat    <NUM>: Set the heat of the stimuli."<<std::endl;

//#ifdef VECTORISE
//		std::cerr << "-t|--threads <NUM>:    Set the number of threads that are started for the computation of the multiplication."<<std::endl;
//#endif
		return -1;
	}
	//}}}

	std::cout<<"hello word!"<<std::endl;

	if( output )
	{
		N_body experiment(count, 1);
		experiment.simulate(steps);
	}






	//N_body foo(count);
	//for( int i = 0; i < 5; i++ )
	//	std::cout<<"( "<<foo.particles[i].pos.x<<" | "<<foo.particles[i].pos.y<<" )"<<std::endl;
	return 0;
}
