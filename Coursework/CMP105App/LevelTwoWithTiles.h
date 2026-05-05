#pragma once
#include "Scene.h"
#include "LevelCompleteOverlay.h"
#include "Framework/Collision.h"
#include "Framework/TileMap.h"
#include "Player.h"
#include "Coin.h"
#include "Flag.h"
#include <algorithm>


class LevelTwoWithTiles :
    public Scene
{
public:
    LevelTwoWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override;
    void render() override;
    void onBegin() override;
    void onEnd() override;

private:
    void updateCameraAndBackground();
    void checkAndSetMessages();

    TileMap m_tilemap;
    TileMap m_bgtilemap;
    Player m_player;

    Coin m_coin;
    GameObject m_boopBlock;
    Flag m_flag;

    const sf::Vector2i WORLD_SIZE = { 2880, 648 };
    const sf::Vector2i VIEW_SIZE = { 432, 432 };

    sf::Font m_font; // declare before m_alertText / m_deathText (ctor passes m_font into sf::Text)
    sf::Text m_alertText;
    sf::Texture m_tileTexture;

    sf::Vector2f m_wallPos = { 11 * 72, 5 * 72 };


    std::vector<std::string> m_promptMessages =
    {
        "Oh I'm stuck.\nMaybe I missed\nsomething back there?",
        "I feel so powerful\nalmost like I\ncan double-jump",
        "Press F to end the day"
    };

    bool m_isDead = false;
    sf::RectangleShape m_deathOverlay;
    sf::Text m_deathText;

    bool m_levelComplete = false;
    LevelCompleteOverlay m_levelCompleteOverlay;
};

