#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Player.h"
#include "Flag.h"
#include <array>
#include <string>
#include <vector>

class LevelFourWithTiles : public Scene
{
public:
    LevelFourWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:
    struct PuzzlePad
    {
        GameObject tile;
        sf::Color activeColor;
        std::string name;
    };

    void resetPuzzle();
    void activateNearestPad();
    void updatePuzzleText();
    void updateCamera();

    Player m_player;
    GameObject m_ground;
    GameObject m_leftWall;
    GameObject m_roof;
    GameObject m_door;
    Flag m_flag;
    sf::Texture m_tileTexture;
    std::vector<PuzzlePad> m_pads;
    std::vector<int> m_enteredSequence;
    std::array<int, 4> m_correctSequence = { 1, 0, 3, 2 }; // blue, red, yellow, green
    bool m_doorOpen = false;
    bool m_showFeedback = false;
    sf::Clock m_feedbackClock;
    sf::Color m_feedbackColor = sf::Color::White;
    std::string m_feedbackMessage;
    sf::Font m_font;
    sf::Text m_label;
    sf::Text m_sequenceText;
    sf::Text m_flagText;

    const sf::Vector2f m_spawn = { 240.f, 220.f };
    const float m_worldRight = 2880.f;
    const float m_padInteractDistance = 110.f;
    const sf::Vector2f m_viewSize = { 432.f, 432.f };
};
