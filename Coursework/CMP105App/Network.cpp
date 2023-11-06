#include "Network.h"

//-----------------The Creation--------------------------------------------------------------

Network::Network()
{
	networkInpt = new Input();
}

Network::~Network()
{
	delete networkInpt;
	delete inputBoxIP;
	delete inputBoxPORT;
}

void Network::Init(Collidable_Manager* DynamicObjects, Player* PlayerPawn, Player* NetworkPawn, sf::RenderWindow* window, sf::Font* font, Input* input, GameState* gameSt, AudioManager* audio)
{
	//Initialize socket

	auto bind_stat = bind(sf::Socket::AnyPort);
	if (bind_stat != sf::Socket::Done)
	{
		std::cout << "ERROR: failed to initialise Network: " << bind_stat;
		return;
	};

	setBlocking(false);

	//game state info
	wnd = window;
	inpt = input;
	Gstate = gameSt;
	aud = audio;

	//dynamic objects to sync
	CollidableObjects = DynamicObjects;
	LocalPlayer = PlayerPawn;
	NetworkPlayer = NetworkPawn;

	// set up the network menu;

	sf::Vector2f size = window->getView().getSize() * 0.1f;
	sf::Vector2f position = window->getView().getSize();
	position.x *= 0.1f;
	position.y *= 0.9f;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();

	NetworkInfo.setFont(*font);
	NetworkInfo.setString("INSTANCE IP ADDRESS: " + ip.toString() + "\n\n" + "INSTANCE PORT NUMBER: " + std::to_string(getLocalPort()));
	NetworkInfo.setCharacterSize(20);
	NetworkInfo.setPosition(position);
	NetworkInfo.setFillColor(sf::Color::Black);
	NetworkInfo.setOrigin(0, NetworkInfo.getGlobalBounds().height * 0.5f);

	position.x += NetworkInfo.getLocalBounds().width * 1.1f;
	position.y -= NetworkInfo.getLocalBounds().height * 0.5f;

	inputBoxIP = new InputField(size / 2.f, position, "REMOTE IP: ", font, ip.toString().size(), false);
	position.y += inputBoxIP->getSize().y * 1.1f;
	inputBoxPORT = new InputField(size / 2.f, position, "REMOTE PORT: ", font);

	position.x += inputBoxIP->getSize().x * 1.1f;

	NetworkStatus.setFont(*font);
	NetworkStatus.setString("NETWORK STATUS: NO.");
	NetworkStatus.setCharacterSize(20);
	NetworkStatus.setPosition(position);
	NetworkStatus.setFillColor(sf::Color::Black);
	NetworkStatus.setOrigin(0, NetworkInfo.getGlobalBounds().height * 0.5f);

	inputBoxIP->inString = sf::IpAddress::getLocalAddress().toString();


}

bool Network::Connect()
{
	//take ip from input field
	 remote_ip = sf::IpAddress(inputBoxIP->inString);

	//take port from input fields
	if (inputBoxPORT->inString.size() == 5) { // if size == port number of numbers
		remote_port = std::stoi(inputBoxPORT->inString);
	}
	else
	{
		remote_port = 0;
	}

	//if port and ip valid attempt connect
	if (remote_port && remote_ip != sf::IpAddress::None)
	{
		CommandID connectAs = (!Server) ? SYSTEM_CONNECT_CLIENT : SYSTEM_CONNECT_SERVER;

		if (connectAs == CommandID::SYSTEM_CONNECT_CLIENT)std::cout << "\n Attempting Connect As Client";
		else std::cout << "\n Attempting Connect As Server";

		sf::Packet pack;

		Pack(connectAs, pack);// connect as client by default

		if (send(pack, remote_ip, remote_port) != sf::Socket::Done )
		{
			std::cout << "\n ERROR: failed send ";
			return false;
		};

		//Nstate = NetworkState::CONNECTING;

		return true;
	}
	else
	{
		return false;
		//Nstate = NetworkState::INACTIVE;
	}
}

//------------------PING PONG ---------------------------------------------------------------

void Network::Pong()
{
	//std::cout << "\n pong";
	sf::Packet pong;
	Pack(PONG, pong);
	send(pong, remote_ip, remote_port);
}

