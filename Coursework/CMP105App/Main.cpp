// Entry point: window, scenes, main loop (poll events → dt → scene → clear input).
// @author Paul Robertson
// @reviser William Kavanagh (2025)

#include <iostream>
#include "Scene.h"
#include "Menu.h"
#include "LevelThatSaves.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "LevelWithTiles.h"
#include "LevelTwoWithTiles.h"
#include "LevelThreeWithTiles.h"
#include "LevelFourWithTiles.h"

#ifndef SFML_VERSION_MAJOR
	#error "SFML 3 is required for this framework."
#endif


void windowProcess(sf::RenderWindow& window, Input& in)
{
	while (const auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		if (const auto* resized = event->getIf<sf::Event::Resized>())
		{
			sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(resized->size));
			window.setView(sf::View(visibleArea));
		}
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			in.setKeyDown(keyPressed->scancode);
		}
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			in.setKeyUp(keyReleased->scancode);
		}
		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				in.setLeftMouse(Input::MouseState::PRESSED);
			}
			else
			{
				in.setRightMouse(Input::MouseState::PRESSED);
			}
		}
		if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseReleased->button == sf::Mouse::Button::Left)
			{
				in.setLeftMouse(Input::MouseState::UP);
			}
			else
			{
				in.setRightMouse(Input::MouseState::UP);
			}
		}
		if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			in.setMousePosition(mouseMoved->position.x, mouseMoved->position.y);
		}
		/*
		* There are other events you may wish to poll for, such as:
		* controller or joystick input, scrolling mouse wheel or focus gained/lost
		*/
	}
}


int main()
{

	sf::RenderWindow window(sf::VideoMode({ 432, 432 }), "Dino Handyman");
	window.setVerticalSyncEnabled(true);

	AudioManager audioManager;
	Input input;
	GameState gameState;


	Menu menu(window, input, gameState, audioManager);
	LevelWithTiles tile_level(window, input, gameState, audioManager);
	LevelTwoWithTiles tile_level_two(window, input, gameState, audioManager);
	LevelThreeWithTiles tile_level_three(window, input, gameState, audioManager);
	LevelFourWithTiles tile_level_four(window, input, gameState, audioManager);
	Scene* currentScene = &menu;

	sf::Clock clock;
	float deltaTime = 0.f;

	gameState.setCurrentState(State::MENU);
	menu.onBegin();

	std::map<State, Scene*> sceneRegistry =
	{
		{State::MENU, &menu},
		{State::LEVELONE, &tile_level},
		{State::LEVELTWO, &tile_level_two},
		{State::LEVELTHREE, &tile_level_three},
		{State::LEVELFOUR, &tile_level_four}
	};
	
	while (window.isOpen())
	{
		windowProcess(window, input);

		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 0.1f) deltaTime = 0.1f; // avoid huge dt after a hitch

		State requestedState = gameState.getCurrentState();
		if (sceneRegistry[requestedState] != currentScene)
		{
			currentScene->onEnd();
			currentScene = sceneRegistry[requestedState];
			currentScene->onBegin();
		}
		currentScene->handleInput(deltaTime);
		currentScene->update(deltaTime);
		currentScene->render();

		input.update(); // after scenes so "pressed this frame" clears correctly
	}

}
