#include "LevelCompleteOverlay.h"

void LevelCompleteOverlay::centerLabelOnButton(sf::Text& label, const GameObject& button)
{
    const sf::Vector2f bp = button.getPosition();
    const sf::Vector2f bs = button.getSize();
    const sf::FloatRect lb = label.getLocalBounds();
    label.setPosition({
        bp.x + (bs.x - lb.size.x) * 0.5f - lb.position.x,
        bp.y + (bs.y - lb.size.y) * 0.5f - lb.position.y });
}

void LevelCompleteOverlay::configure(sf::Font& font, float windowW, float windowH, State nextLevelState, bool hasNextLevel)
{
    m_font = &font;
    m_hasNext = hasNextLevel;
    m_nextState = nextLevelState;

    m_dim.setSize({ windowW, windowH });
    m_dim.setPosition({ 0.f, 0.f });
    m_dim.setFillColor(sf::Color(0, 0, 0, 165));

    const float panelW = 328.f;
    const float panelH = m_hasNext ? 248.f : 198.f;
    const float panelX = (windowW - panelW) * 0.5f;
    const float panelY = (windowH - panelH) * 0.5f;

    m_panel.setSize({ panelW, panelH });
    m_panel.setPosition({ panelX, panelY });
    m_panel.setFillColor(sf::Color(22, 28, 42, 235));
    m_panel.setOutlineThickness(2.f);
    m_panel.setOutlineColor(sf::Color(255, 255, 255, 55));

    const float btnW = 248.f;
    const float btnH = 58.f;
    const float btnX = panelX + (panelW - btnW) * 0.5f;

    m_title.setFont(font);
    m_title.setString("Level complete!");
    m_title.setCharacterSize(26);
    m_title.setFillColor(sf::Color(255, 246, 225));
    m_title.setOutlineColor(sf::Color(14, 18, 30));
    m_title.setOutlineThickness(3.f);
    {
        sf::FloatRect b = m_title.getLocalBounds();
        m_title.setOrigin({ b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f });
        m_title.setPosition({ windowW * 0.5f, panelY + 42.f });
    }

    m_subtitle.setFont(font);
    m_subtitle.setString(hasNextLevel ? "Choose where to go next." : "You've cleared every job site.");
    m_subtitle.setCharacterSize(14);
    m_subtitle.setFillColor(sf::Color(190, 210, 220));
    m_subtitle.setOutlineColor(sf::Color(12, 16, 26));
    m_subtitle.setOutlineThickness(1.f);
    {
        sf::FloatRect b = m_subtitle.getLocalBounds();
        m_subtitle.setOrigin({ b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f });
        m_subtitle.setPosition({ windowW * 0.5f, panelY + 76.f });
    }

    m_nextLabel.setFont(font);
    m_nextLabel.setString("Next level");
    m_nextLabel.setCharacterSize(18);
    m_nextLabel.setFillColor(sf::Color(26, 44, 34));

    m_menuLabel.setFont(font);
    m_menuLabel.setString("Main menu");
    m_menuLabel.setCharacterSize(18);
    m_menuLabel.setFillColor(sf::Color(26, 44, 34));

    if (m_hasNext)
    {
        const float nextY = panelY + 104.f;
        const float menuY = nextY + btnH + 12.f;

        m_nextButton.setSize({ btnW, btnH });
        m_nextButton.setPosition({ btnX, nextY });
        m_nextButton.setCollisionBox({ { 0.f, 0.f }, { btnW, btnH } });
        m_nextButton.setFillColor(m_btnIdle);
        m_nextButton.setOutlineThickness(2.f);
        m_nextButton.setOutlineColor(sf::Color(255, 255, 255, 70));

        m_menuButton.setSize({ btnW, btnH });
        m_menuButton.setPosition({ btnX, menuY });
        m_menuButton.setCollisionBox({ { 0.f, 0.f }, { btnW, btnH } });
        m_menuButton.setFillColor(m_btnIdle);
        m_menuButton.setOutlineThickness(2.f);
        m_menuButton.setOutlineColor(sf::Color(255, 255, 255, 70));

        centerLabelOnButton(m_nextLabel, m_nextButton);
        centerLabelOnButton(m_menuLabel, m_menuButton);
    }
    else
    {
        m_nextButton.setSize({ 0.f, 0.f });
        m_nextButton.setPosition({ -500.f, -500.f });

        const float menuY = panelY + 118.f;
        m_menuButton.setSize({ btnW, btnH });
        m_menuButton.setPosition({ btnX, menuY });
        m_menuButton.setCollisionBox({ { 0.f, 0.f }, { btnW, btnH } });
        m_menuButton.setFillColor(m_btnIdle);
        m_menuButton.setOutlineThickness(2.f);
        m_menuButton.setOutlineColor(sf::Color(255, 255, 255, 70));

        centerLabelOnButton(m_menuLabel, m_menuButton);
    }
}

void LevelCompleteOverlay::handleInput(Input& input, GameState& gameState)
{
    const sf::Vector2i mousePos{ input.getMouseX(), input.getMouseY() };

    if (!input.isLeftMousePressed())
        return;

    if (m_hasNext && Collision::checkBoundingBox(m_nextButton, mousePos))
    {
        gameState.setCurrentState(m_nextState);
        return;
    }

    if (Collision::checkBoundingBox(m_menuButton, mousePos))
        gameState.setCurrentState(State::MENU);
}

void LevelCompleteOverlay::updateHover(Input& input)
{
    const sf::Vector2i mousePos{ input.getMouseX(), input.getMouseY() };

    if (m_hasNext)
    {
        m_nextButton.setFillColor(
            Collision::checkBoundingBox(m_nextButton, mousePos) ? m_btnHover : m_btnIdle);
    }

    m_menuButton.setFillColor(
        Collision::checkBoundingBox(m_menuButton, mousePos) ? m_btnHover : m_btnIdle);
}

void LevelCompleteOverlay::render(sf::RenderWindow& window)
{
    window.draw(m_dim);
    window.draw(m_panel);
    window.draw(m_title);
    window.draw(m_subtitle);

    if (m_hasNext)
    {
        window.draw(m_nextButton);
        window.draw(m_nextLabel);
    }

    window.draw(m_menuButton);
    window.draw(m_menuLabel);
}
