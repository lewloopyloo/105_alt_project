#include "LevelFourWithTiles.h"
#include <iostream>

LevelFourWithTiles::LevelFourWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
    : Scene(window, input, gameState, audio), m_label(m_font)
{
    m_player.setInput(&m_input);
    m_player.setAudio(&m_audio);
    m_player.setEdges(0.f, m_worldRight);

    m_ground.setSize({ m_worldRight, 72.f });
    m_ground.setPosition({ 0.f, 360.f });
    m_ground.setCollisionBox({ {0.f, 0.f}, m_ground.getSize() });
    m_ground.setCollider(true);
    m_ground.setFillColor(sf::Color(60, 120, 60));

    if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
    m_label.setCharacterSize(20);
    m_label.setFillColor(sf::Color::White);
    m_label.setString("Level 4 - Empty Layout\nPress Esc to return to menu");
    m_label.setPosition({ 20.f, 20.f });

    m_player.setPosition(m_spawn);
}

void LevelFourWithTiles::onBegin()
{
    m_player.setPosition(m_spawn);
    m_player.setVelocity({ 0.f, 0.f });
}

void LevelFourWithTiles::onEnd()
{
    m_audio.stopAllSounds();
}

void LevelFourWithTiles::handleInput(float dt)
{
    m_player.handleInput(dt);
    if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
    {
        m_gameState.setCurrentState(State::MENU);
    }
}

void LevelFourWithTiles::update(float dt)
{
    m_player.update(dt);

    if (Collision::checkBoundingBox(m_player, m_ground))
    {
        m_player.collisionResponse(m_ground);
    }

    if (m_player.getPosition().y > 1200.f)
    {
        m_player.setPosition(m_spawn);
        m_player.setVelocity({ 0.f, 0.f });
    }
}

void LevelFourWithTiles::render()
{
    beginDraw();
    m_window.draw(m_ground);
    m_window.draw(m_player);
    m_window.draw(m_label);
    endDraw();
}
