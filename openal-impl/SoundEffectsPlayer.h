#pragma once
#include <AL\al.h>
class SoundEffectsPlayer
{
public:
	SoundEffectsPlayer();
	~SoundEffectsPlayer();

	void Play(const ALuint& buffer_to_play);
	void Stop();
	void Pause();
	void Resume();

	void SetBufferToPlay(const ALuint& buffer_to_play);
	void SetLooping(const bool& loop);

	bool isPlaying();

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
};

