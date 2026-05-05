#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
class Menu :
    public Scene
{
public:
    Menu(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:

    // Level buttons (inside overlay)
    GameObject m_playButton;
    GameObject m_play2Button;
    GameObject m_play3Button; // level 3 button

    // Main menu buttons
    GameObject m_levelSelectButton; // single Level Select button that opens overlay
    GameObject m_exitButton;           // exit button

    // Overlay visuals: dim background + framed panel
    GameObject m_overlayDim;    // fullscreen dim
    GameObject m_overlayFrame;  // frame / border
    GameObject m_overlayPanel;  // panel for buttons

    sf::Font m_font;
    sf::Text m_playButtonLabel;
    sf::Text m_playButton2Label;
    sf::Text m_playButton3Label; // label for level 3
    sf::Text m_exitButtonLabel;        // exit label

    // label for the single Level Select button
    sf::Text m_levelSelectLabel;

    // overlay title
    sf::Text m_overlayTitle;

    GameObject m_titleImage;
    sf::Texture m_titleSplash;

    // state
    bool m_showLevelMenu = false;

    sf::Color m_defaultButtonColour = sf::Color(100, 250, 100, 50);
    sf::Color m_hoverButtonColour = sf::Color(80, 230, 80, 150);
};

