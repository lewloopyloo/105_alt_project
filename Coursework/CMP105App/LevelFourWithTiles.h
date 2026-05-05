#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Player.h"

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
    Player m_player;
    GameObject m_ground;
    sf::Font m_font;
    sf::Text m_label;

    const sf::Vector2f m_spawn = { 80.f, 220.f };
    const float m_worldRight = 2880.f;
};
