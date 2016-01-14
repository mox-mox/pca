#pragma once

#include <vector>

extern double delta_t;								// [s]

class N_body
{
	public:
		struct Particle
		{
			struct Vec2d
			{
				double x;
				double y;
				Vec2d(void);
				Vec2d(double x, double y);
				Vec2d operator-(Vec2d other);
				Vec2d operator+(Vec2d other);
				double operator^(int);
			};
			typedef Vec2d Position;																// [{m, m}]
			typedef Vec2d Force;																// [{N, N}]
			typedef Vec2d Acceleration;															// [{N/kg, N/kg}]
			typedef Vec2d Speed;																// [{m/s, m/s}]

			static constexpr double gamma = 6.673e-11;											// [Nm^2/kg^2]

			Position pos;																		// [{m, m}]
			Speed speed;																		// [{m/s, m/s}]
			double mass;																		// [kg]

			Particle(void);
			Force force(const Particle& other);													// [{N, N}]
			void react(const Force& force);

			private:
				Acceleration a(Force force);													// [{N/kg, N/kg}]
				void v(Acceleration acceleration);												// [{m/s, m/s}]
				void s(void);																	// [{m, m}]
		};
		std::vector < Particle > particles;
		std::vector < Particle::Force > forces;
		std::string filename;

	public:
		N_body(int32_t count, std::string filename = "output");
		void iterate(void);
		void simulate(int32_t steps);
		void save_state(void);
};





N_body::Particle::Vec2d& operator*(double alpha, N_body::Particle::Vec2d&vec);
N_body::Particle::Vec2d& operator/(N_body::Particle::Vec2d&vec, double alpha);




inline N_body::Particle::Acceleration N_body::Particle::a(Force force)
{
	return force/mass;
}
inline void N_body::Particle::v(Acceleration acceleration)
{
	speed = speed+delta_t*acceleration;
}
inline void N_body::Particle::s(void)
{
	pos = pos+delta_t*speed;
}
inline void N_body::Particle::react(const Force& force)
{
	v(a(force));
	s();
}
