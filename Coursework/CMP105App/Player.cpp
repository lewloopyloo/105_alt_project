#include "Player.h"

Player::Player()
{
	if (!m_dinoTexture.loadFromFile("gfx/dino1.png"))
		std::cerr << "No dino texture. sad";

	setTexture(&m_dinoTexture);
	// Dino is 24x24, tiles are 18x18
	// LCM(18,24) = 72.
	setSize({ 72,72 });		
	setPosition({ 24, 100 });

	for (int i = 0; i < 4; i++)
		m_idle.addFrame({{ i * 24, 0 }, { 24, 24} });
	for (int i = 4; i < 10; ++i)
		m_walk.addFrame({{ i * 24, 0 }, { 24, 24}});
	for(int i = 16; i < 24; i++)
		m_sprint.addFrame({ { i * 24, 0 }, { 24, 24} });

	m_currAnim = &m_walk;
	m_walk.setFrameSpeed(1.f / 10.f);
	m_idle.setFrameSpeed(1.f / 4.f);
	m_sprint.setFrameSpeed(1.4 / 15.0f);

	setCollisionBox({ {12,12}, { 45,51 } });

	m_isGrounded = false;
}

void Player::handleInput(float dt)
{
	m_accel = { 0,0 };

	if (m_input->isKeyDown(sf::Keyboard::Scancode::A))
		m_accel.x -= SPEED;
	if (m_input->isKeyDown(sf::Keyboard::Scancode::D))
		m_accel.x += SPEED;
	if (m_input->isPressed(sf::Keyboard::Scancode::Space) && m_isGrounded)
	{
		m_velocity.y = - JUMP_FORCE;
		m_isGrounded = false;
		m_audio->playSoundbyName("jump");
	}
	else if (m_input->isPressed(sf::Keyboard::Scancode::Space) && !m_isGrounded && m_canDoubleJump && !m_hasDoubleJumped)
	{
		m_velocity.y = - JUMP_FORCE;
		m_hasDoubleJumped = true;
		m_audio->playSoundbyName("jump");
	}
	if (m_input->isPressed(sf::Keyboard::Scancode::LControl) && m_sprintTimer <= 0)
	{
		if (!m_currAnim->getFlipped())
			m_velocity.x = SPEED * SPRINT_SPEED_MULT;
		else
			m_velocity.x = -SPEED * SPRINT_SPEED_MULT;
		m_sprintTimer = SPRINT_COOLDOWN;
	}
	if (m_input->isPressed(sf::Keyboard::Scancode::F))
	{
		if (inLeverRange() && !m_leverPulled)
		{
			m_leverPulled = true;
			m_audio->playSoundbyName("wind");
		}
		if (m_leverPulled && inEndRange())
		{
			m_gameEndTriggered = true;
		}
	}

	// for debugging: "Where am I?"
	if (m_input->isPressed(sf::Keyboard::Scancode::T)) // dev: print world position
		std::cout << getPosition().x << "/" << getPosition().y << "\n";

}

void Player::update(float dt)
{
	m_accel.y += GRAVITY;
	m_velocity += dt * m_accel;
	if (m_isGrounded && abs(m_accel.x) < 1.f) m_velocity *= DRAG_FACTOR;
	else if (!m_isGrounded) m_velocity *= AIR_DRAG_FACTOR;
	else if (m_accel.x * m_velocity.x < 0) m_velocity *= TURN_DRAG;

	m_isGrounded = false;

	if (m_sprintTimer > 0) m_sprintTimer -= dt;

	float speed = std::abs(m_velocity.x);
	if (speed < 1.0)
		m_currAnim = &m_idle;
	else if (speed > SPRINT_ANIM_THRESHOLD)
		m_currAnim = &m_sprint;
	else
		m_currAnim = &m_walk;

	if (m_velocity.x > 0 && m_currAnim->getFlipped()
		|| m_velocity.x < 0 && !m_currAnim->getFlipped())
		m_currAnim->setFlipped(!m_currAnim->getFlipped());	

	move(m_velocity);

	if (getPosition().x < m_leftEdge)
	{
		setPosition({ m_leftEdge, getPosition().y });
	}
	if (getPosition().x > m_rightEdge - getSize().x)
	{
		setPosition({ m_rightEdge - getSize().x, getPosition().y});
	}

	m_currAnim->animate(dt);
	setTextureRect(m_currAnim->getCurrentFrame());
}

// Tile colliders only (see GameObject::isCollider).
void Player::collisionResponse(GameObject& collider)
{
	sf::FloatRect playerCollider = getCollisionBox();
	sf::FloatRect wallBounds = collider.getCollisionBox();
	auto overlap = playerCollider.findIntersection(wallBounds);
	if (!overlap) return;
	if (overlap->size.x < overlap->size.y)
	{
		// Narrow overlap on X → resolve horizontally (wall slide).
		if (playerCollider.position.x < wallBounds.position.x)
			move({ -overlap->size.x, 0 });
		else
			move({ overlap->size.x, 0 });
	}
	else
	{
		if (playerCollider.position.y < wallBounds.position.y)
		{
			move({ 0, -overlap->size.y });
			m_velocity.y = 0;
			m_isGrounded = true;
			m_hasDoubleJumped = false;
		}
		else
		{
			move({ 0, overlap->size.y });
			m_velocity.y = 0;
		}
	}
}

bool Player::inLeverRange()
{
	return (getPosition() - m_leverPosition).lengthSquared() < ACTIVATE_RANGE_SQUARED;
}

bool Player::inEndRange()
{
	return (getPosition() - m_endPosition).lengthSquared() < ACTIVATE_RANGE_SQUARED;
}

void Player::reset()
{
	setPosition({ 0, 50 });
	m_velocity = { 0,0 };
	m_leverPulled = false;
	m_gameEndTriggered = false;
}