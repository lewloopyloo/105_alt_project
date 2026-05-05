#include "GameState.h"

void GameState::setCurrentState(State s)
{
	m_currentState = s;
}

State GameState::getCurrentState()
{
	return m_currentState;
}