void Network::Ping(bool pongd)
{
	if (!pongd)
	{
		//std::cout << "\n ping";
		sf::Packet ping;
		
		time_ping_sent = Pack(PING, ping);

		send(ping, remote_ip, remote_port);
		//std::cout << "\n sent to " << remote_ip.toString() << "  " << remote_port;
	}
	else // pongd
	{
		ping = clock.getElapsedTime().asMilliseconds() - time_ping_sent;
		std::cout << "\n ping: " << ping << "ms";
	}
}

//------------------PACK UNPACK---------------------------------------------------------------

int Network::Pack(CommandID ThingsTopack, sf::Packet& packet) // parametrised command to be called before sending 
{
	packet.clear();

	int time = clock.getElapsedTime().asMilliseconds(); // timestamp the goods.
	uint8_t count = 0;

	packet << time;

	if (ThingsTopack)
	{
		packet << (int16_t)ThingsTopack;

		for (int i = 0; i < 4; i++) // only INPUT and 3 bits after have data content to them in commands -> input , player data, collidable data;
		{
			if (ThingsTopack & INPUT << i)
			{
				switch (INPUT << i)
				{
				case CommandID::INPUT: // client sends

					//std::cout << "\n sending input" << inpt->getMouseX();

					Input_pack input;
					input.leftC = inpt->isMouseLDown();
					input.rightC = inpt->isMouseRDown();
					input.positionX = inpt->getMouseX();
					packet << input;

					break;
				case CommandID::PLAYER: // server sends
					Player_pack player;

					player.kicker_leg = LocalPlayer->getKickerLeg();
					player.kicking_state = (int8_t)LocalPlayer->getKickerState();
					player.positionX = LocalPlayer->getPosition().x;
					player.stager = LocalPlayer->getStagger();
					player.velocityX = LocalPlayer->getVelocity().x;

					packet << player;

					break;
				case CommandID::COLLIDABLE: // server sends

					count = CollidableObjects->ALIVE.size();
					packet << count;

					//std::cout << "\n sent count " << CollidableObjects->ALIVE.size();

					for (auto i : CollidableObjects->ALIVE)
					{
						Bread_pack bread;
						bread.number = i->getInstanceNum();
						bread.velocity = i->getVelocity();
						bread.position = i->getPosition();
						bread.rotation = i->getRotationDir();
						packet << bread;
					}
					break;

				case CommandID::SCORE:

					packet << CollidableObjects->score->getScore();
					//std::cout << "\n scorre sent " << CollidableObjects->score->getScore();
					break;

				default:
					break;
				}
			}
		}
	}

	return time;
}

std::pair<int, CommandID> Network::Unlabel(sf::Packet& packet)
{
	std::pair<int, CommandID> Label;

	packet >> Label.first; //retirve time
	packet >> (int16_t&)Label.second;

	return Label;
}

void Network::Unpack(sf::Packet& packet, CommandID label, std::list<Collidable*>* tempDynamic, Player* tempPawn, Input* tempInput)
{
	if (label & CommandID::INPUT)
	{
		Input_pack inpt;
		packet >> inpt;

		tempInput->setMouseX(inpt.positionX);
		tempInput->setMouseLDown(inpt.leftC);
		tempInput->setMouseRDown(inpt.rightC);

		//debug
		//std::cout << "\n recieved input       " << inpt.positionX;

	}
	if (label & CommandID::PLAYER)
	{
		Player_pack pawn;

		packet >> pawn;

		tempPawn->setVelocity(pawn.velocityX, 0);
		tempPawn->setKickerLeg(pawn.kicker_leg);
		tempPawn->setStagger(pawn.stager);
		tempPawn->setKickerState((Leg::kick)pawn.kicking_state);
	}
	if (label & CommandID::COLLIDABLE)
	{
		uint8_t count;
		packet >> count;

		//std::cout <<"\n count recieved " << count;

		if (count > 0)
		{
			for (int i = 0; i < count; i++)
			{
				Bread_pack bread;
				packet >> bread;

				// compare all exisitng instances of bread to the one recieved and if it exists set its velocity position and rotation accordingly

				bool contains = false;
				for (auto c : *tempDynamic)
				{
					if (c->getInstanceNum() == bread.number)
					{
						contains = true;

					//	if(Vector::magnitude(c->getPosition() - bread.position) > 10) // if bread diverges too much force its position
						c->setPosition(bread.position);
						c->setVelocity(bread.velocity);
						c->setRotating((rotationDir)bread.rotation);
					}
				}

				//if it doesnt add one with the relevant number

				if (!contains)
				{
					tempDynamic->push_back(new Collidable(bread.number));
					tempDynamic->back()->setSize(sf::Vector2f(50, 50));
					tempDynamic->back()->setCollisionBox(sf::FloatRect(0, 0, 40, 40));
					tempDynamic->back()->setColliderType(ColliderType::COLLIDABLE);
					tempDynamic->back()->setOrigin(0.5f * tempDynamic->back()->getSize());
					tempDynamic->back()->setRotationSpeed(180);
					tempDynamic->back()->setRotation(25);
					tempDynamic->back()->setTexture(&(CollidableObjects->bread_tx));
					tempDynamic->back()->setFreefall(true);

					tempDynamic->back()->setPosition(bread.position);
					tempDynamic->back()->setVelocity(bread.velocity);
					tempDynamic->back()->setRotating((rotationDir)bread.rotation);
				}

			}
		}
	}
	if (label & CommandID::SCORE)
	{
		uint16_t rec_score;
		packet >> rec_score;
		//std::cout << "\n scorre recieved " << rec_score;
		CollidableObjects->score->setScore(rec_score);
	}
}


