#pragma once

#include "Framework/Collision.h"
#include "Framework/GameObject.h"
#include "Framework/GameState.h"
#include "Framework/Input.h"
#include <SFML/Graphics.hpp>

class LevelCompleteOverlay
{
public:
    void configure(sf::Font& font, float windowW, float windowH, State nextLevelState, bool hasNextLevel);

    void handleInput(Input& input, GameState& gameState);
    void updateHover(Input& input);
    void render(sf::RenderWindow& window);

private:
    static void centerLabelOnButton(sf::Text& label, const GameObject& button);

    sf::Font* m_font = nullptr;
    bool m_hasNext = true;
    State m_nextState = State::LEVELTWO;

    GameObject m_dim;
    GameObject m_panel;
    GameObject m_nextButton;
    GameObject m_menuButton;

    sf::Text m_title;
    sf::Text m_subtitle;
    sf::Text m_nextLabel;
    sf::Text m_menuLabel;

    sf::Color m_btnIdle = sf::Color(228, 242, 232, 238);
    sf::Color m_btnHover = sf::Color(195, 235, 208, 255);
};
