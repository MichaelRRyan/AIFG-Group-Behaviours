#include "Boid.h"
#include "Flock.h"

using namespace std;

// =============================================== //
// ======== Flock Functions from Flock.h ========= //
// =============================================== //

Flock::~Flock()
{
	for (Boid * boid : m_boids)
		delete boid;

	m_boids.clear();
}

int Flock::getSize()
{
	return m_boids.size();
}

Boid Flock::getBoid(int i)
{
	return *m_boids[i];
}

void Flock::addBoid(sf::Vector2f const& t_position, bool t_predator)
{
	m_boids.push_back(new Boid(t_position.x, t_position.y, t_predator));
}

void Flock::addBoid(Boid * b)
{
	m_boids.push_back(b);
}

// Runs the run function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::flocking() 
{
	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->run(m_boids);
	}
}
// Runs the swarm function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::swarming()
{
	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->swarm(m_boids);
	}
}