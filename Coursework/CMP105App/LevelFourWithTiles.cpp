#include "LevelFourWithTiles.h"
#include <algorithm>
#include <iostream>

LevelFourWithTiles::LevelFourWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
    : Scene(window, input, gameState, audio), m_label(m_font), m_sequenceText(m_font), m_flagText(m_font)
{
    m_player.setInput(&m_input);
    m_player.setAudio(&m_audio);
    m_player.setEdges(0.f, m_worldRight);

    m_ground.setSize({ m_worldRight, 72.f });
    m_ground.setPosition({ 0.f, 360.f });
    m_ground.setCollisionBox({ {0.f, 0.f}, m_ground.getSize() });
    m_ground.setCollider(true);
    m_ground.setFillColor(sf::Color(60, 120, 60));

    // Puzzle chamber walls.
    m_leftWall.setSize({ 28.f, 240.f });
    m_leftWall.setPosition({ 190.f, 120.f });
    m_leftWall.setCollisionBox({ {0.f, 0.f}, m_leftWall.getSize() });
    m_leftWall.setCollider(true);
    m_leftWall.setFillColor(sf::Color(95, 95, 110));

    m_roof.setSize({ 520.f, 28.f });
    m_roof.setPosition({ 190.f, 120.f });
    m_roof.setCollisionBox({ {0.f, 0.f}, m_roof.getSize() });
    m_roof.setCollider(true);
    m_roof.setFillColor(sf::Color(95, 95, 110));

    // Door blocks access to the flag corridor until puzzle is solved.
    m_door.setSize({ 28.f, 180.f });
    m_door.setPosition({ 690.f, 180.f });
    m_door.setCollisionBox({ {0.f, 0.f}, m_door.getSize() });
    m_door.setCollider(true);
    m_door.setFillColor(sf::Color(140, 80, 40));

    if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "failed to find tile images";
    m_flag.setTexture(&m_tileTexture);
    m_flag.setSize({ 72.f, 72.f });
    m_flag.setPosition({ 760.f, 288.f });
    m_flag.setup();

    m_pads = {
        { GameObject(), sf::Color(220, 70, 70), "RED" },
        { GameObject(), sf::Color(70, 120, 240), "BLUE" },
        { GameObject(), sf::Color(70, 200, 110), "GREEN" },
        { GameObject(), sf::Color(240, 210, 80), "YELLOW" }
    };

    const std::array<sf::Vector2f, 4> padPositions = {
        sf::Vector2f(260.f, 300.f),
        sf::Vector2f(360.f, 300.f),
        sf::Vector2f(460.f, 300.f),
        sf::Vector2f(560.f, 300.f)
    };

    for (size_t i = 0; i < m_pads.size(); ++i)
    {
        m_pads[i].tile.setSize({ 64.f, 64.f });
        m_pads[i].tile.setPosition(padPositions[i]);
        m_pads[i].tile.setFillColor(m_pads[i].activeColor);
        m_pads[i].tile.setCollisionBox({ {0.f, 0.f}, m_pads[i].tile.getSize() });
    }

    if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
    m_label.setCharacterSize(20);
    m_label.setFillColor(sf::Color::White);
    m_label.setString("Level 4 - Color Puzzle Chamber\nPress F near a color tile");
    m_label.setPosition({ 20.f, 20.f });

    m_sequenceText.setCharacterSize(18);
    m_sequenceText.setFillColor(sf::Color::White);
    m_sequenceText.setPosition({ 20.f, 68.f });

    m_flagText.setCharacterSize(18);
    m_flagText.setPosition({ 20.f, 100.f });

    resetPuzzle();
    m_player.setPosition(m_spawn);
}

void LevelFourWithTiles::onBegin()
{
    resetPuzzle();
    m_player.setPosition(m_spawn);
    m_player.setVelocity({ 0.f, 0.f });
    updateCamera();
}

void LevelFourWithTiles::onEnd()
{
    m_audio.stopAllSounds();
}

void LevelFourWithTiles::handleInput(float dt)
{
    m_player.handleInput(dt);

    if (m_input.isPressed(sf::Keyboard::Scancode::F))
    {
        if (m_doorOpen && (m_flag.getPosition() - m_player.getPosition()).length() < 100.f)
        {
            m_gameState.setCurrentState(State::MENU);
            return;
        }
        activateNearestPad();
    }

    if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
    {
        m_gameState.setCurrentState(State::MENU);
    }
}

void LevelFourWithTiles::update(float dt)
{
    m_player.update(dt);
    m_flag.update(dt);

    if (Collision::checkBoundingBox(m_player, m_ground))
    {
        m_player.collisionResponse(m_ground);
    }
    if (Collision::checkBoundingBox(m_player, m_leftWall))
    {
        m_player.collisionResponse(m_leftWall);
    }
    if (Collision::checkBoundingBox(m_player, m_roof))
    {
        m_player.collisionResponse(m_roof);
    }
    if (!m_doorOpen && Collision::checkBoundingBox(m_player, m_door))
    {
        m_player.collisionResponse(m_door);
    }

    if (m_player.getPosition().y > 1200.f)
    {
        m_player.setPosition(m_spawn);
        m_player.setVelocity({ 0.f, 0.f });
    }

    if (m_showFeedback && m_feedbackClock.getElapsedTime().asSeconds() > 1.1f)
    {
        m_showFeedback = false;
        updatePuzzleText();
    }

    updateCamera();
}

