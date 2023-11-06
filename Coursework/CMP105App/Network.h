#pragma once1
#include "SFML/Network.hpp"
#include "InputField.h"
#include "Framework/GameState.h"
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Framework/GameState.h"
#include "Collidable_Manager.h"
#include "Collidable.h"
#include "Player.h"
#include <list>
#include <string>

//PACKET DATA HEADER

enum CommandID //arranged for bitmasking operations 
{
	DEFAULT					= 0,
	SYSTEM_PAUSE			= 0b1,
	SYSTEM_LEVEL			= 0b10,
	SYSTEM_RESTART			= 0b100,
	SYSTEM_MENU				= 0b1000,
	INPUT					= 0b10000,
	PLAYER					= 0b100000,
	COLLIDABLE				= 0b1000000,
	SCORE					= 0b10000000,
	SYSTEM_DISCONNECT		= 0b100000000,
	SYSTEM_CONNECT_SERVER	= 0b1000000000,
	SYSTEM_CONNECT_CLIENT	= 0b10000000000,
	PING					= 0b100000000000,
	PONG					= 0b1000000000000,
	
	// total 13
};

//DATA STRUCTS

struct Bread_pack //20 bytes						between 0 and 10 are sent each game -> 200 bytes for bread alone
{
	uint8_t number; //1
	sf::Vector2f position; //8
	sf::Vector2f velocity; //8
	int8_t rotation; //1
};

struct Player_pack //11 bytes 
{
	float positionX; //4
	float velocityX; //4
	int8_t kicking_state; //1
	uint8_t kicker_leg; //1
	uint8_t stager; //1
};

struct Input_pack // 6 bytes
{
	uint32_t positionX; //4
	uint8_t rightC; // 1
	uint8_t leftC; // 1
};

//SFML PACKET STREAM OPERATOR EXTENSIONS FOR CUSTOM STRUCTURES

//bread pack packaging

static sf::Packet& operator <<(sf::Packet& packet, const Bread_pack& m)
{
	return packet << m.number << m.position.x << m.position.y << m.velocity.x << m.velocity.y << m.rotation;
}

static sf::Packet& operator >> (sf::Packet& packet, Bread_pack& m)
{
	return packet >> m.number >> m.position.x >> m.position.y >> m.velocity.x >> m.velocity.y >> m.rotation;
}

//player pack packaging // havent been used as syncing input alone was way more efficient and yielded near exact results

static sf::Packet& operator <<(sf::Packet& packet, const Player_pack& m)
{
	return packet << m.positionX << m.velocityX << (int8_t)m.kicking_state << m.kicker_leg << m.stager;
}

static sf::Packet& operator >> (sf::Packet& packet, Player_pack& m)
{
	return packet >> m.positionX >> m.velocityX >>(int8_t&)m.kicking_state >> m.kicker_leg >> m.stager;
}

// input pack packaging

static sf::Packet& operator <<(sf::Packet& packet, const Input_pack& m)
{
	return packet << m.positionX << m.rightC << m.leftC;
}

static sf::Packet& operator >> (sf::Packet& packet, Input_pack& m)
{
	return packet >> m.positionX >> m.rightC >> m.leftC;
}

//NETWORK HEADER

enum class NetworkState
{
	INACTIVE,  // DEFAULT STATE IF NO PARTNER FOUND
	LISTENING, // LOOKING FOR PARTNERS FOR CAN CAN
	READY,     // READY IN STANDBY TO PLAY
	ACTIVE,    // PLAYING GAME
};

class Network : sf::UdpSocket
{

protected:

	// if server else client;
	bool Server; 

	//pointers to game controll

	NetworkState Nstate = NetworkState::LISTENING;
	GameState* Gstate;

	AudioManager* aud;
	sf::RenderWindow* wnd;
	Input* inpt;
	Input* networkInpt;

	// to determine if state change happened recently
	State lastExecutedState = State::MENU;
	bool StateChangeCall = false;

	//timers for sending data at equal isha mounts of time

	sf::Clock timer;
	sf::Clock timerPing;

	//clock to monitor how long the game has been active

	sf::Clock clock;

	//filtering packets by time
	int TimeLastRecieved;
	// state change time to wait
	int StateChangeCallTime;

	int ping;
	int time_ping_sent;
	
	//pointer to collision resolution system as well as non player dynamic object 
	Collidable_Manager* CollidableObjects;

	//player pointers
	Player* NetworkPlayer;
	Player* LocalPlayer;

	//NETWORK MENU

	sf::Text NetworkInfo;
	sf::Text NetworkStatus; 
	InputField* inputBoxIP;
	InputField* inputBoxPORT;

	//Connection properties

	sf::IpAddress remote_ip;
	uint16_t remote_port;

	//FUNctions--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	int Pack(CommandID ThingsToPack, sf::Packet& packet); // put data in a packet return time

	std::pair<int, CommandID> Unlabel(sf::Packet& packet); // get time and header

	void Unpack(sf::Packet& packet, CommandID label, std::list<Collidable*>* tempDynamic, Player* tempPawn, Input* tempInput);  // retrieve data from a to later interpolate it

	bool Connect();

	//listen connections from connected port client and process system commands// pass on packet for unpacking if 
	CommandID ListenConnections(sf::Packet& recieve);

	// ping takes whether or not it got pong to calculate time
	void Ping(bool pongd = false);
	void Pong();

public:

	Network();
	~Network();

	//Initialize network instance - > bind socket, set up input fields, save all the necessary elements of the game state controll // INITIALIZES IP ADDRESS WITH LOCALHOST BY DEFAULT (for debug convenience)
	void Init(Collidable_Manager* collidables, Player* PlayerPawn, Player* NetworkPawn, sf::RenderWindow* window, sf::Font* font, Input* ipt, GameState* gamest, AudioManager* aud);

	// handshake menu on top of whatever menu
	void Network_Menu(char inputTXT);

	// get input from network
	Input* getNetworkInput() { return networkInpt; }; 

	void update(float dt);

};

