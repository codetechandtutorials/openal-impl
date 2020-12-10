#include "SoundEffectsPlayer.h"
#include <iostream>
#include "OpenALErrorCheck.h"

SoundEffectsPlayer::SoundEffectsPlayer()
{
	alGenSources(1, &p_Source);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	AL_CheckAndThrow();
}

SoundEffectsPlayer::~SoundEffectsPlayer()
{
	alDeleteSources(1, &p_Source);
}

void SoundEffectsPlayer::Play(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
			p_Buffer = buffer_to_play;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
			AL_CheckAndThrow();
	}
	alSourcePlay(p_Source);
	AL_CheckAndThrow();
}

void SoundEffectsPlayer::Stop()
{
	alSourceStop(p_Source);
	AL_CheckAndThrow();
}

void SoundEffectsPlayer::Pause()
{
	alSourcePause(p_Source);
	AL_CheckAndThrow();
}

void SoundEffectsPlayer::Resume()
{
	alSourcePlay(p_Source);
	AL_CheckAndThrow();
}

void SoundEffectsPlayer::SetBufferToPlay(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		AL_CheckAndThrow();
	}
}

void SoundEffectsPlayer::SetLooping(const bool& loop)
{
	alSourcei(p_Source, AL_LOOPING, (ALint)loop);
	AL_CheckAndThrow();
}

void SoundEffectsPlayer::SetPosition(const float& x, const float& y, const float& z)
{
	alSource3f(p_Source, AL_POSITION, x, y, z);
	AL_CheckAndThrow();
}

bool SoundEffectsPlayer::isPlaying()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PLAYING);
}
