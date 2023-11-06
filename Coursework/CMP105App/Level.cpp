#include "Level.h"

Level::Level(sf::RenderWindow* hwnd, Input* Inpt, Input* NetInpt, GameState* state, AudioManager* aud)
{
	window = hwnd;
	input = Inpt;
	ninput = NetInpt;
	gameState = state;
	audio = aud;

	// initialise game objects

	font.loadFromFile("font/CanCandB_free.ttf");

	//Score
	score = new Score_Manager(window, &font);

	//Player

	player = new Player(sf::Vector2f(35, 35), audio);
	player->setPosition(window->getSize().x * 0.5f, window->getSize().y * 0.8f);

	pawn = new Player(sf::Vector2f(35, 35), audio, false);
	pawn->setPosition(window->getSize().x * 0.5f, window->getSize().y * 0.8f);

	//Collidables
	Collidables = new Collidable_Manager(window, audio, score);

	//Background*

	background = new Background(player);
	background->setup(window);
	foreground = new Foreground(player);
	foreground->setup(window);

	//Stage
	Floor.setSize(sf::Vector2f(window->getSize().x, 50));
	Floor.setPosition(0, window->getSize().y * 0.94f);
	Floor.setCollisionBox(0, 0, window->getSize().x, 50);
	Floor.setColliderType(ColliderType::STAGE);
	Floor.setFillColor(sf::Color::Transparent);

	Bumper_Right.setSize(sf::Vector2f(window->getSize().x*0.5f, 50));
	Bumper_Right.setOrigin(Bumper_Right.getSize()*0.5f);
	Bumper_Right.setPosition(window->getSize().x*0.9f, window->getSize().y * 0.95f);
	Bumper_Right.setRotation(-45);
	Bumper_Right.setCollisionBox(0, 0, Bumper_Right.getSize().x, Bumper_Right.getSize().y);
	Bumper_Right.setColliderType(ColliderType::STAGE);


	Bumper_Left.setSize(sf::Vector2f(window->getSize().x*0.5f, 50));
	Bumper_Left.setOrigin(Bumper_Left.getSize() * 0.5f);
	Bumper_Left.setPosition(window->getSize().x*0.1f, window->getSize().y * 0.95f);
	Bumper_Left.setRotation(45);
	Bumper_Left.setCollisionBox(0, 0, Bumper_Left.getSize().x, Bumper_Left.getSize().y);
	Bumper_Left.setColliderType(ColliderType::STAGE);

	THE_END = new Menu_END(window,input,gameState,audio,score,player,&font); // it ties everything together!

	//store all objects for easy access for collisions   
			 
	space.push_back(&Bumper_Right);
	space.push_back(&Bumper_Left);
	space.push_back(&Floor);
	space.push_back(player);
	space.push_back(pawn);
}

Level::~Level()
{

}

// handle user input
void Level::handleInput(float dt)
{
	if (Timer <= audio->getMusic()->getDuration().asSeconds() - 11.f)
	{
	//PAUSE
		if (input->isKeyDown(sf::Keyboard::Escape))
		{
			input->setKeyUp(sf::Keyboard::Escape);
			gameState->setCurrentState(State::PAUSE);
		}

		player->handleInput(dt, input);
		pawn->handleInput(dt,ninput);
	}
	else
	{
		THE_END->handleInput();
	}
}

// Update game objects
void Level::update(float dt)
{
	Timer += dt;// count alapsed time in level

	if (Timer >= audio->getMusic()->getDuration().asSeconds())
	{
		audio->stopAllMusic();  // if music ends stop it
	}

	player->update(dt);
	pawn->update(dt);
	Collidables->update(dt, space);

	if (Timer >= audio->getMusic()->getDuration().asSeconds() - 11.f)
	{
		THE_END->update(dt); // start end before music stops
	}
	else
	{
		Collidables->spawn();
		
		
		background->update(dt); 
		foreground->update(dt);

		score->update();
	}
}

// Render level
void Level::render()
{
	//beginDraw();  begin and end draw is now handleled in main

	background->draw(window);

	player->draw(window);
	pawn->draw(window);
	window->draw(Floor);
	Collidables->draw();
	window->draw(Bumper_Left);
	window->draw(Bumper_Right);

	window->draw(Block);
	window->draw(Square1);

	THE_END->draw();
	window->draw(*foreground);
	score->draw();
	

	//endDraw();
}



void Level::restart()
{
	Timer = 0;
	player->setPosition(window->getSize().x * 0.5f, window->getSize().y * 0.8f);
	Collidables->reset();
	background->reset();
	foreground->reset();
	audio->stopAllMusic();
	THE_END->reset();
}


void Level::GetDynamicObjects(Player*& player, Player*& pawn, Collidable_Manager*& Collidables)
{
	player = this->player;
	pawn = this->pawn;
	Collidables = this->Collidables;

}

// Begins rendering to the back buffer. Background colour set to light blue.
void Level::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}