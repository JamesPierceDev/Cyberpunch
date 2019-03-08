#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "SDL_mixer.h"
#include "Component.h"

class AudioComponent : public Component
{
public:

	struct Sound
	{
	public:
		Sound() {};
		Sound(std::string name) :m_name(name), m_isSFX(false) {}
		std::string& getName() { return m_name; }
		void setSound(Mix_Chunk* sfx) { m_isSFX = true, m_sfx = sfx; }
		void setSound(Mix_Music* music) { m_isSFX = false, m_music = music;}
		void play(bool loop)
		{
			if (m_isSFX)
				Mix_PlayChannel(-1, m_sfx, loop ? -1 : 0);
			else
				Mix_PlayMusic(m_music, loop ? -1 : 0);
		}

		void stop() {
		
			Mix_HaltMusic();
		}

	private:
		std::string m_name;
		bool m_isSFX;
		Mix_Chunk* m_sfx;
		Mix_Music* m_music;

	};

	AudioComponent() {}

	void addSound(std::string name, Mix_Chunk* sound)
	{
		m_sounds[name] = Sound(name);
		m_sounds[name].setSound(sound);
	}
	void addSound(std::string name, Mix_Music* sound)
	{
		m_sounds[name] = Sound(name);
		m_sounds[name].setSound(sound);
	}
	void playSound(std::string name, bool loop)
	{
		m_current = &m_sounds[name];
		m_current->play(loop);
	}
	void stop()
	{
		m_current->stop();
	}
	std::string getCurrentID() { return m_current->getName(); }
	Sound* getCurrentSound() { return m_current; }

private:

	Sound * m_current;
	std::map<std::string, Sound>m_sounds;
};


#endif