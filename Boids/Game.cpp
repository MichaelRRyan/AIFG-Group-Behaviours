#include "Game.h"
#include <iostream>
using namespace std;


/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup the image
/// </summary>
Game::Game() :
	m_boidShape{ 8.0f, 3u },
	m_predatorShape{ 10.0f, 3u },
	action{ Action::Flock }

// Tying to create window using desktop values did not work here.
// So used the Create method inside the constructor/
// sf::RenderWindow m_window( sf::VideoMode(desktop.width - 100, desktop.height - 100, desktop.bitsPerPixel), "Flocking", sf::Style::None );
{
	window_height = desktop.height;
	window_width = desktop.width;

	//Having the style of "None" gives a false-fullscreen effect for easier closing and access.
	m_window.create(sf::VideoMode(window_width - 100, window_height - 100, desktop.bitsPerPixel), "Flocking", sf::Style::None);
	m_window.setVerticalSyncEnabled(true);
	m_exitGame = false; //when true game will exit

	m_actionMessage.setFont(m_font);
	m_actionMessage.setString("Flocking");
	m_actionMessage.setFillColor(sf::Color::White);

	if (!m_font.loadFromFile("assets/fonts/ariblk.ttf"))
		std::cout << "failed loading ariblk.ttf font file" << std::endl;
	else
		std::cout << "successfully loaded ariblk.ttf font file" << std::endl;

	// Setup the boid shape.
	m_boidShape.setOutlineColor(sf::Color(0, 255, 0));
	m_boidShape.setFillColor(sf::Color::Green);
	m_boidShape.setOutlineColor(sf::Color::White);
	m_boidShape.setOutlineThickness(1);
	m_boidShape.setRadius(boidsSize);

	// Setup the predator shape.
	m_predatorShape.setOutlineColor(sf::Color(255, 0, 0));
	m_predatorShape.setFillColor(sf::Color(255, 0, 0));
	m_predatorShape.setOutlineColor(sf::Color::White);
	m_predatorShape.setOutlineThickness(1);
	m_predatorShape.setRadius(enemySize);

	for (int i = 0; i < 200; i++) //Number of boids is hardcoded for testing pusposes.
	{
		//Adding the boid to the flock and adding the shapes to the vector<sf::CircleShape>
		flock.addBoid({ window_width / 3.0f, window_height / 3.0f });
	}

}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but method needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
			m_exitGame = true;
		else if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
			processKeys(newEvent);
		else if (sf::Event::MouseButtonPressed == newEvent.type || sf::Event::MouseButtonReleased == newEvent.type) //user pressed a mouse button
			processMouse(newEvent);
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
		m_exitGame = true;
	else if (sf::Keyboard::Space == t_event.key.code)
	{
		if (action == Action::Flock)
		{
			action = Action::Swarm;
			m_actionMessage.setString("Swarm");
		}
		else
		{
			action = Action::Flock;
			m_actionMessage.setString("Flock");
		}
	}
}

/// <summary>
/// deal with mouse button presses from the user
/// </summary>
/// <param name="t_event">mouse press event</param>
void Game::processMouse(sf::Event t_event)
{
	if (sf::Event::MouseButtonReleased == t_event.type && sf::Mouse::Left == t_event.mouseButton.button)
	{
		//Gets mouse coordinates, sets that as the location of the boid and the shape
		sf::Vector2i mouseCoords = sf::Mouse::getPosition(m_window);

		//Adds newly created boid and shape to their respective data structure
		flock.addBoid(static_cast<sf::Vector2f>(mouseCoords), true);
		// New Shape is drawn on next render loop
	}

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{

	//Applies the three rules to each boid in the flock and changes them accordingly.
	if (action == Action::Flock)
		flock.flocking();
	else
		flock.swarming();

	if (m_exitGame)
		m_window.close();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	//Clears previous frames of visualization to not have clutter. (And simulate animation)
	m_window.clear();

	//Draws all of the Boids out, and applies functions that are needed to update.
	for (int i = 0; i < flock.getSize(); i++)
	{
		sf::CircleShape & shape = (flock.getBoid(i).predator) ? m_predatorShape : m_boidShape;
		shape.setPosition(flock.getBoid(i).location.x, flock.getBoid(i).location.y);

		// Calculates the angle where the velocity is pointing so that the triangle turns towards it.
		float theta = flock.getBoid(i).angle(flock.getBoid(i).velocity);
		shape.setRotation(theta);

		m_window.draw(shape);
	}

	m_window.draw(m_actionMessage);
	m_window.display();
}