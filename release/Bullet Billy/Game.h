/// <summary>
/// author Dylan Lawlor 2023
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void setupCanon();
	void setupTarget();
	void setupGravity();
	void moveTarget();
	void animateTarget();
	void processMouseDown(sf::Event t_event);
	void processMouseMove(sf::Event t_event);
	void processMouseUp(sf::Event t_event);
	void setAimLine();
	void drawAimLine();
	void moveBall();
	bool checkGround(sf::Vector2f t_location);
	bool checkCollision(sf::CircleShape& t_ball, sf::RectangleShape& t_block, bool t_target);
	void adjustGravity(float t_adjustment);

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Text m_missMessage; // text used for misses
	sf::Text m_hitMessage; // text used for hits
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	sf::Texture m_gumbaTexture; // Texture for gumba
	sf::Sprite m_gumbaSprite; // sprite for gumba
	sf::Texture m_backgroundTexture; // Texture for background
	sf::Sprite m_backgroundSprite; // Sprite for background
	sf::Texture m_wallTexture; // Texture for wall
	sf::Sprite m_wallSprite; // Sprite for wall
	sf::Texture m_arrowTexture; // texture for gravity arrow
	sf::Sprite m_arrowSprite; // sprite for gravity arrow
	sf::Texture m_baseTexture; // texture for canon base
	sf::Sprite m_baseSprite; // sprite for canon base
	sf::Texture m_barrelTexture; // texture for canon barrel
	sf::Sprite m_barrelSprite; // sprite for cannon barrel
	sf::Texture m_bulletTexture; // texture for bullet 
	sf::Sprite m_bulletSprite; // sprite for bullet
	bool m_exitGame; // control exiting game
	int m_misses{ 0 }; // count misses
	int m_hits{ 0 }; // count hits

	sf::RectangleShape m_wall; // box for wall 
	sf::RectangleShape m_target; // box for target to shoot at
	sf::RectangleShape m_gravityBar; // gravity indicator
	sf::Vector2f m_targetLocation; // Location of target
	sf::Vector2f m_targetVelocity{ 0.6f,0.0f }; // Velocity of target
	sf::RectangleShape m_canon; // Box for cannon
	sf::Vector2f m_mouseEnd; // where user has mouse at 
	sf::Vector2f m_canonEnd{ 100.0f,550.0f }; // end of cannon
	sf::VertexArray m_aimLine{ sf::Lines }; // line for aiming
	sf::CircleShape m_ball; // cannon ball
	sf::Vector2f m_ballLocation; // ball position
	sf::Vector2f m_ballVelocity; // ball movement vector
	sf::Vector2f m_gravity{ 0.0f,0.05f }; // accelerate downwards
	int m_gumbaFrame{ -1 }; // frame occurs straight away
	const int GUMBA_FRAMES = 20;
	float m_gumbaFrameCounter = 0.0f;
	float m_gumbaFrameIncrement = 0.3f;
	bool m_graphics{ false }; // are you in graphics mode?	
	bool m_aiming{ false }; // are you aiming? 
	bool m_firing{ false }; // are you firing? 
	


};

#endif // !GAME_HPP

