#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud),
	m_playButtonLabel(m_font),
	m_playButton2Label(m_font),
	m_playButton3Label(m_font),
	m_playButton4Label(m_font),
	m_exitButtonLabel(m_font),
	m_levelSelectLabel(m_font),
	m_overlayTitle(m_font)
{
	if (!m_font.openFromFile("font/bitcount.ttf"))
		std::cerr << "failed to load bitcount font";

	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });

	auto ws = m_window.getSize();
	sf::Vector2f windowSize{ static_cast<float>(ws.x), static_cast<float>(ws.y) };

	const float mainBtnWidth = 216.f;
	const float mainBtnHeight = 80.f;
	const float mainBtnSpacing = 16.f;

	float totalHeight = mainBtnHeight * 2.f + mainBtnSpacing;
	float startY = (windowSize.y - totalHeight) / 2.f;
	float centreX = (windowSize.x - mainBtnWidth) / 2.f;

	m_levelSelectButton.setSize({ mainBtnWidth, mainBtnHeight });
	m_levelSelectButton.setPosition(sf::Vector2f(centreX, startY));
	m_levelSelectButton.setCollisionBox({ {0,0}, m_levelSelectButton.getSize() });
	m_levelSelectButton.setFillColor(m_defaultButtonColour);

	m_levelSelectLabel.setString("Level Select");
	m_levelSelectLabel.setCharacterSize(20);
	m_levelSelectLabel.setFillColor(sf::Color::Black);
	{
		auto btnPos = m_levelSelectButton.getPosition();
		auto btnSize = m_levelSelectButton.getSize();
		sf::FloatRect lblBounds = m_levelSelectLabel.getLocalBounds();
		float lblX = btnPos.x + (btnSize.x - lblBounds.size.x) / 2.f - lblBounds.position.x;
		float lblY = btnPos.y + (btnSize.y - lblBounds.size.y) / 2.f - lblBounds.position.y;
		m_levelSelectLabel.setPosition(sf::Vector2f(lblX, lblY));
	}

	m_exitButton.setSize({ mainBtnWidth, mainBtnHeight });
	m_exitButton.setPosition(sf::Vector2f(centreX, startY + mainBtnHeight + mainBtnSpacing));
	m_exitButton.setCollisionBox({ {0,0}, m_exitButton.getSize() });
	m_exitButton.setFillColor(m_defaultButtonColour);

	m_exitButtonLabel.setString("Exit");
	m_exitButtonLabel.setCharacterSize(20);
	m_exitButtonLabel.setFillColor(sf::Color::Black);
	{
		auto btnPos = m_exitButton.getPosition();
		auto btnSize = m_exitButton.getSize();
		sf::FloatRect lblBounds = m_exitButtonLabel.getLocalBounds();
		float lblX = btnPos.x + (btnSize.x - lblBounds.size.x) / 2.f - lblBounds.position.x;
		float lblY = btnPos.y + (btnSize.y - lblBounds.size.y) / 2.f - lblBounds.position.y;
		m_exitButtonLabel.setPosition(sf::Vector2f(lblX, lblY));
	}

	m_overlayDim.setSize(windowSize);
	m_overlayDim.setPosition({ 0.f, 0.f });
	m_overlayDim.setCollisionBox({ {0,0}, m_overlayDim.getSize() });
	m_overlayDim.setFillColor(sf::Color(0, 0, 0, 140));

	const sf::Vector2f panelSize{ 340.f, 400.f };
	const float framePadding = 8.f;
	sf::Vector2f frameSize = { panelSize.x + framePadding * 2.f, panelSize.y + framePadding * 2.f };
	sf::Vector2f framePos = { (windowSize.x - frameSize.x) / 2.f, (windowSize.y - frameSize.y) / 2.f };
	m_overlayFrame.setSize(frameSize);
	m_overlayFrame.setPosition(framePos);
	m_overlayFrame.setCollisionBox({ {0,0}, m_overlayFrame.getSize() });
	m_overlayFrame.setFillColor(sf::Color(30, 30, 30, 230));

	sf::Vector2f panelPos = { framePos.x + framePadding, framePos.y + framePadding };
	m_overlayPanel.setSize(panelSize);
	m_overlayPanel.setPosition(panelPos);
	m_overlayPanel.setCollisionBox({ {0,0}, m_overlayPanel.getSize() });
	m_overlayPanel.setFillColor(sf::Color(240, 240, 240, 230));

	m_overlayTitle.setCharacterSize(28);
	m_overlayTitle.setString("Select Level");
	sf::FloatRect titleBounds = m_overlayTitle.getLocalBounds();
	float titleX = panelPos.x + (panelSize.x - titleBounds.size.x) / 2.f - titleBounds.position.x;
	m_overlayTitle.setPosition(sf::Vector2f(titleX, panelPos.y + 10.f));
	m_overlayTitle.setFillColor(sf::Color::Black);

	float btnWidth = 260.f;
	float btnHeight = 72.f;
	float btnX = panelPos.x + (panelSize.x - btnWidth) / 2.f;
	float btnYStart = panelPos.y + 60.f;
	float btnSpacing = 86.f;

	m_playButton.setSize({ btnWidth, btnHeight });
	m_playButton.setPosition({ btnX, btnYStart });
	m_playButton.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_playButton.setFillColor(m_defaultButtonColour);

	m_playButtonLabel.setCharacterSize(22);
	m_playButtonLabel.setPosition({ btnX + btnWidth / 2.f - 30.f, btnYStart + btnHeight / 2.f - 12.f });
	m_playButtonLabel.setString("Level 1");
	m_playButtonLabel.setFillColor(sf::Color::Black);

	m_play2Button.setSize({ btnWidth, btnHeight });
	m_play2Button.setPosition({ btnX, btnYStart + btnSpacing });
	m_play2Button.setCollisionBox({ {0,0}, m_play2Button.getSize() });
	m_play2Button.setFillColor(m_defaultButtonColour);

	m_playButton2Label.setCharacterSize(22);
	m_playButton2Label.setPosition({ btnX + btnWidth / 2.f - 30.f, btnYStart + btnSpacing + btnHeight / 2.f - 12.f });
	m_playButton2Label.setString("Level 2");
	m_playButton2Label.setFillColor(sf::Color::Black);

	m_play3Button.setSize({ btnWidth, btnHeight });
	m_play3Button.setPosition({ btnX, btnYStart + btnSpacing * 2.f });
	m_play3Button.setCollisionBox({ {0,0}, m_play3Button.getSize() });
	m_play3Button.setFillColor(m_defaultButtonColour);

	m_playButton3Label.setCharacterSize(22);
	m_playButton3Label.setPosition({ btnX + btnWidth / 2.f - 30.f, btnYStart + btnSpacing * 2.f + btnHeight / 2.f - 12.f });
	m_playButton3Label.setString("Level 3");
	m_playButton3Label.setFillColor(sf::Color::Black);

	m_play4Button.setSize({ btnWidth, btnHeight });
	m_play4Button.setPosition({ btnX, btnYStart + btnSpacing * 3.f });
	m_play4Button.setCollisionBox({ {0,0}, m_play4Button.getSize() });
	m_play4Button.setFillColor(m_defaultButtonColour);

	m_playButton4Label.setCharacterSize(22);
	m_playButton4Label.setPosition({ btnX + btnWidth / 2.f - 30.f, btnYStart + btnSpacing * 3.f + btnHeight / 2.f - 12.f });
	m_playButton4Label.setString("Level 4");
	m_playButton4Label.setFillColor(sf::Color::Black);
}