void LevelFourWithTiles::render()
{
    beginDraw();
    m_window.draw(m_ground);
    m_window.draw(m_leftWall);
    m_window.draw(m_roof);
    if (!m_doorOpen) m_window.draw(m_door);
    for (auto& pad : m_pads) m_window.draw(pad.tile);
    m_window.draw(m_flag);
    m_window.draw(m_player);

    const auto viewCenter = m_window.getView().getCenter();
    const auto viewSize = m_window.getView().getSize();
    const sf::Vector2f topLeft = viewCenter - viewSize * 0.5f;
    const float left = topLeft.x + 16.f;
    float y = topLeft.y + 12.f;
    const float lineGap = 10.f;

    m_label.setPosition({ left, y });
    y += m_label.getGlobalBounds().size.y + lineGap;

    m_sequenceText.setPosition({ left, y });
    y += m_sequenceText.getGlobalBounds().size.y + lineGap;

    m_flagText.setPosition({ left, y });

    m_window.draw(m_label);
    m_window.draw(m_sequenceText);
    m_window.draw(m_flagText);
    endDraw();
}

void LevelFourWithTiles::resetPuzzle()
{
    m_enteredSequence.clear();
    m_doorOpen = false;
    m_showFeedback = false;
    m_feedbackMessage.clear();
    m_flagText.setFillColor(sf::Color(230, 80, 80));
    m_flagText.setString("Door: LOCKED");
    m_door.setCollider(true);
    m_door.setFillColor(sf::Color(140, 80, 40));
    updatePuzzleText();
}

void LevelFourWithTiles::activateNearestPad()
{
    if (m_doorOpen) return;

    int nearestIndex = -1;
    float nearestDistSq = m_padInteractDistance * m_padInteractDistance;
    const sf::Vector2f playerCenter = m_player.getPosition() + m_player.getSize() * 0.5f;

    for (size_t i = 0; i < m_pads.size(); ++i)
    {
        const sf::Vector2f padCenter = m_pads[i].tile.getPosition() + m_pads[i].tile.getSize() * 0.5f;
        const float distSq = (padCenter - playerCenter).lengthSquared();
        if (distSq < nearestDistSq)
        {
            nearestDistSq = distSq;
            nearestIndex = static_cast<int>(i);
        }
    }

    if (nearestIndex < 0) return;

    if (m_enteredSequence.size() >= m_correctSequence.size()) return;

    m_enteredSequence.push_back(nearestIndex);
    updatePuzzleText();

    if (m_enteredSequence.size() < m_correctSequence.size()) return;

    bool match = true;
    for (size_t i = 0; i < m_correctSequence.size(); ++i)
    {
        if (m_enteredSequence[i] != m_correctSequence[i])
        {
            match = false;
            break;
        }
    }

    if (!match)
    {
        resetPuzzle();
        m_feedbackMessage = "Wrong code. Puzzle reset.";
        m_feedbackColor = sf::Color(240, 90, 90);
        m_feedbackClock.restart();
        m_showFeedback = true;
        updatePuzzleText();
        return;
    }

    m_doorOpen = true;
    m_feedbackMessage = "Correct sequence! Door opened.";
    m_feedbackColor = sf::Color(110, 240, 120);
    m_feedbackClock.restart();
    m_showFeedback = true;
    m_flagText.setFillColor(sf::Color(100, 230, 120));
    m_flagText.setString("Door: OPEN - Reach flag and press F");
    m_door.setCollider(false);
    m_door.setFillColor(sf::Color(100, 190, 120, 120));

    updatePuzzleText();
}

void LevelFourWithTiles::updatePuzzleText()
{
    std::string text = "Sequence: ";
    if (m_enteredSequence.empty()) text += "(none)";
    else
    {
        for (size_t i = 0; i < m_enteredSequence.size(); ++i)
        {
            text += m_pads[m_enteredSequence[i]].name;
            if (i + 1 < m_enteredSequence.size()) text += " -> ";
        }
    }

    if (m_showFeedback)
    {
        text += "\n";
        text += m_feedbackMessage;
    }
    else
    {
        text += "\nHint: sky, ember, sun, leaf.";
    }

    m_sequenceText.setString(text);
    m_sequenceText.setFillColor(m_showFeedback ? m_feedbackColor : sf::Color::White);
}

void LevelFourWithTiles::updateCamera()
{
    auto view = m_window.getView();
    const float halfW = m_viewSize.x * 0.5f;
    const float halfH = m_viewSize.y * 0.5f;
    sf::Vector2f playerCenter = m_player.getPosition() + m_player.getSize() * 0.5f;
    playerCenter.x = std::clamp(playerCenter.x, halfW, m_worldRight - halfW);
    playerCenter.y = halfH;
    view.setSize(m_viewSize);
    view.setCenter(playerCenter);
    m_window.setView(view);
}
