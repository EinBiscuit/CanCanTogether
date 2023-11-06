#include "Menu_END.h"

Menu_END::Menu_END()
{
}

Menu_END::~Menu_END()
{
}

Menu_END::Menu_END(sf::RenderWindow* windae, Input* input, GameState* state, AudioManager* aud, Score_Manager* scr, Player* plr, sf::Font* font):Menus(windae,input,state,aud)
{
	score = scr;
	player = plr;
	
	body_tx.loadFromFile("gfx/Bird/Bird_LARGE.png");

	//set up animation
	for (int i = 0; i < 6; i++)
	{
		END.addFrame(sf::IntRect(15+i*1000,0,1000,1000));
	}
	END.setFrameSpeed(0.1f);

	//set up body of a big boy
	body.setSize(sf::Vector2f(window->getSize().x,window->getSize().x)*1.1f);
	body.setOrigin(body.getSize() * 0.5f);
	body.setPosition(window->getSize().x*1.8f,500);
	body.setTexture(&body_tx);
	body.setTextureRect(END.getCurrentFrame());

	TargetPos = sf::Vector2f(window->getSize()) * 0.5f + sf::Vector2f(50,170);

	//buttons and txt

	sf::Vector2f size = sf::Vector2f(window->getSize()) * 0.105f;
	sf::Vector2f position = sf::Vector2f( window->getSize().x*0.1f, window->getSize().y * 0.4f);
	sf::Vector2f spacingX = { window->getSize().x * 0.2f, 0 };
	//TITLE

	Title = new Button(size*2.f, position ,"g THATS ALL FOLK'S h",font);

	//BUTTONS
	position += {-5, size.y*3.f};

	button[0] = new Button(size * 0.8f, position, "MENU", font);
	position += spacingX*0.8f;
	button[1] = new Button(size * 0.8f, position, "RESTART", font);
	position += spacingX;
	button[2] = new Button(size * 0.8f, position, "EXIT", font);

	
}

void Menu_END::update(float dt)
{
	if (player->getPosition().x > 0)
	{
		player->setVelocity(-300, 0);
		player->setPlayerState(Player::PlayerState::WALK);
		player->setHeadAnimation(true);
		player->getHeadAnimation()->reset();
	
	}
	else
	{
		player->setPlayerState(Player::PlayerState::IDLE);
		
		END.animate(dt);
		body.setTextureRect(END.getCurrentFrame());

		if (Vector::magnitude(TargetPos - body.getPosition()) < 20.f)
		{
			END.setLooping(false);
			if (!quack)
			{
				audio->playSoundbyName("Seagull");
			}
			quack = true;
		}
		else
		body.move(sf::Vector2f(-300, 0) * dt);
	}
	
}

void Menu_END::handleInput()
{
	if (!END.getLooping())
	{
		if (button[0]->handleInput(input))
		{
			gameState->setCurrentState(State::MENU);
			return;
		}
		if (button[1]->handleInput(input)) 
		{ 
			gameState->setCurrentState(State::RESTART);
			return;
		}
		if (button[2]->handleInput(input))
		{ 
			window->close();
			return;
		}
	}
}

void Menu_END::draw()
{
	window->draw(body);

	if (!END.getLooping())
	{
		for (int i = 0; i < 3; i++)
		{
		button[i]->draw(window);
		}

		Title->draw(window);
	}
}

void Menu_END::reset()
{
	quack = false;
	END.setLooping(true);
	END.reset();
	END.setPlaying(true);
	body.setPosition(window->getSize().x *1.8f, 500);
}
