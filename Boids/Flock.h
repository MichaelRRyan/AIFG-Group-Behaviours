#ifndef FLOCK_H_
#define FLOCK_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Boid.h"

using namespace std;

/*
Brief description of Flock Class:
// This file contains the class needed to create flock of boids. It utilizes
// the boids class and initializes boid flocks with parameters that can be
// specified. This class will be utilized in main.
*/

class Flock
{
private:

public:
	//Public Variables
	vector<Boid *> m_boids;

	//Constructors
	Flock() {}
	~Flock();
/*
Commented out for now, since including destructors causes g++ to spit back
errors. 
	//Destructor
	Flock::~Flock()
	{
		//cout << "Flock is being deleted by destructor!" << endl;
	}
*/
	//Accessor functions
	int getSize();
	Boid getBoid(int i);
	//Mutator Functions
	void addBoid(sf::Vector2f const& t_position, bool predator = false);
	void addBoid(Boid * b);
	void flocking();
	void swarming();

};

#endif