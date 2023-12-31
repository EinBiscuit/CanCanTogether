// Main
// Entry point of the application.
// Handles window creation and window events.
// Contains the game loop, updates input class and contains the level objects.
// @author Paul Robertson

#include <iostream>
#include "Framework/GameState.h"
#include "Menu_Main.h"
#include "Menu_Pause.h"
#include "Menu_Credits.h"
#include "Level.h"
#include "Network.h"

int main()
{
	//Create the window
	
	sf::RenderWindow window(sf::VideoMode(1200, 675), "301 - CAN CAN Together",sf::Style::Close);
	//AUDIO

	AudioManager audioManager;

	audioManager.addMusic("sfx/Music/Orchestra_tuning.ogg", "Tuning"); // Menu music
	audioManager.addMusic("sfx/Music/Can_Can.ogg", "CanCan"); // Game music

	audioManager.addSound("sfx/Steve_ugh.ogg", "P_UGH");// player ugh sound
	audioManager.addSound("sfx/Seagulls_sound.ogg", "Seagull");// seagull quack
	audioManager.addSound("sfx/Box_drop.ogg", "Box");// box sound

	//Create state
	GameState currentState;
	currentState.setCurrentState(State::MENU);

	// Initialise input and level objects.
	Input input;

	//create network;

	Network Net;

	//pass the net input pointer so that
	Level level(&window, &input,Net.getNetworkInput(), &currentState,&audioManager);
	
	Menu_Main mainmenu(&window, &input, &currentState, &audioManager);
	Menu_Pause pausemenu(&window, &input, &currentState, &audioManager);
	Menu_Credits credits(&window, &input, &currentState);
	
	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime;

	//NETWORK

	Collidable_Manager* collidables;
	Player* player;
	Player* pawn;

	level.GetDynamicObjects(player,pawn,collidables);

	Net.Init(collidables, player, pawn, &window, mainmenu.GetFont(), &input, &currentState, &audioManager);

	//window.setFramerateLimit(144);

	char textentered = NULL;

	// Game Loop
	while (window.isOpen())
	{
		textentered = 0;

		// Handle window events.
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
			window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));			
				break;
			case sf::Event::KeyPressed:
				// update input class
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				//update input class
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseMoved:
				//update input class
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setMouseLDown(true);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRDown(true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setMouseLDown(false);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRDown(false);
				}
				break;
			case sf::Event::TextEntered:
				if (event.text.unicode < 128)
				{
					textentered = static_cast<char>(event.text.unicode);
				}
			default:
				// don't handle other events
				break;
			}
		}

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();


		// Call standard game loop functions (input, update and render)
		// depending on game state

		window.clear(sf::Color(255, 255, 255));

		Net.update(deltaTime);

		switch(currentState.getCurrentState())
		{
		case State::MENU:
			if (audioManager.getMusic()->getStatus() == sf::SoundSource::Status::Stopped) audioManager.playMusicbyName("Tuning");
			mainmenu.handleInput();
			mainmenu.render();
			Net.Network_Menu(textentered);
			break;

		case State::CREDITS:
			credits.handleInput();
			credits.draw();
			break;

		case State::LEVEL:
			if (audioManager.getMusic()->getStatus() == sf::SoundSource::Status::Stopped) audioManager.playMusicbyName("CanCan");
			if (audioManager.getMusic()->getStatus() == sf::SoundSource::Status::Paused) audioManager.getMusic()->play();
			level.handleInput(deltaTime);
			level.update(deltaTime);
			level.render();
			break;

		case State::PAUSE:
			if (audioManager.getMusic()->getStatus() == sf::SoundSource::Status::Playing) audioManager.getMusic()->pause();
			pausemenu.handleInput();
			pausemenu.render();
			break;

		case State::RESTART:
			level.restart();
			currentState.setCurrentState(State::LEVEL);
			break;
		}

		window.display();
		
	}

}

