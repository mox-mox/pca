#pragma once

//#include <iostream>
#include <vector>
//#include "getopt_pp.hpp"
//#include <random>
//#include <functional>


class N_body
{
	public:
	struct Particle
	{
		struct Position
		{
			double x;									// [m]
			double y;									// [m]
			Position(void);
		};
		typedef Position Path;
		struct Speed
		{
			double x;									// [m/s]
			double y;									// [m/s]
			Speed(void);
		};
		typedef Speed Acceleration;
		static constexpr double gamma = 6.673e-11; 		// [Nm^2/kg^2]
		Position pos;
		Speed speed;
		double mass;									// [kg]
		Particle(void);
		double r(const Particle& other);				// [m]
		double f(const Particle& other);				// [N]
		const Acceleration& a(const Particle& other);	// [{m/s, m/s}]
		const Path& s(const Particle& other);

	};
	std::vector < Particle > particles;
	const double delta_t;								// [s]

	public:
	N_body(int32_t count, double delta_t=0.01):particles(count), delta_t(delta_t) {}
};

