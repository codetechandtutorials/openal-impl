#include "SoundSource.h"
#include <iostream>

SoundSource::SoundSource()
{
	alGenSources(1, &p_Source);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &p_Source);
}

void SoundSource::Play(const ALuint buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
			p_Buffer = buffer_to_play;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);


	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		std::cout << "currently playing sound\n";
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	}
	std::cout << "done playing sound\n";
}
