#include "LevelThreeWithTiles.h"
#include <iostream>
#include <SFML/Graphics/RenderTexture.hpp>

LevelThreeWithTiles::LevelThreeWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
    : Scene(window, input, gameState, audio), m_alertText(m_font), m_deathText(m_font), m_lightMask(m_lightRadius)
{
    // Basic tileset setup (reusing approach from other levels)
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

    // Add Blank
    tile.setTextureRect({ {0, 0}, {-24, -24} }); // Empty rect for blank
    int b = tileSet.size();
    tile.setCollider(false);
    tileSet.push_back(tile);

    sf::Vector2u mapDimensions{ 40, 8 };

    // Create a tileMap similar to Level 1/2 so visuals match
    std::vector<int> tileMap = {
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , 145, b  , b  , 145, b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , 101, 102, 102, 102, 102, 103, b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , 121, 122, 122, 122, 122, 123, b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , 121, 122, 122, 122, 122, 123, b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , 121, 122, 122, 122, 122, 123, b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
        21 , 22 , 22 , 22 , 22 , 22 , 22 , 22 , 22 , 22 , 22 , 22 , 25 , 122, 122, 122, 122, 24 , 22 , 22 , 22 , 23 , b, b, b, b, b, b, b, b, b, 21 , 22 , 22 , 22 , 22 , 22 , 22 , 22 , 23 ,
        141, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 143, b, b, b, b, b, b, b, b, b, 141, 142, 142, 142, 142, 142, 142, 142, 143
    };

    m_tilemap.loadTexture("gfx/tilemap.png");
    m_tilemap.setTileSet(tileSet);
    m_tilemap.setTileMap(tileMap, mapDimensions);
    m_tilemap.setPosition({ 0, 0 });
    m_tilemap.buildLevel();

    tileSet.clear();

    // setup background (reuse small bg tiles)
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

    // background tilemap (same structure as other levels)
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

    // setup player
    m_player.setInput(&m_input);
    m_player.setEdges(0, WORLD_SIZE.x);
    m_player.setAudio(&m_audio);

    if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";

    if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "failed to find tile images";

    m_alertText.setPosition({ 30, 10 });
    m_alertText.setCharacterSize(20);
    m_alertText.setFillColor(sf::Color::White);
    m_alertText.setString(m_promptMessages[1]);

    // Death overlay initialisation
    m_deathOverlay.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
    m_deathOverlay.setFillColor(sf::Color(0, 0, 0, 200)); // semi-transparent black

    m_deathText.setFont(m_font);
    m_deathText.setCharacterSize(36);
    m_deathText.setFillColor(sf::Color::White);
    m_deathText.setOutlineColor(sf::Color::Black);
    m_deathText.setOutlineThickness(2.f);
    m_deathText.setString("You Died\n\nPress R to Respawn\nPress Esc to Menu");

    // Cave vision setup
    m_darkTexture = sf::RenderTexture(m_window.getSize());
    m_lightMask.setPointCount(60);
    // The circle will be drawn with a fully transparent color and BlendNone so it overwrites the darkness
    m_lightMask.setFillColor(sf::Color(0,0,0,0));
    m_lightMask.setOrigin({ m_lightRadius, m_lightRadius });

    // Helper to assign a specific tile piece from the tile atlas to a GameObject
    auto applyTilePiece = [&](GameObject &obj, int tileCol, int tileRow)
    {
        obj.setTexture(&m_tileTexture);
        // source tile size 18 and spacing 1 -> pitch 19
        obj.setTextureRect({ { tileCol * 19, tileRow * 19 }, { 18, 18 } });
    };

    // Build stairs to replace the snow section (tile-sized steps)
    m_platforms.clear();
    const float tileWorldSize = 72.f; // each tile is rendered at 72x72
    int startCol = 12;
    int steps = 6; // number of steps
    for (int i = 0; i < steps; ++i)
    {
        int col = startCol + i;
        int row = 6 - i; // step upwards from row 6 towards row 1
        if (row < 1) row = 1;
        GameObject step;
        step.setSize({ tileWorldSize, tileWorldSize });
        step.setPosition({ static_cast<float>(col) * tileWorldSize, static_cast<float>(row) * tileWorldSize });
        step.setCollisionBox({ {0,0}, step.getSize() });
        step.setCollider(true);
        // use a ground-like tile piece
        applyTilePiece(step, 1, 6);
        m_platforms.push_back(step);
    }

    // place flag on the tilemap where other levels place it
    m_flag.setSize({72, 72});
    m_flag.setTexture(&m_tileTexture);
    m_flag.setPosition({ 36 * 72.f, 5 * 72.f });
    m_flag.setup();

    // Set player start to be on the first stair (if present) or safe fallback
    if (!m_platforms.empty())
    {
        auto& first = m_platforms.front();
        m_player.setPosition({ first.getPosition().x + 10.f, first.getPosition().y - m_player.getSize().y });
    }
    else
    {
        m_player.setPosition({ 100.f, 100.f });
    }
}

