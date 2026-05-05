#include "GameState.h"

// Set the current state
void GameState::setCurrentState(State s)
{
	m_currentState = s;
}

// Returns the current state.
State GameState::getCurrentState()
{
	return m_currentState;
}
