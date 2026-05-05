// Game State Class
// Simple game state controller. enum lists possible game states (can be modified), tracks and updates the current state
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#pragma once

enum class CharacterId { DINO = 0, NINJA = 1, ROBOT = 2 };

// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU, LEVELONE, LEVELTWO, LEVELTHREE, LEVELFOUR, PAUSE, CREDITS };

class GameState
{
public:
	// Set the current state
	void setCurrentState(State s);
	// Returns the current state.
	State getCurrentState();
	void setSelectedCharacter(CharacterId id);
	CharacterId getSelectedCharacter() const;

protected:
	State m_currentState;
	CharacterId m_selectedCharacter = CharacterId::DINO;
};