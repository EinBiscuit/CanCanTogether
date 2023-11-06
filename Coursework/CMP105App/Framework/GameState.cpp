#include "GameState.h"

// Set the current state
void GameState::setCurrentState(State s)
{
	lastState = currentState;
	currentState = s;
}

// Returns the current state.
State GameState::getCurrentState()
{
	return currentState;
}

State GameState::getLastState()
{
	return lastState;
}
