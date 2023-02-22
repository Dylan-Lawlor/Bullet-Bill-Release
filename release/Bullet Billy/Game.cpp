/// <summary>
/// @author Dylan Lawlor
/// @date 2023
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
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
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processMouseDown(newEvent); // user clicks
		}
		if (sf::Event::MouseMoved == newEvent.type)
		{
			processMouseMove(newEvent); // User moves mouse
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouseUp(newEvent); // User releases mouse click
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::F1 == t_event.key.code)
	{
		m_graphics = !m_graphics;
	}
	if (sf::Keyboard::Up == t_event.key.code)
	{
		adjustGravity(0.005f);
	}
	if (sf::Keyboard::Down == t_event.key.code)
	{
		adjustGravity(-0.005f);
	}
	
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{

	
	if (m_exitGame)
	{
		m_window.close();
	}
	if (m_firing)
	{
		moveBall();
		m_firing = checkGround(m_ballLocation);
	}
	if (m_firing)
	{
		m_firing = checkCollision(m_ball, m_wall, false);
	}
	if (m_firing)
	{
		m_firing = checkCollision(m_ball, m_target, true);
	}

	
	moveTarget();
	animateTarget();

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	if (m_graphics)
	{
		m_window.draw(m_backgroundSprite);
		m_window.draw(m_wallSprite);
		m_window.draw(m_arrowSprite);
		m_window.draw(m_bulletSprite);
		m_window.draw(m_barrelSprite);
		m_window.draw(m_baseSprite);
		if (m_aiming)
		{
			drawAimLine();
		}
		m_window.draw(m_gumbaSprite);

	}
	else
	{
		m_window.draw(m_canon);
		m_window.draw(m_ball);
		m_window.draw(m_wall);
		m_window.draw(m_gravityBar);
		if (m_aiming)
		{
			m_window.draw(m_aimLine);
		}
		m_window.draw(m_target);
	}
	m_window.draw(m_missMessage);
	m_window.draw(m_hitMessage);
	m_window.display();

	
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

	m_missMessage.setFont(m_ArialBlackfont);
	m_missMessage.setString("Misses: ");
	m_missMessage.setPosition(40.0f, 110.0f);
	m_missMessage.setCharacterSize(20U);
	m_missMessage.setFillColor(sf::Color::Blue);

	m_hitMessage.setFont(m_ArialBlackfont);
	m_hitMessage.setString("Hits: ");
	m_hitMessage.setPosition(40.0f, 50.0f);
	m_hitMessage.setCharacterSize(20U);
	m_hitMessage.setFillColor(sf::Color::Blue);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	setupCanon();
	setupTarget();
	setupGravity();

	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\background.jpg"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading background" << std::endl;
	}
	m_backgroundSprite.setTexture(m_backgroundTexture);

	
}

void Game::setupCanon()
{
	m_ball.setFillColor(sf::Color::Red);
	m_ball.setRadius(10.0f);
	m_ball.setOrigin(10.0f, 10.0f);
	m_ballLocation = sf::Vector2f{ 100.0f, 550.0f };
	m_ball.setPosition(m_ballLocation);

	m_canon.setFillColor(sf::Color::Black);
	m_canon.setSize(sf::Vector2f{ 20.0f,70.0f });
	m_canon.setPosition(100.0f, 550.0f);
	m_canon.setOrigin(10.0f, 35.0f);
	m_canon.setRotation(45.0f);

	if (!m_baseTexture.loadFromFile("ASSETS\\IMAGES\\base.png"))
	{
		std::cout << "Problem loading base" << std::endl;
	}
	m_baseSprite.setTexture(m_baseTexture);
	m_baseSprite.setPosition(70.0f, 531.0f);

	if (!m_barrelTexture.loadFromFile("ASSETS\\IMAGES\\barrel.png"))
	{
		std::cout << "Problem loading barrel" << std::endl;
	}
	m_barrelSprite.setTexture(m_barrelTexture);
	m_barrelSprite.setOrigin(22.0f, 45.0f);
	m_barrelSprite.setRotation(45.0f);
	m_barrelSprite.setPosition(100.0f, 550.0f);

	if (!m_bulletTexture.loadFromFile("ASSETS\\IMAGES\\bullet.png"))
	{
		std::cout << "Problem loading bullet" << std::endl;
	}
	m_bulletSprite.setTexture(m_bulletTexture);
	m_bulletSprite.setOrigin(20.0f, 20.0f);
	m_bulletSprite.setRotation(45.0f);
	m_bulletSprite.setPosition(100.0f, 550.0f);



}