void LevelThreeWithTiles::onBegin()
{
    // ensure player does NOT carry double-jump into level 3
    m_isDead = false;
    m_player.setCanDoubleJump(false);

    // place player at first stair
    if (!m_platforms.empty())
    {
        auto& first = m_platforms.front();
        m_player.setPosition({ first.getPosition().x + 10.f, first.getPosition().y - m_player.getSize().y });
    }
    else
    {
        m_player.setPosition({ 100.f, 100.f });
    }

    m_audio.playMusicbyName("bgm3");
}

void LevelThreeWithTiles::onEnd()
{
    m_audio.stopAllSounds();
    m_audio.stopAllMusic();
}

void LevelThreeWithTiles::handleInput(float dt)
{
    if (!m_isDead)
        m_player.handleInput(dt);

    // if I press F on the flag  / I press escape.
    if (!m_isDead &&
        ((m_flag.getPosition() - m_player.getPosition()).length() < 90 &&
         m_input.isPressed(sf::Keyboard::Scancode::F)) )
    {
        // return to menu.
        m_gameState.setCurrentState(State::MENU);
    }

    if (m_isDead)
    {
        if (m_input.isPressed(sf::Keyboard::Scancode::R))
        {
            // Respawn on the first stair if available
            if (!m_platforms.empty())
            {
                auto& first = m_platforms.front();
                m_player.setPosition({ first.getPosition().x + 10.f, first.getPosition().y - m_player.getSize().y });
                m_player.setVelocity({ 0.f, 0.f });
            }
            else
            {
                m_player.setPosition({ 100.f, 100.f });
                m_player.setVelocity({ 0.f, 0.f });
            }
            m_player.setCanDoubleJump(false);
            m_isDead = false;
            updateCameraAndBackground();
        }
        else if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
        {
            m_gameState.setCurrentState(State::MENU);
        }
    }
}

void LevelThreeWithTiles::update(float dt)
{
    if (m_isDead) return;

    m_player.update(dt);

    // handle collisions with tilemap
    std::vector<GameObject>& level = *m_tilemap.getLevel();
    for (auto& t : level)
    {
        if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
            m_player.collisionResponse(t);
    }

    // handle collisions with stair GameObjects
    for (auto& p : m_platforms)
    {
        if (p.isCollider() && Collision::checkBoundingBox(m_player, p))
            m_player.collisionResponse(p);
    }

    // interaction with flag: allow pressing F (handled in handleInput) and also show prompts
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

    // update render texture size in case the window changed
    m_deathOverlay.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
    m_darkTexture.resize(m_window.getSize());
}

void LevelThreeWithTiles::render()
{
    beginDraw();
    m_bgtilemap.render(m_window);
    m_tilemap.render(m_window);

    // draw stair GameObjects before player so player stands on top
    for (auto& p : m_platforms) m_window.draw(p);

    m_window.draw(m_player);
    m_window.draw(m_flag);
    m_window.draw(m_alertText);

    // Build and draw darkness mask with a circular light around player
    m_darkTexture.clear(sf::Color::Transparent);

    sf::RectangleShape darkRect;
    darkRect.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
    darkRect.setFillColor(sf::Color(0, 0, 0, 220));
    darkRect.setPosition({0.f, 0.f});
    m_darkTexture.draw(darkRect);

    // compute player position in screen space (relative to view)
    auto viewCenter = m_window.getView().getCenter();
    auto viewSize = m_window.getView().getSize();
    sf::Vector2f topLeft = viewCenter - viewSize * 0.5f;
    sf::Vector2f playerCenter = m_player.getPosition() + m_player.getSize() * 0.5f;
    sf::Vector2f lightPos = playerCenter - topLeft;

    m_lightMask.setPosition(lightPos);
    // draw the transparent circle using BlendNone so it overwrites the dark rect
    m_darkTexture.draw(m_lightMask, sf::RenderStates{ sf::BlendNone });
    m_darkTexture.display();

    sf::Sprite darkSprite(m_darkTexture.getTexture());
    m_window.draw(darkSprite);

    // Death overlay drawn on top when player is dead
    if (m_isDead)
    {
        // ensure overlay covers current view
        auto viewCenter2 = m_window.getView().getCenter();
        auto viewSize2 = m_window.getView().getSize();
        m_deathOverlay.setPosition(viewCenter2 - viewSize2 * 0.5f);

        // center text in view
        m_deathText.setPosition({ viewCenter2.x - m_deathText.getGlobalBounds().size.x * 0.5f,
            viewCenter2.y - m_deathText.getGlobalBounds().size.y * 0.5f });

        m_window.draw(m_deathOverlay);
        m_window.draw(m_deathText);
    }

    endDraw();
}

void LevelThreeWithTiles::checkAndSetMessages()
{
    // show contextual prompt when near the flag
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
        // keep reminding the player it's dark
        m_alertText.setCharacterSize(20);
        m_alertText.setPosition({30.f, 10.f});
        m_alertText.setString(m_promptMessages[0]);
    }
}
