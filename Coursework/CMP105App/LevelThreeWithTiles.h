#pragma once
#include "Scene.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "Coin.h"
#include "Flag.h"
#include <algorithm>
#include <vector>
#include <string>

class LevelThreeWithTiles :
    public Scene
{
public:
    LevelThreeWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:
    void updateCameraAndBackground();
    void checkAndSetMessages();
    void setSpawnFromFirstPlatform();

    TileMap m_tilemap;
    TileMap m_bgtilemap;
    Player m_player;

    Coin m_coin; // optional coin
    GameObject m_boopBlock;
    Flag m_flag;

    const sf::Vector2i WORLD_SIZE = { 2880, 648 };
    const sf::Vector2i VIEW_SIZE = { 432, 432 };

    sf::Font m_font;
    sf::Text m_alertText;
    sf::Texture m_tileTexture;

    std::vector<GameObject> m_platforms; // explicit platforms for parkour
    sf::Vector2f m_spawnPoint = { 2.f * 72.f, 5.f * 72.f };

    sf::Vector2f m_wallPos = { 11 * 72, 5 * 72 };

    std::vector<std::string> m_promptMessages =
    {
        "It's dark...",
        "Use A/D to move, Space to jump. You can double-jump.",
        "Press F to end the day"
    };

    // Death screen members
    bool m_isDead = false;
    sf::RectangleShape m_deathOverlay;
    sf::Text m_deathText;

    // Cave vision members
    sf::RenderTexture m_darkTexture;
    float m_lightRadius = 56.f; // slightly larger than the 72x72 player
    sf::CircleShape m_lightMask;
};