void Game::setupTarget()
{
	m_wall.setFillColor(sf::Color::Black);
	m_wall.setSize(sf::Vector2f{ 32.0f,100.0f });
	m_wall.setPosition(400.0f, 500.0f);

	m_target.setFillColor(sf::Color::Green);
	m_target.setSize(sf::Vector2f{ 55.0f,55.0f });
	m_targetLocation = sf::Vector2f{ 420.0f,545.0f };
	m_target.setPosition(m_targetLocation);

	if (!m_wallTexture.loadFromFile("ASSETS\\IMAGES\\wall.jpg"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading wall" << std::endl;
	}
	m_wallSprite.setTexture(m_wallTexture);
	m_wallSprite.setPosition(400.0f, 500.0f);
	m_wallTexture.setRepeated(true);
	m_wallSprite.setTextureRect(sf::IntRect{ 0,0,32,100 });

	if (!m_gumbaTexture.loadFromFile("ASSETS\\IMAGES\\gumba.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading gumba" << std::endl;
	}

	m_gumbaSprite.setTexture(m_gumbaTexture);
	m_gumbaSprite.setPosition(m_targetLocation);
	m_gumbaSprite.setTextureRect(sf::IntRect{ 0,0,52,54 });
	m_gumbaSprite.setScale(-1.0f, 1.0f);
	m_gumbaSprite.setOrigin(52.0f, 0.0f);
}

void Game::setupGravity()
{

	m_gravityBar.setFillColor(sf::Color::Blue);
	m_gravityBar.setSize(sf::Vector2f{ 20.0f,60.0f });
	m_gravityBar.setPosition(760.0f, 40.0f);

	if (!m_arrowTexture.loadFromFile("ASSETS\\IMAGES\\arrow.png"))
	{
		std::cout << "problem loading arrow" << std::endl;
	}
	m_arrowSprite.setTexture(m_arrowTexture);
	m_arrowSprite.setPosition(m_gravityBar.getPosition());
}

void Game::moveTarget()
{
	const float SPEED = 0.6f;
	const float LEFT_EDGE = 420.0f;
	const float RIGHT_EDGE = 740.0f;

	if (m_targetLocation.x < LEFT_EDGE)
	{
		m_targetVelocity.x = SPEED;
		m_gumbaSprite.setScale(-1.0f, 1.0f);
		m_gumbaSprite.setOrigin(52.0f, 0.0f);
	}
	if (m_targetLocation.x > RIGHT_EDGE)
	{
		m_targetVelocity.x = -SPEED;
		m_gumbaSprite.setScale(1.0f, 1.0f);
		m_gumbaSprite.setOrigin(0.0f, 0.0f);
	}
	m_targetLocation += m_targetVelocity;
	m_target.setPosition(m_targetLocation);
	m_gumbaSprite.setPosition(m_targetLocation);

}

void Game::animateTarget()
{
	int frame = 0;
	const int FRAME_WIDTH = 52;
	const int FRAME_HEIGTH = 54;

	m_gumbaFrameCounter += m_gumbaFrameIncrement;
	frame = static_cast<int>(m_gumbaFrameCounter);
	if (frame >= GUMBA_FRAMES)
	{
		frame = 0;
		m_gumbaFrameCounter = 0.0f;
	}
	if (frame != m_gumbaFrame)
	{
		m_gumbaFrame = frame;
		m_gumbaSprite.setTextureRect(sf::IntRect{ frame * FRAME_WIDTH,0,FRAME_WIDTH,FRAME_HEIGTH });
	}

}

void Game::processMouseDown(sf::Event t_event)
{
	if (!m_aiming && !m_firing)
	{
		m_mouseEnd.x = static_cast<float>(t_event.mouseButton.x);
		m_mouseEnd.y = static_cast<float>(t_event.mouseButton.y);
		m_aiming = true;
		setAimLine();
	}
}

void Game::processMouseMove(sf::Event t_event)
{
	m_mouseEnd.x = static_cast<float>(t_event.mouseMove.x);
	m_mouseEnd.y = static_cast<float>(t_event.mouseMove.y);
	setAimLine();

}

void Game::processMouseUp(sf::Event t_event)
{
	if (m_aiming && !m_firing)
	{
		m_aiming = false; // set aiming to false when the mouse button is released
		m_firing = true;
		m_mouseEnd.x = static_cast<float>(t_event.mouseButton.x);
		m_mouseEnd.y = static_cast<float>(t_event.mouseButton.y);
		m_ballVelocity = m_mouseEnd - m_canonEnd;
		m_ballVelocity = m_ballVelocity / 50.0f;
		m_aimLine.clear();
	}
	
}

void Game::setAimLine()
{
	float angleD; // Degrees
	float angleR; // Radians
	const float PI = 3.14f;
	sf::Vector2f line;

	
	line = m_mouseEnd - m_canonEnd; // get length of line
	angleR = std::atan2f(line.y, line.x); // y is opposite, x is adjacent
	angleD = angleR * 180 / PI; 
	m_canon.setRotation(angleD + 90.0f); // must plus 90 degrees else Cannon is perpendicular to mouse
	m_barrelSprite.setRotation(angleD + 90.0f); // sets the sprite the same angle as m_cannon
	sf::Vertex point;
	point.color = sf::Color::Black;
	m_aimLine.clear();
	point.position = m_mouseEnd;
	m_aimLine.append(point);
	point.position = m_canonEnd;
	m_aimLine.append(point);

}

void Game::drawAimLine()
{
	sf::Vector2f velocity = (m_mouseEnd - m_canonEnd) / 50.0f;
	sf::Vector2f location = m_ballLocation;
	float angle;
	sf::Color colour{ 255u,255u,255u,255u };

	for (int i = 0; i < 255; i++)
	{
		velocity += m_gravity;
		location += velocity;
		if (i % 20 == 0)
		{
			angle = std::atan2(velocity.y, velocity.x);
			angle = angle * 180.0f / 3.14f;
			m_bulletSprite.setRotation(angle + 90.0f);
			m_bulletSprite.setPosition(location);
			colour.a = 255 - i;
			m_bulletSprite.setColor(colour);
			m_window.draw(m_bulletSprite);
		}
	}
	m_bulletSprite.setColor(sf::Color::White);
}

void Game::moveBall()
{
	float angle;
	m_ballVelocity += m_gravity;
	m_ballLocation += m_ballVelocity;
	m_ball.setPosition(m_ballLocation);
	m_bulletSprite.setPosition(m_ballLocation);
	angle = std::atan2(m_ballVelocity.y, m_ballVelocity.x);
	angle = angle * 180.0f / 3.14f;
	m_bulletSprite.setRotation(angle + 90.0f);
}

bool Game::checkGround(sf::Vector2f t_location)
{
	if (t_location.y > 600.0f || t_location.x > 800.0f || t_location.x < 0.0f)
	{
		m_ballLocation = sf::Vector2f{ 100.0f,550.0f };
		m_ball.setPosition(m_ballLocation);
		m_misses++;
		m_missMessage.setString("Misses: " + std::to_string(m_misses));
		return false;
	}
	return true;
}

bool Game::checkCollision(sf::CircleShape& t_ball, sf::RectangleShape& t_block, bool t_target)
{
	bool result{ true };
	sf::FloatRect circle = t_ball.getGlobalBounds();
	sf::FloatRect block = t_block.getGlobalBounds();

	if (circle.intersects(block))
	{
		m_ballLocation = sf::Vector2f{ 100.0f,550.0f };
		m_ball.setPosition(m_ballLocation);
		result = false;
		if (t_target)
		{
			m_hits++;
			m_hitMessage.setString("Hits: " + std::to_string(m_hits));
		}
		else
		{
			m_misses++;
			m_missMessage.setString("Misses: " + std::to_string(m_misses));
		}
	}

	return result;
}

void Game::adjustGravity(float t_adjustment)
{
	float magnitude;

	m_gravity.y += t_adjustment;
	m_gravityBar.setFillColor(sf::Color::Blue);
	m_arrowSprite.setColor(sf::Color::White);
	if (m_gravity.y < 0.0f)
	{
		m_gravityBar.setFillColor(sf::Color::Red);
		m_arrowSprite.setColor(sf::Color::Blue);
		m_gravity.y = 0.0f; // stop user going into negative gravity.
	}


	magnitude = m_gravity.y * 500.0f + 35.0f;
	m_gravityBar.setSize(sf::Vector2f{ 20.0f,magnitude });
	m_arrowSprite.setScale(1.0f, magnitude/60.0f);
	
	std::cout << m_gravity.y << std::endl;
}
