#include "LevelTwoWithTiles.h"

LevelTwoWithTiles::LevelTwoWithTiles(sf::RenderWindow& window, Input& input, GameState& gameState, AudioManager& audio)
	: Scene(window, input, gameState, audio), m_alertText(m_font), m_deathText(m_font)
{
	GameObject tile;
	std::vector<GameObject> tileSet;

	int num_columns = 20;
	int num_rows = 9;
	int tile_size = 18;      // Visual size of the tile
	int sheet_spacing = 1;   // Gap between tiles

	// Set GameObject size (Scaling up 4x for visibility)
	// 4 * 18 = 3 * 24 = 72 (dino size is 24).
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
		b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b  , b, b, b, b, b, b, b, b, b, b  , b  , b  , b  , b  , b  , b  , b  , b  ,
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

	tile_size = 24;
	num_columns = 8;
	num_rows = 3;
	// 24 * 9 = 216, a multiple of 72, the LCM of the player and tile size.
	tile.setSize(sf::Vector2f(tile_size * 9, tile_size * 9));

	for (int i = 0; i < num_columns * num_rows; i++)
	{
		int row = i / num_columns;
		int col = i % num_columns;

		tile.setTextureRect({
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},
			{tile_size, tile_size} });
		tile.setCollider(false); 		// don't collide with background
		tileSet.push_back(tile);
	}

	mapDimensions = { 14,5 };
	tileMap = {
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		10,10,10,10,10,10,10,10,10,10,10,10,10,10,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,18,18,18,18,18,18,18,18,18,18,18,18,18
	};

	m_bgtilemap.loadTexture("gfx/tilemap-backgrounds.png");
	m_bgtilemap.setTileSet(tileSet);
	m_bgtilemap.setTileMap(tileMap, mapDimensions);
	m_bgtilemap.setPosition({ 0, -200 });
	m_bgtilemap.buildLevel();

	m_player.setInput(&m_input);
	m_player.setEdges(0, WORLD_SIZE.x);
	m_player.setPosition({ 100, 100 });
	m_player.setAudio(&m_audio);

	if (!m_font.openFromFile("font/bitcount.ttf")) std::cerr << "no font found";
	m_levelCompleteOverlay.configure(m_font,
		static_cast<float>(m_window.getSize().x),
		static_cast<float>(m_window.getSize().y),
		State::LEVELTHREE, true);

	sf::Vector2f boop_location = { 100, 72 * 2 + 100 };

	if (!m_tileTexture.loadFromFile("gfx/tilemap.png")) std::cerr << "failed to find tile images";
	m_coin.setTexture(&m_tileTexture);
	m_coin.setPosition(boop_location);
	m_coin.setSize({ 72,72 });
	m_coin.setAlive(false);
	m_coin.setCollisionBox({ { 0,0 }, { 72,72 } });

	m_boopBlock.setTexture(&m_tileTexture);
	m_boopBlock.setTextureRect({ {10 * 19,0}, { 18,18 } });
	m_boopBlock.setAlive(false);
	m_boopBlock.setCollisionBox({ { 0,0 }, { 72,72 } });
	m_boopBlock.setPosition(boop_location);
	m_boopBlock.setSize({ 72, 72 });

	m_flag.setPosition({ 38 * 72, 5 * 72 });
	m_flag.setSize({ 72,72 });
	m_flag.setTexture(&m_tileTexture);

	m_alertText.setPosition({ 50, 150 });
	m_alertText.setCharacterSize(36);
	m_alertText.setFillColor(sf::Color::Black);

	m_deathOverlay.setSize({ static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y) });
	m_deathOverlay.setFillColor(sf::Color(0, 0, 0, 180)); // semi-transparent black

	m_deathText.setFont(m_font);
	m_deathText.setCharacterSize(36);
	m_deathText.setFillColor(sf::Color::White);
	m_deathText.setOutlineColor(sf::Color::Black);
	m_deathText.setOutlineThickness(2.f);
	m_deathText.setString("You Died\n\nPress R to Respawn\nPress Esc to Menu");

}

void LevelTwoWithTiles::onBegin()
{
	m_boopBlock.setAlive(false);
	m_coin.setAlive(false);
	m_player.setPosition({ 100, 100 });
	m_player.setCanDoubleJump(false);
	m_isDead = false;
	m_levelComplete = false;
	m_audio.playMusicbyName("bgm3");
}

