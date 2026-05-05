#include "AudioManager.h"

AudioManager::AudioManager()
{
	addSound("jump", "sfx/jump.mp3");
	addSound("wind", "sfx/wind.mp3");
	addSound("death", "sfx/death.mp3");

	addMusic("bgm1", "sfx/bgm1.mp3");
	getMusic("bgm1")->setVolume(20.f);
	addMusic("bgm2", "sfx/bgm2.mp3");
	getMusic("bgm2")->setVolume(20.f);
	addMusic("bgm3", "sfx/bgm3.mp3");
	getMusic("bgm3")->setVolume(20.f);

}

AudioManager::~AudioManager()
{
}

void AudioManager::addSound(const std::string& tag, const std::string& filename)
{
	if (!m_sounds[tag].loadSound(filename))
	{
		std::cerr << "AudioManager failed to load sound: " << filename << " with tag: " << tag << "\n";
	}
}

void AudioManager::playSoundbyName(const std::string& tag)
{
	auto it = m_sounds.find(tag);
	if (it != m_sounds.end())
		it->second.getSound().play();
}

void AudioManager::stopAllSounds()
{
	for (auto& pair : m_sounds)
	{
		pair.second.getSound().stop();
	}
}

sf::Sound* AudioManager::getSound(const std::string& tag)
{
	auto it = m_sounds.find(tag);
	if (it != m_sounds.end())
		return &it->second.getSound();

	return nullptr;
}

void AudioManager::addMusic(const std::string& tag, const std::string& filename)
{
	auto music = std::make_unique<sf::Music>();

	if (!music->openFromFile(filename))
	{
		std::cerr << "Error: Could not load music file: " << filename << std::endl;
		return;
	}

	m_musicStreams[tag] = std::move(music);
}

void AudioManager::playMusicbyName(const std::string& tag)
{
	auto it = m_musicStreams.find(tag);
	if (it != m_musicStreams.end())
		it->second->play();
}

void AudioManager::stopAllMusic()
{
	for (auto& pair : m_musicStreams)
	{
		pair.second->stop();
	}
}

sf::Music* AudioManager::getMusic(const std::string& tag)
{
	auto it = m_musicStreams.find(tag);
	if (it != m_musicStreams.end())
		return it->second.get();

	return nullptr;
}
