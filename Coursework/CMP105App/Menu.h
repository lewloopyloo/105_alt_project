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
    GameObject m_play4Button; // level 4 button

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
    sf::Text m_playButton4Label; // label for level 4
    sf::Text m_exitButtonLabel;        // exit label

    // label for the single Level Select button
    sf::Text m_levelSelectLabel;

    // overlay title
    sf::Text m_overlayTitle;

    sf::VertexArray m_backdropGradient;
    sf::RectangleShape m_menuCard;
    sf::Text m_mainTitle;
    sf::Text m_tagline;

    GameObject m_dinoLogo;
    sf::Texture m_dinoLogoTexture;
    bool m_hasDinoLogo = false;

    // state
    bool m_showLevelMenu = false;

    sf::Color m_defaultButtonColour = sf::Color(245, 252, 248, 238);
    sf::Color m_hoverButtonColour = sf::Color(210, 242, 222, 255);
};
