#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "Lever.h"
#include "Flag.h"
#include <algorithm>

class LevelWithTiles :
    public Scene
{
public:
    LevelWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:
    void updateCameraAndBackground();

    TileMap m_tilemap;
    TileMap m_bgtilemap;
    Player m_player;
    sf::Texture m_tileTexture;
    Lever m_lever;
    sf::Font m_font;          // ensure font constructed before text
    sf::Text m_alertText;
    std::vector<Flag*> m_flags;
    bool m_flagLeverPulled = false;
    float m_promptTimer;

    const float PROMPT_TIME = 2.f;
    const sf::Vector2i WORLD_SIZE = { 2880, 648 };
    const sf::Vector2i VIEW_SIZE = { 432, 432 };

    // Death screen members
    bool m_isDead = false;
    sf::RectangleShape m_deathOverlay;
    sf::Text m_deathText;

};