void LevelTwoWithTiles::onEnd()
{
	m_player.setCanDoubleJump(false);
	m_audio.stopAllSounds();
	m_audio.stopAllMusic();
}

void LevelTwoWithTiles::handleInput(float dt)
{
	if (m_isDead)
	{
		if (m_input.isPressed(sf::Keyboard::Scancode::R))
		{
			m_player.reset();
			m_player.setCanDoubleJump(false);
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

	if ((m_flag.getPosition() - m_player.getPosition()).length() < 75 &&
		m_input.isPressed(sf::Keyboard::Scancode::F))
		m_levelComplete = true;
}

void LevelTwoWithTiles::update(float dt)
{
	if (m_isDead)
		return;

	if (m_levelComplete)
	{
		m_levelCompleteOverlay.updateHover(m_input);
		return;
	}

	m_player.update(dt);
	m_flag.update(dt);
	if (m_coin.isAlive()) m_coin.update(dt);

	std::vector<GameObject>& level = *m_tilemap.getLevel();
	for (auto& t : level)
	{
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t))
		{
			m_player.collisionResponse(t);
		}	
	}

	if (m_boopBlock.isAlive())
	{
		if (Collision::checkBoundingBox(m_player, m_boopBlock))
		{
			if (m_player.getPosition().y >= m_boopBlock.getPosition().y)
			{
				m_boopBlock.setAlive(false);
				m_coin.setAlive(true);
			}
			m_player.collisionResponse(m_boopBlock);

		}
	}

	if (m_coin.isAlive())
	{
		if (Collision::checkBoundingBox(m_player, m_coin))
		{
			m_coin.setAlive(false);
			m_player.setCanDoubleJump(true);
		}
	}

	if ((m_wallPos - m_player.getPosition()).length() < 75)
	{
		m_boopBlock.setAlive(true);
	}

	if (m_player.getPosition().y > 1200)
	{
		m_isDead = true;
		m_audio.playSoundbyName("death");
	}

	updateCameraAndBackground();
	checkAndSetMessages();
}

void LevelTwoWithTiles::updateCameraAndBackground()
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
}

void LevelTwoWithTiles::checkAndSetMessages()
{
	sf::Vector2f inner_top_left = m_window.getView().getCenter();
	sf::Vector2f window_size = { 
		static_cast<float>(m_window.getSize().x), 
		static_cast<float>(m_window.getSize().y) };
	inner_top_left -= window_size * 0.25f;
	if (!m_player.canDoubleJump() && 
		(m_wallPos - m_player.getPosition()).length() < 75)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(inner_top_left);
		m_alertText.setString(m_promptMessages[0]);
		
	}
	else if (m_player.canDoubleJump() && 
		(m_boopBlock.getPosition() - m_player.getPosition()).length() < 150)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(inner_top_left);
		m_alertText.setString(m_promptMessages[1]);
	}

	else if ((m_flag.getPosition() - m_player.getPosition()).length() < 75)
	{
		m_alertText.setCharacterSize(24);
		m_alertText.setPosition(inner_top_left);
		m_alertText.setString(m_promptMessages[2]);
	}
	else
	{
		m_alertText.setPosition({ 0,-100 });
	}
}

void LevelTwoWithTiles::render()
{
	beginDraw();
	m_bgtilemap.render(m_window);
	m_tilemap.render(m_window);
	if (m_boopBlock.isAlive()) m_window.draw(m_boopBlock);
	m_window.draw(m_flag);
	m_window.draw(m_player);
	if (m_coin.isAlive()) m_window.draw(m_coin);
	if (!m_levelComplete)
		m_window.draw(m_alertText);

	if (m_isDead)
	{
		auto viewCenter = m_window.getView().getCenter();
		auto viewSize = m_window.getView().getSize();
		m_deathOverlay.setPosition(viewCenter - viewSize * 0.5f);

		m_deathText.setPosition({ viewCenter.x - m_deathText.getGlobalBounds().size.x * 0.5f,
			viewCenter.y - m_deathText.getGlobalBounds().size.y * 0.5f });

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