void Menu::handleInput(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };

	if (m_showLevelMenu)
	{
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_playButton, mousePos))
		{
			m_gameState.setCurrentState(State::LEVELONE);
			m_showLevelMenu = false;
			return;
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_play2Button, mousePos))
		{
			m_gameState.setCurrentState(State::LEVELTWO);
			m_showLevelMenu = false;
			return;
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_play3Button, mousePos))
		{
			m_gameState.setCurrentState(State::LEVELTHREE);
			m_showLevelMenu = false;
			return;
		}
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_play4Button, mousePos))
		{
			m_gameState.setCurrentState(State::LEVELFOUR);
			m_showLevelMenu = false;
			return;
		}

		if (m_input.isLeftMousePressed() &&
			!Collision::checkBoundingBox(m_overlayPanel, mousePos))
		{
			m_showLevelMenu = false;
			return;
		}
	}
	else
	{
		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_levelSelectButton, mousePos))
		{
			m_showLevelMenu = true;
			return;
		}

		if (m_input.isLeftMousePressed() &&
			Collision::checkBoundingBox(m_exitButton, mousePos))
		{
			m_window.close();
			return;
		}
	}
}

void Menu::render()
{
	beginDraw();
	m_window.draw(m_titleImage);

	if (m_showLevelMenu)
	{
		m_window.draw(m_overlayDim);
		m_window.draw(m_overlayFrame);
		m_window.draw(m_overlayPanel);
		m_window.draw(m_overlayTitle);

		m_window.draw(m_playButton);
		m_window.draw(m_playButtonLabel);

		m_window.draw(m_play2Button);
		m_window.draw(m_playButton2Label);

		m_window.draw(m_play3Button);
		m_window.draw(m_playButton3Label);

		m_window.draw(m_play4Button);
		m_window.draw(m_playButton4Label);
	}
	else
	{
		m_window.draw(m_levelSelectButton);
		m_window.draw(m_levelSelectLabel);

		m_window.draw(m_exitButton);
		m_window.draw(m_exitButtonLabel);
	}

	endDraw();
}

void Menu::update(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };

	if (m_showLevelMenu)
	{
		if (Collision::checkBoundingBox(m_playButton, mousePos))
			m_playButton.setFillColor(m_hoverButtonColour);
		else
			m_playButton.setFillColor(m_defaultButtonColour);

		if (Collision::checkBoundingBox(m_play2Button, mousePos))
			m_play2Button.setFillColor(m_hoverButtonColour);
		else
			m_play2Button.setFillColor(m_defaultButtonColour);

		if (Collision::checkBoundingBox(m_play3Button, mousePos))
			m_play3Button.setFillColor(m_hoverButtonColour);
		else
			m_play3Button.setFillColor(m_defaultButtonColour);

		if (Collision::checkBoundingBox(m_play4Button, mousePos))
			m_play4Button.setFillColor(m_hoverButtonColour);
		else
			m_play4Button.setFillColor(m_defaultButtonColour);
	}
	else
	{
		if (Collision::checkBoundingBox(m_levelSelectButton, mousePos))
			m_levelSelectButton.setFillColor(m_hoverButtonColour);
		else
			m_levelSelectButton.setFillColor(m_defaultButtonColour);

		if (Collision::checkBoundingBox(m_exitButton, mousePos))
			m_exitButton.setFillColor(m_hoverButtonColour);
		else
			m_exitButton.setFillColor(m_defaultButtonColour);
	}
}

void Menu::onBegin()
{
	std::cout << "starting menu\n";
	auto view = m_window.getDefaultView();
	view.setCenter({ 216, 216 });
	m_window.setView(view);
	m_audio.playMusicbyName("bgm2");
}

void Menu::onEnd()
{
	std::cout << "leaving menu\n";
	m_audio.stopAllMusic();
}