//------------------NETWORK MENU--------------------------------------------------------------

void Network::Network_Menu(char inputTXT)
{
	inputBoxIP->handleInput(inpt,inputTXT);
	inputBoxPORT->handleInput(inpt, inputTXT);
	wnd->draw(NetworkInfo);
	wnd->draw(NetworkStatus);
	inputBoxIP->draw(wnd);
	inputBoxPORT->draw(wnd);
}

//------------------UPDATE--------------------------------------------------------------------

CommandID Network::ListenConnections(sf::Packet& recieve)
{
	//recieve someting;
	///sf::Packet recieve;

	unsigned short port;
	sf::IpAddress ip;

	Status STATUS = receive(recieve, ip, port);

	//if network state is not listening to new connections ignore all other ones
	if (Nstate != NetworkState::LISTENING)
	{
		if ((port != remote_port && port) || (ip != remote_ip && ip != sf::IpAddress::None))
		{
			std::cout << "\n recieved data from foreign source";
			return DEFAULT;
		}
	}

	if (STATUS == sf::Socket::Error)
	{
		std::cout << "\n ERROR: failed recieve ";
		return DEFAULT;
	}
	else if (STATUS == Status::Done)
	{
		//if recieved a packet unlabel it
		std::pair<int, CommandID> label = Unlabel(recieve);

		//ignore old packets
		if (TimeLastRecieved < label.first)
		{
			TimeLastRecieved = label.first;
		}
		else
		{
			return DEFAULT; // recieved old packet stop here
		}

		// dont process state change commands if state change call was issued to prevent jumping between states erratically

		//if (label.second & INPUT)
		//{
		//	Nstate = NetworkState::READY;
		//	std::cout << "\n RC INPT ";
		//
		//}

		//if changed state recently ignore chnage state calls;

		if (StateChangeCall)
		{
			if (clock.getElapsedTime().asMilliseconds()-StateChangeCallTime > ping)
				StateChangeCall = false;
		}
		else
		{

			if (label.second & CommandID::SYSTEM_PAUSE)
			{
				if (Gstate->getCurrentState() != State::PAUSE)
				{
					std::cout << "\nchanged state to pause";
					aud->getMusic()->pause();
					Gstate->setCurrentState(State::PAUSE);
				}
			}

			if (label.second & CommandID::SYSTEM_LEVEL)
			{
				if (Gstate->getCurrentState() != State::LEVEL)
				{
					std::cout << "\nchanged state to level";
					aud->stopAllMusic();
					Gstate->setCurrentState(State::LEVEL);
				}
			}

			if (label.second & CommandID::SYSTEM_MENU)
			{
				if (Gstate->getCurrentState() != State::MENU)
				{
					std::cout << "\nchanged state to menu";
					aud->stopAllMusic();
					Gstate->setCurrentState(State::MENU);
				}
			}

		}

		// recieved ping send pong
		if (label.second & CommandID::PING)
		{
			Nstate = NetworkState::READY; // recieved ping from client ready up
			Pong();
			return PONG;
		}

		//client is aware of server and sends out a ping
		if (label.second & CommandID::PONG)
		{
			Ping(true);
			return PING;
		}

		//if recieved a connection process  it

		if (label.second & CommandID::SYSTEM_CONNECT_CLIENT) {
			Server = true;

			remote_ip = ip;
			remote_port = port;

			inputBoxIP->inString = remote_ip.toString();
			inputBoxPORT->inString = std::to_string(remote_port);

			std::cout << "\n recieved call to be a host, initialized as a server";
		}
		else if (label.second & CommandID::SYSTEM_CONNECT_SERVER)
		{
			Server = false;
			CollidableObjects->CanSpawn = false;

			remote_ip = inputBoxIP->inString;
			remote_port = std::stoi(inputBoxPORT->inString);

			std::cout << "\n recieved response from host, initialized as a client";

			Nstate = NetworkState::READY; // completed handshake start sending ping
		}

		return label.second;
		//else std::cout << "\n Failed Read Packet";
		//Nstate = NetworkState::INACTIVE;
	}
	return DEFAULT;
}

