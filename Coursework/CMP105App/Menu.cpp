#include "Menu.h"

Menu::Menu(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud) :
	Scene(hwnd, in, gs, aud), m_playButtonLabel(m_font), m_playButton2Label(m_font), m_playButton3Label(m_font), m_exitButtonLabel(m_font)
{
	if (!m_font.openFromFile("font/bitcount.ttf"))
		std::cerr << "failed to load bitcount font";

	m_playButtonLabel.setCharacterSize(24); 		// setup labels
	m_playButtonLabel.setPosition({ 185,70 });
	m_playButtonLabel.setString("Level 1");
	m_playButtonLabel.setFillColor(sf::Color::Black);

	m_playButton2Label.setCharacterSize(24);
	m_playButton2Label.setPosition({ 185,170 });
	m_playButton2Label.setString("Level 2");
	m_playButton2Label.setFillColor(sf::Color::Black);

	m_playButton3Label.setCharacterSize(24);
	m_playButton3Label.setPosition({ 185,270 });
	m_playButton3Label.setString("Level 3");
	m_playButton3Label.setFillColor(sf::Color::Black);

	// Exit button label
	m_exitButtonLabel.setCharacterSize(24);
	m_exitButtonLabel.setPosition({ 185,370 });
	m_exitButtonLabel.setString("Exit");
	m_exitButtonLabel.setFillColor(sf::Color::Black);


	m_playButton.setSize({ 216,80 }); 			// setup buttons (reduced height to fit 4 buttons)
	m_playButton.setPosition({ 108,40 });
	m_playButton.setCollisionBox({ {0,0}, m_playButton.getSize()});
	m_playButton.setFillColor(m_defaultButtonColour); 

	m_play2Button.setSize({ 216,80 });
	m_play2Button.setPosition({ 108,140 });
	m_play2Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_play2Button.setFillColor(m_defaultButtonColour);

	m_play3Button.setSize({ 216,80 });
	m_play3Button.setPosition({ 108,240 });
	m_play3Button.setCollisionBox({ {0,0}, m_playButton.getSize() });
	m_play3Button.setFillColor(m_defaultButtonColour);

	// Exit button (placed under the third button)
	m_exitButton.setSize({ 216,80 });
	m_exitButton.setPosition({ 108,340 });
	m_exitButton.setCollisionBox({ {0,0}, m_exitButton.getSize() });
	m_exitButton.setFillColor(m_defaultButtonColour);

	if (!m_titleSplash.loadFromFile("gfx/title_splash.png")) std::cerr << "no splash found";
	m_titleImage.setTexture(&m_titleSplash);
	m_titleImage.setSize({ 432,432 });
}

void Menu::handleInput(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY()};

	if(m_input.isLeftMousePressed() && 
		Collision::checkBoundingBox(m_playButton, mousePos))
	{
		m_gameState.setCurrentState(State::LEVELONE);
	}
	if (m_input.isLeftMousePressed() &&
		Collision::checkBoundingBox(m_play2Button, mousePos))
	{
		m_gameState.setCurrentState(State::LEVELTWO);
	}
	if (m_input.isLeftMousePressed() &&
		Collision::checkBoundingBox(m_play3Button, mousePos))
	{
		m_gameState.setCurrentState(State::LEVELTHREE);
	}
	// Exit action: close the render window
	if (m_input.isLeftMousePressed() &&
		Collision::checkBoundingBox(m_exitButton, mousePos))
	{
		m_window.close();
	}
}

void Menu::render()
{
	beginDraw();
	m_window.draw(m_titleImage);
	m_window.draw(m_playButton);
	m_window.draw(m_playButtonLabel);
	m_window.draw(m_play2Button);
	m_window.draw(m_playButton2Label);
	m_window.draw(m_play3Button);
	m_window.draw(m_playButton3Label);
	m_window.draw(m_exitButton);         // draw exit button
	m_window.draw(m_exitButtonLabel);    // draw exit label
	endDraw();
}

void Menu::update(float dt)
{
	sf::Vector2i mousePos{ m_input.getMouseX(), m_input.getMouseY() };
	if (Collision::checkBoundingBox(m_playButton, mousePos))
	{
		m_playButton.setFillColor(m_hoverButtonColour);
	}
	else
	{
		m_playButton.setFillColor(m_defaultButtonColour);
	}
	if (Collision::checkBoundingBox(m_play2Button, mousePos))
	{
		m_play2Button.setFillColor(m_hoverButtonColour);
	}
	else
	{
		m_play2Button.setFillColor(m_defaultButtonColour);

	}
	// hover for third button
	if (Collision::checkBoundingBox(m_play3Button, mousePos))
	{
		m_play3Button.setFillColor(m_hoverButtonColour);
	}
	else
	{
		m_play3Button.setFillColor(m_defaultButtonColour);
	}
	// hover for exit button
	if (Collision::checkBoundingBox(m_exitButton, mousePos))
	{
		m_exitButton.setFillColor(m_hoverButtonColour);
	}
	else
	{
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