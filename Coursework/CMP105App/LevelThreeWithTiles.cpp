#include "LevelThreeWithTiles.h"
#include <iostream>
#include <SFML/Graphics/RenderTexture.hpp>

LevelThreeWithTiles::LevelThreeWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
    : Scene(window, input, gameState, audio), m_alertText(m_font), m_deathText(m_font), m_lightMask(m_lightRadius)
{
    GameObject tile;
    std::vector<GameObject> tileSet;

    int num_columns = 20;
    int num_rows = 9;
    int tile_size = 18;      // Visual size of the tile
    int sheet_spacing = 1;   // Gap between tiles

    tile.setSize(sf::Vector2f(tile_size * 4, tile_size * 4));
    tile.setCollisionBox({ { 0,0 }, tile.getSize() });

    for (int i = 0; i < num_columns * num_rows; i++)
    {
        int row = i / num_columns;
        int col = i % num_columns;
        tile.setTextureRect({
            {(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
            {tile_size, tile_size} });
        if (col <= 4 || col >= 12) tile.setCollider(true);
        else tile.setCollider(false);
        tileSet.push_back(tile);

    }

    tile.setTextureRect({ {0, 0}, {-24, -24} }); // blank tile index
    int b = tileSet.size();
    tile.setCollider(false);
    tileSet.push_back(tile);

    sf::Vector2u mapDimensions{ 40, 8 };

    std::vector<int> tileMap = {
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b
    };

    m_tilemap.loadTexture("gfx/tilemap.png");
    m_tilemap.setTileSet(tileSet);
    m_tilemap.setTileMap(tileMap, mapDimensions);
    m_tilemap.setPosition({ 0, 0 });
    m_tilemap.buildLevel();

    tileSet.clear();

    tile_size = 24;
    num_columns = 8;
    num_rows = 3;
    tile.setSize(sf::Vector2f(tile_size * 9, tile_size * 9));

    for (int i = 0; i < num_columns * num_rows; i++)
    {
        int row = i / num_columns;
        int col = i % num_columns;

        tile.setTextureRect({
            {(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
            {tile_size, tile_size} });
        tile.setCollider(false);        // don't collide with background
        tileSet.push_back(tile);
    }

    std::vector<int> bgTileMap = {
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        10,10,10,10,10,10,10,10,10,10,10,10,10,10,
        18,18,18,18,18,18,18,18,18,18,18,18,18,18,
        18,18,18,18,18,18,18,18,18,18,18,18,18,18
    };

    m_bgtilemap.loadTexture("gfx/tilemap-backgrounds.png");
    m_bgtilemap.setTileSet(tileSet);
    m_bgtilemap.setTileMap(bgTileMap, {14,5});
    m_bgtilemap.setPosition({ 0, -200 });
    m_bgtilemap.buildLevel();

    m_player.setInput(&m_input);
    m_player.setEdges(0, WORLD_SIZE.x);
    m_player.setAudio(&m_audio);

    if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
    m_levelCompleteOverlay.configure(m_font,
        static_cast<float>(m_window.getSize().x),
        static_cast<float>(m_window.getSize().y),
        State::LEVELFOUR, true);

    if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "failed to find tile images";

    m_alertText.setPosition({ 30, 10 });
    m_alertText.setCharacterSize(20);
    m_alertText.setFillColor(sf::Color::White);
    m_alertText.setString(m_promptMessages[1]);

    m_deathOverlay.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
    m_deathOverlay.setFillColor(sf::Color(0, 0, 0, 200)); // semi-transparent black

    m_deathText.setFont(m_font);
    m_deathText.setCharacterSize(36);
    m_deathText.setFillColor(sf::Color::White);
    m_deathText.setOutlineColor(sf::Color::Black);
    m_deathText.setOutlineThickness(2.f);
    m_deathText.setString("You Died\n\nPress R to Respawn\nPress Esc to Menu");

    // Cave vignette: full-screen darkness in m_darkTexture; transparent circle + BlendNone punches the hole.
    m_darkTexture = sf::RenderTexture(m_window.getSize());
    m_lightMask.setPointCount(60);
    m_lightMask.setFillColor(sf::Color(0,0,0,0));
    m_lightMask.setOrigin({ m_lightRadius, m_lightRadius });

    auto applyTilePiece = [&](GameObject &obj, int tileCol, int tileRow)
    {
        obj.setTexture(&m_tileTexture);
        obj.setTextureRect({ { tileCol * 19, tileRow * 19 }, { 18, 18 } }); // atlas stride 19 (18px tile + 1px gap)
    };

    m_platforms.clear();
    const float tileWorldSize = 72.f; // each tile is rendered at 72x72
    struct PlatformChunk
    {
        int col;
        int row;
        int width;
    };

    const std::vector<PlatformChunk> chunks =
    {
        {2, 6, 3},
        {6, 5, 2},

        {11, 6, 2},
        {15, 4, 2},
        {20, 5, 2},
        {24, 3, 2},

        {28, 6, 2},
        {32, 4, 2},
        {36, 3, 2}
    };

    for (const auto& chunk : chunks)
    {
        for (int i = 0; i < chunk.width; ++i)
        {
            GameObject platformTile;
            platformTile.setSize({ tileWorldSize, tileWorldSize });
            platformTile.setPosition({
                static_cast<float>(chunk.col + i) * tileWorldSize,
                static_cast<float>(chunk.row) * tileWorldSize
                });
            platformTile.setCollisionBox({ {0,0}, platformTile.getSize() });
            platformTile.setCollider(true);
            applyTilePiece(platformTile, 2, 1);
            m_platforms.push_back(platformTile);
        }
    }

    m_flag.setSize({72, 72});
    m_flag.setTexture(&m_tileTexture);
    m_flag.setPosition({ 36 * 72.f, 2 * 72.f });
    m_flag.setup();

    setSpawnFromFirstPlatform();
    m_player.setPosition(m_spawnPoint);
}

void LevelThreeWithTiles::onBegin()
{
    m_isDead = false;
    m_levelComplete = false;
    m_player.setCanDoubleJump(false); // level 3 is single-jump only

    setSpawnFromFirstPlatform();
    m_player.setPosition(m_spawnPoint);

    m_audio.playMusicbyName("bgm3");
}

void LevelThreeWithTiles::onEnd()
{
    m_audio.stopAllSounds();
    m_audio.stopAllMusic();
}

void LevelThreeWithTiles::handleInput(float dt)
{
    if (m_isDead)
    {
        if (m_input.isPressed(sf::Keyboard::Scancode::R))
        {
            setSpawnFromFirstPlatform();
            m_player.setPosition(m_spawnPoint);
            m_player.setVelocity({ 0.f, 0.f });
            m_player.setCanDoubleJump(false);
            m_input.setKeyUp(sf::Keyboard::Scancode::R);
            m_isDead = false;
            updateCameraAndBackground();
        }
        else if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
            m_gameState.setCurrentState(State::MENU);
        return;
    }

    if (m_levelComplete)
    {
        m_levelCompleteOverlay.handleInput(m_input, m_gameState);
        if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
            m_gameState.setCurrentState(State::MENU);
        return;
    }

    m_player.handleInput(dt);

    if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
    {
        m_gameState.setCurrentState(State::MENU);
        return;
    }

    if ((m_flag.getPosition() - m_player.getPosition()).length() < 90 &&
        m_input.isPressed(sf::Keyboard::Scancode::F))
        m_levelComplete = true;
}

void LevelThreeWithTiles::update(float dt)
{
    if (m_isDead) return;

    if (m_levelComplete)
    {
        m_levelCompleteOverlay.updateHover(m_input);
        return;
    }

    m_player.update(dt);

    std::vector<GameObject>& level = *m_tilemap.getLevel();
    for (auto& t : level)
    {
        if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
            m_player.collisionResponse(t);
    }

    for (auto& p : m_platforms)
    {
        if (p.isCollider() && Collision::checkBoundingBox(m_player, p))
            m_player.collisionResponse(p);
    }

    checkAndSetMessages();

    if (m_player.getPosition().y > 1200)
    {
        m_isDead = true;
        m_audio.playSoundbyName("death");
    }

    updateCameraAndBackground();
}

void LevelThreeWithTiles::updateCameraAndBackground()
{
    auto view = m_window.getView();
    auto player_pos = m_player.getPosition() + m_player.getSize() * 0.5f;

    float halfViewWidth = VIEW_SIZE.x / 2.0f;
    float halfViewHeight = VIEW_SIZE.y / 2.0f;

    player_pos.x = std::clamp(player_pos.x, halfViewWidth, WORLD_SIZE.x - halfViewWidth);
    player_pos.y = std::clamp(player_pos.y, halfViewHeight, WORLD_SIZE.y - halfViewHeight);

    view.setCenter(player_pos);
    m_window.setView(view);

    m_bgtilemap.setPosition({ player_pos.x - halfViewWidth, 0 });

    m_deathOverlay.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
    m_darkTexture.resize(m_window.getSize());
}

void LevelThreeWithTiles::render()
{
    beginDraw();
    m_bgtilemap.render(m_window);
    m_tilemap.render(m_window);

    for (auto& p : m_platforms) m_window.draw(p);

    m_window.draw(m_player);
    m_window.draw(m_flag);
    if (!m_levelComplete)
        m_window.draw(m_alertText);

    m_darkTexture.clear(sf::Color::Transparent);

    sf::RectangleShape darkRect;
    darkRect.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
    darkRect.setFillColor(sf::Color(0, 0, 0, 245));
    darkRect.setPosition({0.f, 0.f});
    m_darkTexture.draw(darkRect);

    auto viewCenter = m_window.getView().getCenter();
    auto viewSize = m_window.getView().getSize();
    sf::Vector2f topLeft = viewCenter - viewSize * 0.5f;
    sf::Vector2f playerCenter = m_player.getPosition() + m_player.getSize() * 0.5f;
    sf::Vector2f lightPos = playerCenter - topLeft;
    m_lightMask.setPosition(lightPos);
    m_darkTexture.draw(m_lightMask, sf::RenderStates{ sf::BlendNone });
    m_darkTexture.display();

    sf::Sprite darkSprite(m_darkTexture.getTexture());
    darkSprite.setPosition(viewCenter - viewSize * 0.5f); // screen-space; tracks camera
    m_window.draw(darkSprite);

    if (m_isDead)
    {
        auto viewCenter2 = m_window.getView().getCenter();
        auto viewSize2 = m_window.getView().getSize();
        m_deathOverlay.setPosition(viewCenter2 - viewSize2 * 0.5f);

        m_deathText.setPosition({ viewCenter2.x - m_deathText.getGlobalBounds().size.x * 0.5f,
            viewCenter2.y - m_deathText.getGlobalBounds().size.y * 0.5f });

        m_window.draw(m_deathOverlay);
        m_window.draw(m_deathText);
    }

    if (m_levelComplete && !m_isDead)
    {
        m_window.setView(m_window.getDefaultView());
        m_levelCompleteOverlay.render(m_window);
    }

    endDraw();
}

void LevelThreeWithTiles::checkAndSetMessages()
{
    sf::Vector2f inner_top_left = m_window.getView().getCenter();
    sf::Vector2f window_size = {
        static_cast<float>(m_window.getSize().x),
        static_cast<float>(m_window.getSize().y) };
    inner_top_left -= window_size * 0.25f;

    if ((m_flag.getPosition() - m_player.getPosition()).length() < 120)
    {
        m_alertText.setCharacterSize(24);
        m_alertText.setPosition(inner_top_left);
        m_alertText.setString(m_promptMessages[2]);
    }
    else
    {
        m_alertText.setCharacterSize(20);
        m_alertText.setPosition({30.f, 10.f});
        m_alertText.setString(m_promptMessages[0]);
    }
}

void LevelThreeWithTiles::setSpawnFromFirstPlatform()
{
    if (!m_platforms.empty())
    {
        const auto firstBlockPos = m_platforms.front().getPosition();
        const float centeredX = firstBlockPos.x + (m_platforms.front().getSize().x - m_player.getSize().x) * 0.5f;
        const float stableY = firstBlockPos.y - m_player.getSize().y + 1.f; // 1px overlap: reliable landing next frame
        m_spawnPoint = { centeredX, stableY };
    }
    else
    {
        m_spawnPoint = { 2.f * 72.f + 8.f, 5.f * 72.f };
    }
}