void Network::update(float dt)
{
	//recieved packet
	sf::Packet recieve;
	recieve.clear();
	CommandID recievedInfo;
	//listen to incoming connections
	recievedInfo = ListenConnections(recieve);

	//packet for sending;

	sf::Packet packet;
	int16_t commands = 0;
	
	//flag if state has changed
	if (lastExecutedState != Gstate->getCurrentState())
	{
		StateChangeCall = true;
		StateChangeCallTime = clock.getElapsedTime().asMilliseconds();
	}

	switch (Gstate->getCurrentState())
	{
	case State::MENU:
		commands = commands | SYSTEM_MENU;

		if (Nstate == NetworkState::INACTIVE) Nstate = NetworkState::LISTENING;

		if (ping > 0)
		{
			//Adjust network player speed just a bit to accomodate lag
			float NetplayerSp = NetworkPlayer->WalkSpeed;
			NetplayerSp = 1.2 + (float)ping / 1000; // 1.2 is default speed
			NetworkPlayer->WalkSpeed = NetplayerSp;
		}

		if (timerPing.getElapsedTime().asSeconds() > 1) //evry s 
		{
			//restart timer
			timerPing.restart();

			//attempt handshake with using known inputs
			
			if (Nstate == NetworkState::LISTENING)
			{
				Connect();
			}
			else if (Nstate == NetworkState::READY)
			{
				Ping();
			}
			

			if(Nstate == NetworkState::READY)
			{
				NetworkStatus.setString("NETWORK STATUS: YES.");
			}
			else
			{
				NetworkStatus.setString("NETWORK STATUS: NO.");
			}

			

			Pack((CommandID)commands, packet);
			send(packet, remote_ip, remote_port);
		}

		//if (recievedInfo & INPUT)
		//	std::cout << " \n still recieved input ";

		Unpack(recieve, recievedInfo, &CollidableObjects->ALIVE, NetworkPlayer, networkInpt);


		lastExecutedState = State::MENU;

		break;

	case State::LEVEL:
		commands = commands | SYSTEM_LEVEL;

		if (Nstate != NetworkState::READY)
		{
			networkInpt->setMouseX(-100);
			Nstate = NetworkState::INACTIVE;
		}
		else
		{
			if (timerPing.getElapsedTime().asSeconds() > 1)
			{
				timerPing.restart();
				Ping();
			}

			if (timer.getElapsedTime().asMilliseconds() > 30)
			{
				timer.restart();

				switch (Server)
				{
				case true:
					commands = commands | INPUT | COLLIDABLE | SCORE;
					break;
				case false:
					commands = commands | INPUT;
					break;
				}



				Pack((CommandID)commands, packet);
				send(packet, remote_ip, remote_port);
			}

			if (recievedInfo)
			{
				Unpack(recieve, recievedInfo, &CollidableObjects->ALIVE, NetworkPlayer, networkInpt);
			}
		}
		lastExecutedState = State::LEVEL;
		break;

	case State::PAUSE:
		commands = commands | SYSTEM_PAUSE;

		if (Nstate != NetworkState::READY) Nstate = NetworkState::INACTIVE;
		else
		{
			if (timerPing.getElapsedTime().asSeconds() > 1)
			{
				timerPing.restart();
				Ping();
			}


			if (timer.getElapsedTime().asMilliseconds() > 15)
			{
				timer.restart();
				Pack((CommandID)commands, packet);
				send(packet, remote_ip, remote_port);
			}
		}
		lastExecutedState = State::PAUSE;
		break;


		//FIXME doesnt work as only sent once...
	case State::RESTART:
		commands = commands | SYSTEM_RESTART;

		Pack((CommandID)commands, packet);
		send(packet, remote_ip, remote_port);

		lastExecutedState = State::RESTART;
		break;

	default:
		break;
	}
}
